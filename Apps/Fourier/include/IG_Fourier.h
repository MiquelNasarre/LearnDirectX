#pragma once
#include "iGManager.h"

class IG_Fourier : public iGManager
{
private:
	static bool loadMenuOpen;
	static bool colorPickerOpen;
	static char** interpolationsNames;
	static char** figureNames;
	static char** plotsNames;
	static int* figureSizes;
	static int light;

	static void saveLightState(int id);
	static void loadLightState(int id);
	static void doLightEditor(int& id);
	static void loadMenu();
	static void colorPicker();
	static void interpolationEditor();

	static void mainMenu();

public:
	static void render();

};
