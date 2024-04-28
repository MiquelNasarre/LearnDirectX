#include "IG_Fourier.h"
#include "Fourier.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

bool IG_Fourier::loadMenuOpen = false;
bool IG_Fourier::colorPickerOpen = false;
const char** IG_Fourier::figureNames;
unsigned int* IG_Fourier::figureSizes;

static IG_DATA::lightsource savestate;

//	Private

void IG_Fourier::saveLightState(int id)
{
	savestate = IG_DATA::LIGHTS[id];
}

void IG_Fourier::loadLightState(int id)
{
	IG_DATA::LIGHTS[id] = savestate;
}

void IG_Fourier::doLightEditor(int& id)
{
	if (ImGui::Begin(" Light editor", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
	{
		ImGui::SetWindowSize(ImVec2(340, 430));

		ImGui::Text("Color:");
		if (ImGui::ColorPicker4("", (float*)&IG_DATA::LIGHTS[id].color))
			IG_DATA::UPDATE_LIGHT = id;
		ImGui::Spacing();
		ImGui::Text("Intensities:");
		if (ImGui::SliderFloat2("", (float*)&IG_DATA::LIGHTS[id].intensities, 0.f, 100000.f, "%.3f", 4))
			IG_DATA::UPDATE_LIGHT = id;
		ImGui::Spacing();
		ImGui::Text("Position:");
		if (ImGui::SliderFloat3(" ", (float*)&IG_DATA::LIGHTS[id].position, -1000.f, 1000.f, "%.3f", 4))
			IG_DATA::UPDATE_LIGHT = id;

		ImGui::SetCursorPos(ImVec2(245, 235));
		if (ImGui::Button("Cancel", ImVec2(80, 45))) {
			loadLightState(id);
			IG_DATA::UPDATE_LIGHT = id;
			id = -1;
		}
		ImGui::SetCursorPos(ImVec2(245, 290));
		if (ImGui::Button("Delete", ImVec2(80, 45))) {
			IG_DATA::LIGHTS[id].is_on = false;
			IG_DATA::LIGHTS[id].position = { 0.f,0.f,0.f,0.f };
			IG_DATA::LIGHTS[id].intensities = { 0.f,0.f,0.f,0.f };
			IG_DATA::LIGHTS[id].color = { 0.f,0.f,0.f,0.f };
			IG_DATA::UPDATE_LIGHT = id;
			id = -1;
		}
		ImGui::SetCursorPos(ImVec2(245, 345));
		if (ImGui::Button("Apply", ImVec2(80, 45)))
			id = -1;

	}
	ImGui::End();
}

void IG_Fourier::loadMenu()
{
	if (ImGui::Begin("Load Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::SetWindowPos(ImVec2(IG_DATA::WindowDim.x / 2.f - 100.f, IG_DATA::WindowDim.y / 2.f - 52.f));

		if(IG_DATA::FIGURE_FILE)
			ImGui::SetWindowSize(ImVec2(200, 104));

		else
			ImGui::SetWindowSize(ImVec2(200, 81));


		static bool change = false;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Load Menu", false))
				ImGui::EndMenu();

			const char* label;
			if (IG_DATA::FIGURE_FILE)
				label = "Figure";
			else
				label = "Coefficients";

			if (ImGui::BeginMenu(label))
			{
				if (ImGui::MenuItem("Figure"))
				{
					IG_DATA::FIGURE_FILE = true;
					change = true;
				}
				if (ImGui::MenuItem("Coefficients"))
				{
					IG_DATA::FIGURE_FILE = false;
					change = true;
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		static char* filename = (char*)calloc(100,sizeof(char));
		static bool exists = false;

		if (!exists)
		{
			if (ImGui::InputText(" File*", filename, 100, ImGuiInputTextFlags_AutoSelectAll) || change)
			{
				change = false;
				FILE* file;
				if (IG_DATA::FIGURE_FILE)
					file = fopen((FIGURES_DIR + std::string(filename) + ".dat").c_str(), "r");
				else
					file = fopen((COEFFICIENTS_DIR + std::string(filename) + ".dat").c_str(), "r");
				if (file)
				{
					exists = true;
					fclose(file);
				}
			}
		}
		else
		{
			if (ImGui::InputText(" File", filename, 100, ImGuiInputTextFlags_AutoSelectAll) || change)
			{
				change = false;
				FILE* file;
				if (IG_DATA::FIGURE_FILE)
					file = fopen((FIGURES_DIR + std::string(filename) + ".dat").c_str(), "r");
				else
					file = fopen((COEFFICIENTS_DIR + std::string(filename) + ".dat").c_str(), "r");
				if (file)
					fclose(file);
				else
					exists = false;
			}
		}

		IG_DATA::FILENAME = filename;

		if (IG_DATA::FIGURE_FILE)
		{
			static int L = 20;
			ImGui::InputInt(" Max L", &L);
			if (L < 0)
				L = 0;
			if (L > MAX_L)
				L = MAX_L;
			IG_DATA::MAXL = (unsigned int)L;
		}

		if (ImGui::Button("Load") && exists)
		{
			figureNames = (const char**)memcpy(calloc(IG_DATA::NFIG + 1, sizeof(const char*)), figureNames, IG_DATA::NFIG * sizeof(const char*));
			figureNames[IG_DATA::NFIG] = (const char*)memcpy(calloc(100, sizeof(char)), filename, 100 * sizeof(char));

			figureSizes = (unsigned int*)memcpy(calloc(IG_DATA::NFIG + 1, sizeof(int)), figureSizes, IG_DATA::NFIG * sizeof(int));
			figureSizes[IG_DATA::NFIG] = IG_DATA::MAXL;

			IG_DATA::CALCULATE_FIGURE = true;
			loadMenuOpen = false;
		}

		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 45.f, ImGui::GetCursorPosY() - 23.f));
		if (ImGui::Button("Cancel"))
			loadMenuOpen = false;

	}
	ImGui::End();

}

void IG_Fourier::colorPicker()
{
	static _float4color color = Color::Red.getColor4();

	if (ImGui::Begin(" Light editor", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
	{
		ImGui::SetWindowSize(ImVec2(350, 380));

		ImGui::Text("Color:");
		if (ImGui::ColorPicker4("", (float*)&color))

		ImGui::SetCursorPos(ImVec2(8, 321));
		if (ImGui::Button("Cancel", ImVec2(110, 45))) 
		{
			color = IG_DATA::TEXTURE;
			colorPickerOpen = false;
		}
		ImGui::SetCursorPos(ImVec2(125, 321));

		if (ImGui::Button("Apply", ImVec2(110, 45)))
		{
			colorPickerOpen = false;
			IG_DATA::UPDATE_TEXTURE = true;
			IG_DATA::TEXTURE = color;
		}
	}
	ImGui::End();
}

//	Public

void IG_Fourier::render()
{
	static int light = -1;
	newFrame();

	if (ImGui::Begin("Selector", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar))
	{

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Figure"))
			{
				if (ImGui::MenuItem("Spehrical Harmonics"))
				{
					IG_DATA::FIGURE_VIEW = -1;
					IG_DATA::UPDATE_CURVES = true;
				}

				for (unsigned int i = 0; i < IG_DATA::NFIG; i++)
				{
					if (ImGui::MenuItem(figureNames[i], std::to_string(figureSizes[i]).c_str()))
					{
						IG_DATA::FIGURE_VIEW = i;
						IG_DATA::UPDATE_CURVES = true;
					}
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Advanced"))
			{
				static bool datasetStarted = false;

				if (datasetStarted)
				{
					unsigned int d = FourierSurface::depthDataset();
					if (d < MAX_L)
						ImGui::MenuItem("Generating", (std::to_string(d) + "/" + std::to_string(MAX_L)).c_str(), false, false);
					else
						ImGui::MenuItem("Generated", (std::to_string(d) + "/" + std::to_string(MAX_L)).c_str(), false, false);
				}

				ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
				if (!datasetStarted && ImGui::MenuItem("Generate dataset"))
				{
					datasetStarted = true;
					FourierSurface::generateDataSet();
				}
				ImGui::PopItemFlag();

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
							saveLightState(i);
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
					IG_DATA::LIGHTS[0].color = { 1.f, 1.f, 1.f, 1.f };
					IG_DATA::LIGHTS[0].intensities = { 600.f, 320.f, 0.f, 0.f };
					IG_DATA::LIGHTS[0].position = { 30.f, 10.f, 20.f, 0.f };
					IG_DATA::UPDATE_LIGHT = -2;
					light = -1;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Textures"))
			{
				if (ImGui::MenuItem("Default"))
				{
					IG_DATA::UPDATE_TEXTURE = true;
					IG_DATA::TEXTURE = { -1.f,0.f,0.f,0.f };
				}
				if (ImGui::MenuItem("Pick color"))
					colorPickerOpen = true;
				if (ImGui::MenuItem("Randomized"))
				{
					IG_DATA::UPDATE_TEXTURE = true;
					IG_DATA::TEXTURE = { 0.f,-1.f,0.f,0.f };
				}

				ImGui::EndMenu();
			}
			

			ImGui::EndMenuBar();
		}

		ImGui::SetWindowPos(ImVec2(2, 2), ImGuiCond_Once);
		ImGui::SetWindowCollapsed(true, ImGuiCond_Once);
		ImGui::SetWindowSize(ImVec2(350, 120), ImGuiCond_Once);

		if (ImGui::SliderInt("Valor de L", &IG_DATA::L, 0, MAX_L))
		{
			IG_DATA::UPDATE = true;
			IG_DATA::UPDATE_TEXTURE = true;
		}

		if (ImGui::SliderInt("Valor de M", &IG_DATA::M, -IG_DATA::L, IG_DATA::L))
		{
			IG_DATA::UPDATE = true;
			IG_DATA::UPDATE_TEXTURE = true;
		}

		if (IG_DATA::M > IG_DATA::L) IG_DATA::M = IG_DATA::L;
		if (-IG_DATA::M > IG_DATA::L) IG_DATA::M = -IG_DATA::L;

		if (!IG_DATA::LOADING && ImGui::Button("Load Figure"))
			loadMenuOpen = true;

		static int counter = 0;
		static std::string loadText = "Loading...";
		if (IG_DATA::LOADING)
		{
			counter++;
			counter %= 120;
			if (counter < 30)
				loadText = "Loading.";
			else if (counter < 60)
				loadText = "Loading..";
			else if (counter < 90)
				loadText = "Loading...";
			else
				loadText = "Loading....";
			ImGui::Button(loadText.c_str());
		}

		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 100.f, ImGui::GetCursorPosY() - 23.f));

		if (IG_DATA::CURVES)
		{
			if (ImGui::Button("Hide curves"))
			{
				IG_DATA::CURVES = false;
				ImGui::SetWindowSize(ImVec2(350, 120));
			}

			if (ImGui::SliderFloat("phi", &IG_DATA::phi, 0.f, 2 * pi)) IG_DATA::UPDATE_CURVES = true;
			if (ImGui::SliderFloat("theta", &IG_DATA::theta, 0.f, pi)) IG_DATA::UPDATE_CURVES = true;
		}
		else if (ImGui::Button("Show curves"))
		{
			ImGui::SetWindowSize(ImVec2(350, 168));
			IG_DATA::CURVES = true;
			IG_DATA::UPDATE_CURVES = true;
		}

		if (loadMenuOpen)
			loadMenu();

		if (light > -1)
			doLightEditor(light);

		if (colorPickerOpen)
			colorPicker();
	}
	ImGui::End();

	drawFrame();
}
