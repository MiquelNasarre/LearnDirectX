#include "IG_Fourier.h"
#include "Fourier.h"
#include "imgui/imgui.h"

void IG_Fourier::render()
{
	newFrame();

	if (ImGui::Begin("Selector",NULL, ImGuiWindowFlags_NoResize))
	{
		ImGui::SetWindowPos(ImVec2(2, 2), ImGuiCond_Once);
		ImGui::SetWindowCollapsed(true, ImGuiCond_Once);
		ImGui::SetWindowSize(ImVec2(350, 100), ImGuiCond_Once);

		if (ImGui::SliderInt("Valor de L", &IG_DATA::L, 0, MAX_L))IG_DATA::UPDATE = true;
		if (ImGui::SliderInt("Valor de M", &IG_DATA::M, -IG_DATA::L, IG_DATA::L))IG_DATA::UPDATE = true;

		if (IG_DATA::M > IG_DATA::L) IG_DATA::M = IG_DATA::L;
		if (-IG_DATA::M > IG_DATA::L) IG_DATA::M = -IG_DATA::L;

		if (IG_DATA::CURVES)
		{
			if (ImGui::Button("Hide curves"))
			{
				IG_DATA::CURVES = false;
				ImGui::SetWindowSize(ImVec2(315, 100));
			}

			if (ImGui::SliderFloat("phi", &IG_DATA::phi, 0.f, 2 * pi)) IG_DATA::UPDATE_CURVES = true;
			if (ImGui::SliderFloat("theta", &IG_DATA::theta, 0.f, 2 * pi)) IG_DATA::UPDATE_CURVES = true;
		}
		else if (ImGui::Button("Show curves"))
		{
			ImGui::SetWindowSize(ImVec2(315, 148));
			IG_DATA::CURVES = true;
			IG_DATA::UPDATE_CURVES = true;
		}


	}
	ImGui::End();

	drawFrame();
}
