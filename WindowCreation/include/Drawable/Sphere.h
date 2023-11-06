#pragma once
#include "Drawable.h"

class Sphere : public Drawable
{
public:
	Sphere(Graphics& gfx, Color color, UINT icosphere_degree);
	void Update(Graphics& gfx, Vector3f position);
private:
	struct VSconstBuffer {
		_float4vector translation;
	}vscBuff;

	struct Vertex {
		Vector3f vector;
		Color color;
	};

	struct Model {
		std::vector<Vertex> vertexs;
		std::vector<unsigned short> indexs;
	};

	Model generateIcosphere(UINT depth, Color color);
	ConstantBuffer<VSconstBuffer>* pVSCB;
	ConstantBuffer<_float4vector>* pPSCB;
};