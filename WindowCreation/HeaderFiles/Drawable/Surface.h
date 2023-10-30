#pragma once
#include "Drawable.h"

#define error_epsilon	0.001f

enum SURFACE_TYPE {
	_EXPLICIT,
	_IMPLICIT,
	_PARAMETRIC,
	_RADIAL_ICOSPHERE,
	_RADIAL_SPHERICAL,
	_PARAMETRIC_SPHERICAL,
	_IMPLICIT_SPHERICAL
};

class Surface : public Drawable
{
public:
	//	Explicit & Radial spherical

	Surface(Graphics& gfx, SURFACE_TYPE Type, float F0(float, float), bool defaultValues = true, Vector2f minRect = {}, Vector2f maxRect = {}, UINT numX = 100u, UINT numY = 100u);
	Surface(Graphics& gfx, SURFACE_TYPE Type, float F0(float, float), std::string Texture0, std::string Texture1, bool defaultValues = true, Vector2f minRect = {}, Vector2f maxRect = {}, UINT numX = 100u, UINT numY = 100u);
	Surface(Graphics& gfx, SURFACE_TYPE Type, float F0(float, float), Texture texture0, Texture texture1, bool defaultValues = true, Vector2f minRect = {}, Vector2f maxRect = {}, UINT numX = 100u, UINT numY = 100u);

	//	Radial icosphere

	Surface(Graphics& gfx, SURFACE_TYPE Type, float rad(float, float), UINT ICOSPHERE_DEPTH);
	Surface(Graphics& gfx, SURFACE_TYPE Type, float rad(float, float), UINT ICOSPHERE_DEPTH, std::string Texture0, std::string Texture1);
	Surface(Graphics& gfx, SURFACE_TYPE Type, float rad(float, float), UINT ICOSPHERE_DEPTH, Texture texture0, Texture texture1);

	//	Parametric & Parametric spherical

	Surface(Graphics& gfx, SURFACE_TYPE Type, float F0(float, float), float F1(float, float), float F2(float, float), bool defaultValues = true, Vector2f minRect = {}, Vector2f maxRect = {}, UINT numU = 100u, UINT numV = 100u);
	Surface(Graphics& gfx, SURFACE_TYPE Type, float F0(float, float), float F1(float, float), float F2(float, float), std::string Texture0, std::string Texture1, bool defaultValues = true, Vector2f minRect = {}, Vector2f maxRect = {}, UINT numU = 100u, UINT numV = 100u);
	Surface(Graphics& gfx, SURFACE_TYPE Type, float F0(float, float), float F1(float, float), float F2(float, float), Texture texture0, Texture texture1, bool defaultValues = true, Vector2f minRect = {}, Vector2f maxRect = {}, UINT numU = 100u, UINT numV = 100u);

	//	Implicit & Implicit spherical

	Surface(Graphics& gfx, SURFACE_TYPE Type, float H(float, float, float));

	//	Public functions

	void updateRotation(Graphics& gfx, float rotationZ, float rotationX, Vector3f position = Vector3f());
	void updateTexture(Graphics& gfx, UINT id, std::string texture);
	void updateTexture(Graphics& gfx, UINT id, Texture texture);
	void updateTextures(Graphics& gfx, Texture texture0, Texture texture1);
	void updateTextures(Graphics& gfx, std::string texture0, std::string texture1);
	void updateLight(Graphics& gfx, UINT id, Vector2f intensity, Color color, Vector3f position);
	void updateLight(Graphics& gfx, UINT id, _float4vector intensity, _float4color color, _float4vector position);
	void clearLights(Graphics& gfx);

private:
	void generateExplicit(Graphics& gfx, float F(float, float), Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY, bool Textured = false);
	void generatePolarNormal(Graphics& gfx, float rad(float, float), Vector2f minRect, Vector2f maxRect, UINT numX, UINT numY, bool Textured = false);
	void generatePolarIcosphere(Graphics& gfx, float rad(float, float), UINT depth, bool Textured = false);
	void generateParametric(Graphics& gfx, float x(float, float), float y(float, float), float z(float, float), Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV, bool Textured = false);
	void generatePolarParametric(Graphics& gfx, float theta(float, float), float phi(float, float), float rad(float, float), Vector2f minRect, Vector2f maxRect, UINT numU, UINT numV, bool Textured = false);
	void generateImplicit(Graphics& gfx, float H(float, float, float));
	void generateImplicitPolar(Graphics& gfx, float H(float, float, float));

	void addDefaultBinds(Graphics& gfx);
	void addTexturedBinds(Graphics& gfx, std::string texture0, std::string texture1);
	void addTexturedBinds(Graphics& gfx, Texture texture0, Texture texture1);

	struct VSconstBuffer {
		_float4matrix rotattion;
		_float4vector traslation;
	}vscBuff;

	struct PSconstBuffer {
		struct {
			_float4vector intensity;
			_float4color  color;
			_float4vector position;
		}lightsource[8];
	}pscBuff;

	ConstantBuffer<VSconstBuffer>* pVSCB;
	ConstantBuffer<PSconstBuffer>* pPSCB;
	bool textured = false;

	Vector3f evalPolar(float r(float, float), float theta, float phi);

	struct Vertex {
		Vector3f vector;
		Vector3f norm;
	};

	struct TexVertex {
		Vector3f vector;
		Vector3f norm;
		Vector2f texCoor;
	};
};