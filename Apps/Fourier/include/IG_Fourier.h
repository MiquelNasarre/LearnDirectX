#pragma once
#include "iGManager.h"

class IG_Fourier : public iGManager
{
private:
	static bool loadMenuOpen;
	static const char** figureNames;
	static unsigned int* figureSizes;

public:
	static void render();

	static void loadMenu();
};
