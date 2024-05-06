#pragma once
#include "iGManager.h"

class IG_Fourier : public iGManager
{
private:
	static bool loadMenuOpen;
	static bool saveMenuOpen;
	static bool addName;
	static int iMenu;
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
	static void saveMenu();
	static void colorPicker();
	static void errorsWindow();
	static void interpolationEditor();

	static void mainMenu();

public:
	static void render();

};
