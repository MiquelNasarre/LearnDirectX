#pragma once
#include "Drawable.h"

//enum CURVE_TYPE {
//	_PARAMETRIC,
//	_PARAMETRIC_SPHERICAL,
//};

class Curve : public Drawable
{
public:
	Curve(Graphics& gfx, Vector3f F(float), Vector2f rangeT, UINT Npoints, Color color = Color::White);

	void updateRotation(Graphics& gfx, float rotationZ, float rotationX, Vector3f position = Vector3f());

private:

	struct Vertex {
		_float4vector position;
		_float4color color;
	};

	struct VSconstBuffer {
		_float4matrix rotattion;
		_float4vector traslation;
	}vscBuff;

	ConstantBuffer<VSconstBuffer>* pVSCB;
};