#pragma once
#include "Drawable.h"

class Curve : public Drawable
{
public:
	Curve(Graphics& gfx, Vector3f F(float), Vector2f rangeT, UINT Npoints, Color color = Color::White);
	Curve(Graphics& gfx, Vector3f F(float), Vector2f rangeT, UINT Npoints, std::vector<Color> colors);
	Curve(Graphics& gfx, float X(float), float Y(float), float Z(float), Vector2f rangeT, UINT Npoints, Color color = Color::White);
	Curve(Graphics& gfx, float X(float), float Y(float), float Z(float), Vector2f rangeT, UINT Npoints, std::vector<Color> colors);


	void updateRotation(Graphics& gfx, float rotationZ, float rotationX, Vector3f position = Vector3f());

private:

	void addDefaultBinds(Graphics& gfx);

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