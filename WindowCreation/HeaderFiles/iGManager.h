#pragma once

enum : unsigned int {
	IMGUIDATA_EARTH_THETA,
	IMGUIDATA_EARTH_PHI,
	IMGUIDATA_SPEED,
	IMGUIDATA_MOON_SPEED,
	IMGUIDATA_MOON_POS,
	IMGUIDATA_THETA,
	IMGUIDATA_PHI,
	IMGUIDATA_FOV,
	IMGUIDATA_TEXTURE_EARTH,
	IMGUIDATA_TEXTURE_MOON,
	IMGUIDATA_TEXTURE_BACKGROUND,
	__IMGUIDATA_END__
};

class iGManager {
private:
	static iGManager main;

	iGManager();
	~iGManager();
	static void newFrame();
	static void drawFrame();

	static float* data;

public:

	static void initWin32(void* hWnd);
	static void initDX11(void* pDevice, void* pContext);
	static void closeWin32();
	static void closeDX11();

	static bool WndProcHandler(void* hWnd, unsigned int msg, unsigned int wParam, unsigned int lParam);
	static float* getData();
	static void render();
};