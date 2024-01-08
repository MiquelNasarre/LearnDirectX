#pragma once
#include "Drawable.h"

class Curve : public Drawable
{
public:
	Curve(Graphics& gfx, Vector3f F(float), Vector2f rangeT, UINT Npoints, Color color = Color::White);
	Curve(Graphics& gfx, Vector3f F(float), Vector2f rangeT, UINT Npoints, std::vector<Color> colors);
	Curve(Graphics& gfx, float X(float), float Y(float), float Z(float), Vector2f rangeT, UINT Npoints, Color color = Color::White);
	Curve(Graphics& gfx, float X(float), float Y(float), float Z(float), Vector2f rangeT, UINT Npoints, std::vector<Color> colors);

	template<typename C>
	Curve(Graphics& gfx, Vector3f F(float, const C&), const C& param, Vector2f rangeT, UINT Npoints, Color color = Color::White);
	template<typename C>
	Curve(Graphics& gfx, Vector3f F(float, const C&), const C& param, Vector2f rangeT, UINT Npoints, std::vector<Color> colors);
	template<typename C>
	Curve(Graphics& gfx, float X(float, const C&), float Y(float, const C&), float Z(float, const C&), const C& param, Vector2f rangeT, UINT Npoints, Color color = Color::White);
	template<typename C>
	Curve(Graphics& gfx, float X(float, const C&), float Y(float, const C&), float Z(float, const C&), const C& param, Vector2f rangeT, UINT Npoints, std::vector<Color> colors);

	void updateRotation(Graphics& gfx, float rotationX, float rotationY, float rotationZ);
	void updateRotation(Graphics& gfx, Vector3f axis, float angle, bool multiplicative = false);
	void updatePosition(Graphics& gfx, Vector3f position, bool additive = false);

	Quaternion getRotation();
	Vector3f getPosition();

private:

	void addDefaultBinds(Graphics& gfx);

	struct Vertex {
		_float4vector position;
		_float4color color;
	};

	struct VSconstBuffer {
		_float4vector translation = { 0.f, 0.f, 0.f, 0.f };
		Quaternion rotation = 1.f;
	}vscBuff;

	ConstantBuffer<VSconstBuffer>* pVSCB;
};

template<typename C>
Curve::Curve(Graphics& gfx, Vector3f F(float, const C&), const C& param, Vector2f rangeT, UINT Npoints, Color color)
{
	std::vector<Vertex> vertexs;

	for (UINT i = 0; i <= Npoints; i++)
		vertexs.push_back(Vertex(F(rangeT.x + float(i) / Npoints * (rangeT.y - rangeT.x), param).getVector4(), color.getColor4()));

	std::vector<unsigned short> indexs;

	for (UINT i = 0; i <= Npoints; i++)
		indexs.push_back(i);

	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));

	addDefaultBinds(gfx);
}

template<typename C>
Curve::Curve(Graphics& gfx, Vector3f F(float, const C&), const C& param, Vector2f rangeT, UINT Npoints, std::vector<Color> colors)
{
	if (!colors.size())
		throw std::exception("The color vector for a curve must have at least one color!!");

	std::vector<Vertex> vertexs;

	for (UINT i = 0; i <= Npoints; i++) {
		float c = (colors.size() - 1) * float(i) / Npoints;
		UINT C0 = UINT(c);
		UINT C1 = (C0 + 1) % colors.size();
		c -= C0;

		vertexs.push_back(Vertex(
			F(rangeT.x + float(i) / Npoints * (rangeT.y - rangeT.x), param).getVector4(),
			(colors[C0] * (1 - c) + colors[C1] * c).getColor4()
		));
	}


	std::vector<unsigned short> indexs;

	for (UINT i = 0; i <= Npoints; i++)
		indexs.push_back(i);

	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));

	addDefaultBinds(gfx);
}

template<typename C>
Curve::Curve(Graphics& gfx, float X(float, const C&), float Y(float, const C&), float Z(float, const C&), const C& param, Vector2f rangeT, UINT Npoints, Color color)
{
	std::vector<Vertex> vertexs;

	for (UINT i = 0; i <= Npoints; i++)
		vertexs.push_back(Vertex(
			Vector3f(
				X(rangeT.x + float(i) / Npoints * (rangeT.y - rangeT.x), param),
				Y(rangeT.x + float(i) / Npoints * (rangeT.y - rangeT.x), param),
				Z(rangeT.x + float(i) / Npoints * (rangeT.y - rangeT.x), param)
			).getVector4(),

			color.getColor4()
		));

	std::vector<unsigned short> indexs;

	for (UINT i = 0; i <= Npoints; i++)
		indexs.push_back(i);

	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));

	addDefaultBinds(gfx);
}

template<typename C>
Curve::Curve(Graphics& gfx, float X(float, const C&), float Y(float, const C&), float Z(float, const C&), const C& param, Vector2f rangeT, UINT Npoints, std::vector<Color> colors)
{
	if (!colors.size())
		throw std::exception("The color vector for a curve must have at least one color!!");

	std::vector<Vertex> vertexs;

	for (UINT i = 0; i <= Npoints; i++) {
		float c = (colors.size() - 1) * float(i) / Npoints;
		UINT C0 = UINT(c);
		UINT C1 = (C0 + 1) % colors.size();
		c -= C0;

		vertexs.push_back(Vertex(
			Vector3f(
				X(rangeT.x + float(i) / Npoints * (rangeT.y - rangeT.x), param),
				Y(rangeT.x + float(i) / Npoints * (rangeT.y - rangeT.x), param),
				Z(rangeT.x + float(i) / Npoints * (rangeT.y - rangeT.x), param)
			).getVector4(),

			(colors[C0] * (1 - c) + colors[C1] * c).getColor4()
		));
	}

	std::vector<unsigned short> indexs;

	for (UINT i = 0; i <= Npoints; i++)
		indexs.push_back(i);

	AddBind(std::make_unique<VertexBuffer>(gfx, vertexs));
	AddBind(std::make_unique<IndexBuffer>(gfx, indexs));

	addDefaultBinds(gfx);
}