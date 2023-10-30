#include "ImGui/IG_Tester.h"
#include "ImGui/imgui.h"
#include "Apps/Tester.h"

void IG_Tester::render()
{
	newFrame();

	if (ImGui::Begin("Camera View"))
	{
		ImGui::SetWindowPos(ImVec2(2, 2), ImGuiCond_Once);
		ImGui::SetWindowCollapsed(true, ImGuiCond_Once);
		ImGui::SetWindowSize(ImVec2(315, 80), ImGuiCond_Once);

		ImGui::SliderAngle("Theta", &IG_DATA_TESTER::THETA);
		ImGui::SliderAngle("Phi", &IG_DATA_TESTER::PHI, -90, 90);
	}
	ImGui::End();

	drawFrame();
}