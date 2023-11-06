#include "IG_Tester.h"
#include "ImGui/imgui.h"
#include "Tester.h"

void IG_Tester::render()
{
	newFrame();

	if (ImGui::Begin("Camera View"))
	{
		ImGui::SetWindowPos(ImVec2(2, 2), ImGuiCond_Once);
		ImGui::SetWindowCollapsed(true, ImGuiCond_Once);
		ImGui::SetWindowSize(ImVec2(315, 80), ImGuiCond_Once);

		ImGui::SliderAngle("Theta", &IG_DATA::THETA);
		ImGui::SliderAngle("Phi", &IG_DATA::PHI, -90, 90);
	}
	ImGui::End();

	drawFrame();
}
