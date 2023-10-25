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

bool iGManager::show_demo_window = true;

iGManager::iGManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

void iGManager::initWin32(void* hWnd)
{
	ImGui_ImplWin32_Init((HWND)hWnd);
}

void iGManager::initDX11(void* pDevice, void* pContext)
{
	ImGui_ImplDX11_Init((ID3D11Device*)pDevice, (ID3D11DeviceContext*)pContext);
}

bool iGManager::WndProcHandler(void* hWnd, unsigned int msg, unsigned int wParam, unsigned int lParam)
{
	ImGui_ImplWin32_WndProcHandler((HWND)hWnd, msg, wParam, lParam);
	const auto& imio = ImGui::GetIO();
	if (imio.WantCaptureKeyboard || imio.WantCaptureMouse)
		return true;
	return false;
}

void iGManager::render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
