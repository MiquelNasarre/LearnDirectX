#pragma once
#include "Drawable.h"

class Background : public Drawable
{
public:
	Background(Graphics& gfx, std::string filename);
	void updateTexture(Graphics& gfx, std::string filename);
};