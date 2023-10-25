#pragma once

class iGManager {
private:
	static iGManager main;

	static bool show_demo_window;
public:
	iGManager();

	static void initWin32(void* hWnd);
	static void initDX11(void* pDevice, void* pContext);

	static bool WndProcHandler(void* hWnd, unsigned int msg, unsigned int wParam, unsigned int lParam);

	static void render();
};