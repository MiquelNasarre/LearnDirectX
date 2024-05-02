#include "iGManager.h"
#include <Window.h>
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

unsigned int iGManager::contextCount = 0u;

iGManager::iGManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	Context = ImGui::GetCurrentContext();
	contextCount++;
}

iGManager::~iGManager()
{
	contextCount--;
	if (!contextCount)
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
	}

}

void* iGManager::getContext()
{
	return Context;
}

void iGManager::initWin32(void* hWnd)
{
	if (contextCount == 1)
		ImGui_ImplWin32_Init((HWND)hWnd);
}

void iGManager::initDX11(void* pDevice, void* pContext)
{
	if (contextCount == 1)
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
