#include "IG_Fourier.h"
#include "Fourier.h"
#include "imgui/imgui.h"

bool IG_Fourier::loadMenuOpen = false;
const char** IG_Fourier::figureNames;
unsigned int* IG_Fourier::figureSizes;

void IG_Fourier::render()
{
	newFrame();

	if (ImGui::Begin("Selector",NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar))
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
			/*if (ImGui::BeginMenu("Textures"))
				{
					if (ImGui::BeginMenu("Earth"))
					{
						if (ImGui::MenuItem("Default"))
							IG_DATA::UPDATE_TEXTURE = Vector2i(0, 0);
						if (ImGui::MenuItem("Inverted"))
							IG_DATA::UPDATE_TEXTURE = Vector2i(0, 1);
						if (ImGui::MenuItem("Chalked"))
							IG_DATA::UPDATE_TEXTURE = Vector2i(0, 2);
						if (ImGui::MenuItem("Moon"))
							IG_DATA::UPDATE_TEXTURE = Vector2i(0, 3);
						if (ImGui::MenuItem("No night"))
							IG_DATA::UPDATE_TEXTURE = Vector2i(0, 4);
						if (ImGui::MenuItem("Only night"))
							IG_DATA::UPDATE_TEXTURE = Vector2i(0, 5);

						ImGui::EndMenu();
					}
					if (ImGui::BeginMenu("Moon"))
					{
						if (ImGui::MenuItem("Default"))
							IG_DATA::UPDATE_TEXTURE = Vector2i(1, 0);
						if (ImGui::MenuItem("Inverted"))
							IG_DATA::UPDATE_TEXTURE = Vector2i(1, 1);
						if (ImGui::MenuItem("Chalked"))
							IG_DATA::UPDATE_TEXTURE = Vector2i(1, 2);
						if (ImGui::MenuItem("Earth"))
							IG_DATA::UPDATE_TEXTURE = Vector2i(1, 3);

						ImGui::EndMenu();
					}
					if (ImGui::BeginMenu("Background"))
					{
						if (ImGui::MenuItem("Default"))
							IG_DATA::UPDATE_TEXTURE = Vector2i(2, 0);
						if (ImGui::MenuItem("Inverted"))
							IG_DATA::UPDATE_TEXTURE = Vector2i(2, 1);
						if (ImGui::MenuItem("Earth"))
							IG_DATA::UPDATE_TEXTURE = Vector2i(2, 2);
						if (ImGui::MenuItem("Moon"))
							IG_DATA::UPDATE_TEXTURE = Vector2i(2, 3);

						ImGui::EndMenu();
					}
					ImGui::Spacing();
					if (ImGui::MenuItem("Reset"))
						IG_DATA::UPDATE_TEXTURE = Vector2i(3, 0);

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
					IG_DATA::UPDATE_LIGHT = -2;
					light = -1;
				}

				ImGui::EndMenu();
			}*/

			ImGui::EndMenuBar();
		}

		ImGui::SetWindowPos(ImVec2(2, 2), ImGuiCond_Once);
		ImGui::SetWindowCollapsed(true, ImGuiCond_Once);
		ImGui::SetWindowSize(ImVec2(350, 120), ImGuiCond_Once);

		if (ImGui::SliderInt("Valor de L", &IG_DATA::L, 0, MAX_L))IG_DATA::UPDATE = true;
		if (ImGui::SliderInt("Valor de M", &IG_DATA::M, -IG_DATA::L, IG_DATA::L))IG_DATA::UPDATE = true;

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
	}
	ImGui::End();

	drawFrame();
}

void IG_Fourier::loadMenu()
{
	if (ImGui::Begin("Load menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
	{
		ImGui::SetWindowPos(ImVec2(IG_DATA::WindowDim.x / 2.f - 100.f, IG_DATA::WindowDim.y / 2.f - 52.f));
		ImGui::SetWindowSize(ImVec2(200, 104), ImGuiCond_Once);

		static char* filename = (char*)calloc(100,sizeof(char));
		static bool exists = false;

		if (!exists)
		{
			if (ImGui::InputText(" File*", filename, 100, ImGuiInputTextFlags_AutoSelectAll))
			{
				FILE* file = fopen((FIGURES_DIR + std::string(filename) + ".txt").c_str(), "r");
				if (file)
				{
					exists = true;
					fclose(file);
				}
			}
		}
		else
		{
			if (ImGui::InputText(" File", filename, 100, ImGuiInputTextFlags_AutoSelectAll))
			{
				FILE* file = fopen((FIGURES_DIR + std::string(filename) + ".txt").c_str(), "r");
				if (file)
					fclose(file);
				else
					exists = false;
			}
		}

		IG_DATA::FILENAME = filename;

		static int L = 20;
		ImGui::InputInt(" Max L", &L);
		if (L < 0)
			L = 0;
		IG_DATA::MAXL = (unsigned int)L;

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
