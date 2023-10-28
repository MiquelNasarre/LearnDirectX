#include "iGManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_internal.h"
#include "imgui/imconfig.h"
#include "imgui/imstb_rectpack.h"
#include "imgui/imstb_textedit.h"
#include "imgui/imstb_truetype.h"

#include "Application.h"

iGManager iGManager::main;

iGManager::iGManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

iGManager::~iGManager()
{
}

void iGManager::initWin32(void* hWnd)
{
	ImGui_ImplWin32_Init((HWND)hWnd);
}

void iGManager::initDX11(void* pDevice, void* pContext)
{
	ImGui_ImplDX11_Init((ID3D11Device*)pDevice, (ID3D11DeviceContext*)pContext);
}

void iGManager::closeWin32()
{
	ImGui_ImplWin32_Shutdown();
}

void iGManager::closeDX11()
{
	ImGui_ImplDX11_Shutdown();
}

bool iGManager::WndProcHandler(void* hWnd, unsigned int msg, unsigned int wParam, unsigned int lParam)
{
	ImGui_ImplWin32_WndProcHandler((HWND)hWnd, msg, wParam, lParam);
	const auto& imio = ImGui::GetIO();
	if (imio.WantCaptureKeyboard || imio.WantCaptureMouse)
		return true;
	return false;
}

void iGManager::newFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void iGManager::drawFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

//	Application functions

static IG_DATA::lightsource savestate;

void saveLightState(int id)
{
	savestate = IG_DATA::LIGHTS[id];
}

void loadLightState(int id)
{
	IG_DATA::LIGHTS[id] = savestate;
}

void doLightEditor(int& id)
{

	if (ImGui::Begin("Light editor", NULL, ImGuiWindowFlags_NoResize))
	{
		ImGui::Text("Color:");
		if (ImGui::ColorPicker4("", (float*)&IG_DATA::LIGHTS[id].color))
			IG_DATA::UPDATE_LIGHT = id;
		ImGui::Spacing();
		ImGui::Text("Intensities:");
		if(ImGui::SliderFloat2("", (float*)&IG_DATA::LIGHTS[id].intensities, 0.f, 100000.f, "%.3f", 4))
			IG_DATA::UPDATE_LIGHT = id;
		ImGui::Spacing();
		ImGui::Text("Position:");
		if(ImGui::SliderFloat3(" ", (float*)&IG_DATA::LIGHTS[id].position, -1000.f, 1000.f, "%.3f", 4))
			IG_DATA::UPDATE_LIGHT = id;
		
		ImGui::SetCursorPos(ImVec2(290, 285));
		if (ImGui::Button("Cancel", ImVec2(80, 45))) {
			loadLightState(id);
			IG_DATA::UPDATE_LIGHT = id;
			id = -1;
		}
		ImGui::SetCursorPos(ImVec2(290, 340));
		if (ImGui::Button("Delete", ImVec2(80, 45))) {
			IG_DATA::LIGHTS[id].is_on = false;
			IG_DATA::LIGHTS[id].position = { 0.f,0.f,0.f,0.f };
			IG_DATA::LIGHTS[id].intensities = { 0.f,0.f,0.f,0.f };
			IG_DATA::LIGHTS[id].color = { 0.f,0.f,0.f,0.f };
			IG_DATA::UPDATE_LIGHT = id;
			id = -1;
		}
		ImGui::SetCursorPos(ImVec2(290, 395));
		if (ImGui::Button("Apply", ImVec2(80, 45)))
			id = -1;
		
	}
	ImGui::End();
}

void iGManager::render()
{
	static int light = -1;

	newFrame();

	//	Add all imGui functionalities here

	if (ImGui::Begin("Settings", NULL, ImGuiWindowFlags_MenuBar)) {

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Textures"))
			{
				if (ImGui::BeginMenu("Earth"))
				{
					if (ImGui::MenuItem("Default"))
						IG_DATA::TEXTURE_EARTH = 0;
					if (ImGui::MenuItem("Inverted"))
						IG_DATA::TEXTURE_EARTH = 1;
					if (ImGui::MenuItem("Chalked"))
						IG_DATA::TEXTURE_EARTH = 2;
					if (ImGui::MenuItem("Moon"))
						IG_DATA::TEXTURE_EARTH = 3;

					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Moon"))
				{
					if (ImGui::MenuItem("Default"))
						IG_DATA::TEXTURE_MOON = 0;
					if (ImGui::MenuItem("Inverted"))
						IG_DATA::TEXTURE_MOON = 1;
					if (ImGui::MenuItem("Chalked"))
						IG_DATA::TEXTURE_MOON = 2;
					if (ImGui::MenuItem("Earth"))
						IG_DATA::TEXTURE_MOON = 3;

					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Background"))
				{
					if (ImGui::MenuItem("Default"))
						IG_DATA::TEXTURE_BACKGROUND = 0;
					if (ImGui::MenuItem("Inverted"))
						IG_DATA::TEXTURE_BACKGROUND = 1;
					if (ImGui::MenuItem("Earth"))
						IG_DATA::TEXTURE_BACKGROUND = 2;
					if (ImGui::MenuItem("Moon"))
						IG_DATA::TEXTURE_BACKGROUND = 3;

					ImGui::EndMenu();
				}
				ImGui::Spacing();
				if(ImGui::MenuItem("Reset"))
				{
					IG_DATA::TEXTURE_EARTH = 0;
					IG_DATA::TEXTURE_MOON = 0;
					IG_DATA::TEXTURE_BACKGROUND = 0;
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Lights"))
			{
				for (int i = 0; i < 8; i++) {
					if (IG_DATA::LIGHTS[i].is_on && ImGui::MenuItem(std::string("Light " + std::to_string(i)).c_str()))
					{
						light = i;
						saveLightState(i);
					}
				}
				ImGui::Spacing();

				if (ImGui::MenuItem("New"))
				{
					for (int i = 0; i < 8; i++) {
						if (!IG_DATA::LIGHTS[i].is_on) {
							IG_DATA::LIGHTS[i].is_on = true;
							light = i;
							break;
						}
					}
				}

				if (ImGui::MenuItem("Reset"))
				{
					free(IG_DATA::LIGHTS);
					IG_DATA::LIGHTS = (IG_DATA::lightsource*)calloc(8, sizeof(IG_DATA::lightsource));
					IG_DATA::LIGHTS[0].is_on = true;
					IG_DATA::LIGHTS[0].color = Color::White.getColor4();
					IG_DATA::LIGHTS[0].intensities = { 32000.f,5000.f };
					IG_DATA::LIGHTS[0].position = { 160.f,0.f,60.f };
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Spacing();
		ImGui::Text("Earth & Moon movement:");

		ImGui::SliderFloat("Speed",	&IG_DATA::SPEED, -1.5f, 1.5f,"%0.3f");
		ImGui::SliderAngle("Rotation", &IG_DATA::EARTH_THETA);
		ImGui::SliderAngle("Pitch", &IG_DATA::EARTH_PHI,-90.f,90.f);
		ImGui::SliderFloat("Moon Speed", &IG_DATA::MOON_SPEED, -4.f, 4.f, "%0.3f");

		ImGui::Spacing();
		ImGui::Text("Observer/Camera position:");

		ImGui::SliderAngle("Theta", &IG_DATA::THETA);
		ImGui::SliderAngle("Phi", &IG_DATA::PHI,-90.f,90.f);
		ImGui::SliderFloat("Background View", &IG_DATA::FOV, 0.2f, 2.f);


		if (IG_DATA::EARTH_THETA > 2.f * 3.141593f)
			IG_DATA::EARTH_THETA -= 4.f * 3.141593f;
		if (IG_DATA::EARTH_THETA < -2.f * 3.141593f)
			IG_DATA::EARTH_THETA += 4.f * 3.141593f;

		if (IG_DATA::PHI > 3.140f / 2.f)
			IG_DATA::PHI = 3.140f / 2.f;
		if (IG_DATA::PHI < -3.140f / 2.f)
			IG_DATA::PHI = -3.140f / 2.f;
	}
	ImGui::End();

	if (light > -1)
		doLightEditor(light);

	//	....

	drawFrame();
}
