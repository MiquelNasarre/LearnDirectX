#pragma once
#include "iGManager.h"

class IG_EarthDemo : public iGManager
{
private:
	static void saveLightState(int id);
	static void loadLightState(int id);
	static void doLightEditor(int& id);

public:
	static void render();
};