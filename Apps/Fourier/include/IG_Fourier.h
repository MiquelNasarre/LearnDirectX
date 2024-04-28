#pragma once
#include "iGManager.h"

class IG_Fourier : public iGManager
{
private:
	static bool loadMenuOpen;
	static bool colorPickerOpen;
	static const char** figureNames;
	static unsigned int* figureSizes;

	static void saveLightState(int id);
	static void loadLightState(int id);
	static void doLightEditor(int& id);
	static void loadMenu();
	static void colorPicker();

public:
	static void render();

};
