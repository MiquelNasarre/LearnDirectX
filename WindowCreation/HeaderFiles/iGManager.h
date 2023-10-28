#pragma once

class iGManager {
private:
	static iGManager main;

	iGManager();
	~iGManager();
	static void newFrame();
	static void drawFrame();

public:

	static void initWin32(void* hWnd);
	static void initDX11(void* pDevice, void* pContext);
	static void closeWin32();
	static void closeDX11();

	static bool WndProcHandler(void* hWnd, unsigned int msg, unsigned int wParam, unsigned int lParam);
	static float* getData();
	static void render();
};