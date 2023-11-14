#pragma once
#include "iGManager.h"

class IG_Navigation : public iGManager
{
	static int waypointSelected;
	static int routeSelected;

	struct values
	{
		static float radius;
		static float latitude;
		static float Longitude;

	};

	static void renderWaypointSettings(int& w);
	static void renderRouteSettings(int& r);
public:
	static void render();
};
