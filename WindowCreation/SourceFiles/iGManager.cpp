#include "iGManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_internal.h"
#include "imgui/imconfig.h"
#include "imgui/imstb_rectpack.h"
#include "imgui/imstb_textedit.h"
#include "imgui/imstb_truetype.h"

iGManager iGManager::main;

float* iGManager::data = NULL;

iGManager::iGManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	data = (float*)calloc(sizeof(float), __IMGUIDATA_END__);
}

iGManager::~iGManager()
{
	if (data)
		free(data);
	data = NULL;
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

float* iGManager::getData()
{
	return data;
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

void iGManager::render()
{
	newFrame();

	//	Add all imGui functionalities here

	if (ImGui::Begin("Settings")) {
		if (data[IMGUIDATA_EARTH_THETA] > 2.f * 3.141593f)
			data[IMGUIDATA_EARTH_THETA] -= 4.f * 3.141593f;
		if (data[IMGUIDATA_EARTH_THETA] < -2.f * 3.141593f)
			data[IMGUIDATA_EARTH_THETA] += 4.f * 3.141593f;
		

		ImGui::SliderFloat("Speed", &data[IMGUIDATA_SPEED], -1.5f, 1.5f,"%0.3f");
		ImGui::SliderAngle("Rotation", &data[IMGUIDATA_EARTH_THETA]);
		ImGui::SliderAngle("Pitch", &data[IMGUIDATA_EARTH_PHI],-90.f,90.f);
		ImGui::Text("Observer/Camera position:");
		ImGui::SliderAngle("Theta", &data[IMGUIDATA_THETA]);
		ImGui::SliderAngle("Phi", &data[IMGUIDATA_PHI],-90.f,90.f);
		ImGui::SliderFloat("Background View", &data[IMGUIDATA_FOV], 0.2f, 2.f);

		if (data[IMGUIDATA_PHI] > 3.140f / 2.f)
			data[IMGUIDATA_PHI] = 3.140f / 2.f;
		if (data[IMGUIDATA_PHI] < -3.140f / 2.f)
			data[IMGUIDATA_PHI] = -3.140f / 2.f;
	}
	ImGui::End();

	//	....

	drawFrame();
}
