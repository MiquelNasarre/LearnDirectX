#include "iGManager.h"

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

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
