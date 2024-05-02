#pragma once

class iGManager {
	friend class Window;
	friend class Graphics;
protected:
	void* Context;
	static unsigned int contextCount;

	iGManager();
	~iGManager();
	static void newFrame();
	static void drawFrame();

	void* getContext();

	static void initWin32(void* hWnd);
	static void initDX11(void* pDevice, void* pContext);

	static bool WndProcHandler(void* hWnd, unsigned int msg, unsigned int wParam, unsigned int lParam);
};