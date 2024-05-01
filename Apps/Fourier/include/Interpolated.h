#pragma once
#include "FourierSurface.h"

class Interpolated : public Drawable
{
private:
	FourierSurface* Surface0;
	FourierSurface* Surface1;

	float T = 0.f;

	struct InterpolatedVertex
	{
		Vector3f vector;
		Vector3f dYlm0;
		Vector3f dYlm1;
		Color color;
	};

public:
	Interpolated() {}
	~Interpolated() = default;
	Interpolated(Graphics& gfx, FourierSurface* surface0, FourierSurface* surface1, float t = 0.f);

	void create(Graphics& gfx, FourierSurface* surface0, FourierSurface* surface1, float t = 0.f);
	void updateShape(Graphics& gfx, FourierSurface* surface0, FourierSurface* surface1, float t = 0.f);
	void updateInterpolation(Graphics& gfx, float t);

	void saveCoefficients(const char* filename);
	void saveInterpolation(const char* filename);
	void updateLight(Graphics& gfx, UINT id, Vector2f intensity, Color color, Vector3f position);
	void updateLight(Graphics& gfx, UINT id, _float4vector intensity, _float4color color, _float4vector position);
	void clearLights(Graphics& gfx);
	void updateTexture(Graphics& gfx, Color color, bool def = false, bool random = false);
	void updateRotation(Graphics& gfx, Vector3f axis, float angle, bool multiplicative = false);
	void updateRotation(Graphics& gfx, Quaternion rotation, bool multiplicative = false);
	void updateScreenPosition(Graphics& gfx, Vector2f screenDisplacement);
	void updateCurves(Graphics& gfx, float phi, float theta);
	Quaternion getRotation();

	void DrawCurves(Graphics& gfx);

private:

	struct PSconstBuffer {
		struct {
			_float4vector intensity = { 0.f,0.f,0.f,0.f };
			_float4color  color = { 1.f,1.f,1.f,1.f };
			_float4vector position = { 0.f,0.f,0.f,0.f };
		}lightsource[8];
	}pscBuff;

	struct VSconstBuffer {
		_float4vector tvalue = { 0.f, 0.f, 0.f, 0.f };
		Quaternion rotation = 1.f;
		_float4vector screenDisplacement = { 0.f, 0.f, 0.f, 0.f };
	}vscBuff;

	ConstantBuffer<VSconstBuffer>* pVSCB = NULL;
	ConstantBuffer<PSconstBuffer>* pPSCB = NULL;
};

class InterpolatedString
{
private:
	FourierSurface** Surfaces = NULL;
	Interpolated** Interpolations = NULL;
	unsigned int Nsurfaces = 0u;
	float T = 0.f;

public:
	InterpolatedString() {}
	~InterpolatedString();
	InterpolatedString(Graphics& gfx, FourierSurface** surfaces, unsigned int nsurfaces, float t = 0.f);

	void create(Graphics& gfx, FourierSurface** surfaces, unsigned int nsurfaces, float t = 0.f);
	void addSurface(Graphics& gfx, FourierSurface* surface);
	void deleteSurface(Graphics& gfx, unsigned int s);
	void updateInterpolation(Graphics& gfx, float t);

	void saveCoefficients(const char* filename);
	void saveInterpolation(const char* filename);
	void updateLight(Graphics& gfx, UINT id, Vector2f intensity, Color color, Vector3f position);
	void updateLight(Graphics& gfx, UINT id, _float4vector intensity, _float4color color, _float4vector position);
	void clearLights(Graphics& gfx);
	void updateTexture(Graphics& gfx, Color color, bool def = false, bool random = false);
	void updateRotation(Graphics& gfx, Vector3f axis, float angle, bool multiplicative = false);
	void updateRotation(Graphics& gfx, Quaternion rotation, bool multiplicative = false);
	void updateScreenPosition(Graphics& gfx, Vector2f screenDisplacement);
	void updateCurves(Graphics& gfx, float phi, float theta);
	Quaternion getRotation();

	void Draw(Graphics& gfx);
	void DrawCurves(Graphics& gfx);

};