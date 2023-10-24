#pragma once
#include "Drawable.h"


#define error_epsilon	0.001f
#define _FUNCTION_EXPLICIT			Surface::Type0()
#define _FUNCTION_RADIUS_LAT_LONG	Surface::Type1()
#define _FUNCTION_RADIUS_ICOSPHERE	Surface::Type2()
#define _FUNCTION_PARAMETRIC		Surface::Type3()
#define _FUNCTION_POLAR_PARAMETRIC	Surface::Type4()
#define _FUNCTION_IMPLICIT			Surface::Type5()
#define _FUNCTION_POLAR_IMPLICIT	Surface::Type6()

class Surface : public Drawable {
public:
	struct Type0 {};
	struct Type1 {};
	struct Type2 {};
	struct Type3 {};
	struct Type4 {};
	struct Type5 {};
	struct Type6 {};

	Surface(Graphics& gfx, Type0 TYPE_FLAG, float F(float, float), Vector2f minRect = { -1.f, -1.f }, Vector2f maxRect = { 1.f, 1.f }, UINT numX = 100u, UINT numY = 100u);
	Surface(Graphics& gfx, Type1 TYPE_FLAG, float rad(float, float), UINT numX = 100u, UINT numY = 100u, Vector2f minRect = { 0.f, -pi / 2.f }, Vector2f maxRect = { 2.f * pi, pi / 2.f });
	Surface(Graphics& gfx, Type2 TYPE_FLAG, float rad(float, float), UINT ICOSPHERE_DEPTH);
	Surface(Graphics& gfx, Type3 TYPE_FLAG, float x(float, float), float y(float, float), float z(float, float), Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV);
	Surface(Graphics& gfx, Type4 TYPE_FLAG, float theta(float, float), float phi(float, float), float rad(float, float), Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV);

	void updateRotation(Graphics& gfx, float rotationZ, float rotationX);
private:
	void generateExplicit(Graphics& gfx, float F(float, float), Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY);
	void generatePolarNormal(Graphics& gfx, float rad(float, float), Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY);
	void generatePolarIcosphere(Graphics& gfx, float rad(float, float), UINT depth);
	void generateParametric(Graphics& gfx, float x(float, float), float y(float, float), float z(float, float), Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV);
	void generatePolarParametric(Graphics& gfx, float theta(float, float), float phi(float, float), float rad(float, float), Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV);
	void addDefaultBinds(Graphics& gfx);

	ConstantBuffer<_float4matrix>* pVSCB;

	Vector3f evalPolar(float r(float, float), float theta, float phi);

	struct Vertex {
		Vector3f vector;
		Vector3f norm;
	};
};