#pragma once
#include "Drawable.h"

class Surface : public Drawable {
public:
	Surface(Graphics& gfx, float F(float, float), Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY);
private:
	
	struct Vertex {
		Vector3f vector;
		Vector3f norm;
	};
};