#include "IG_Fourier.h"
#include "Fourier.h"
#include "imgui/imgui.h"

void IG_Fourier::render()
{
	newFrame();

	if (ImGui::Begin("Selector"))
	{
		ImGui::SetWindowPos(ImVec2(2, 2), ImGuiCond_Once);
		ImGui::SetWindowCollapsed(true, ImGuiCond_Once);
		ImGui::SetWindowSize(ImVec2(315, 120), ImGuiCond_Once);

		if (ImGui::SliderInt("Valor de L", &IG_DATA::L, 0, 10))IG_DATA::UPDATE = true;
		if (ImGui::SliderInt("Valor de M", &IG_DATA::M, -IG_DATA::L, IG_DATA::L))IG_DATA::UPDATE = true;

		if (IG_DATA::M > IG_DATA::L) IG_DATA::M = IG_DATA::L;
		if (-IG_DATA::M > IG_DATA::L) IG_DATA::M = -IG_DATA::L;
	}
	ImGui::End();

	drawFrame();
}
