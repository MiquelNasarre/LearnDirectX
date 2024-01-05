#pragma once
#include "Drawable.h"

class Triangle : public Drawable
{
public:

	Triangle() {}
	Triangle(Graphics& gfx, Vector3f* vertexs, Vector3i* triangles, UINT numT, Color* colors = NULL, bool vertexColor = false);

	void create(Graphics& gfx, Vector3f* vertexs, Vector3i* triangles, UINT numT, Color* colors = NULL, bool vertexColor = false);

	void updateShape(Graphics& gfx, Vector3f* vertexs, Vector3i* triangles, UINT numT, Color* colors = NULL, bool vertexColor = false);
	void updateRotation(Graphics& gfx, float rotationX, float rotationY, float rotationZ);
	void updateRotation(Graphics& gfx, Vector3f axis, float angle, bool multiplicative = false);
	void updatePosition(Graphics& gfx, Vector3f position, bool additive = false);
	void updateLight(Graphics& gfx, UINT id, Vector2f intensity, Color color, Vector3f position);
	void updateLight(Graphics& gfx, UINT id, _float4vector intensity, _float4color color, _float4vector position);
	void clearLights(Graphics& gfx);

	void Draw(Graphics& gfx) override;
	
private:
	struct VSconstBuffer {
		_float4vector translation = { 0.f, 0.f, 0.f, 0.f };
		Quaternion rotation = 1.f;
	}vscBuff;

	struct PSconstBuffer {
		struct {
			_float4vector intensity = { 0.f,0.f,0.f,0.f };
			_float4color  color		= { 1.f,1.f,1.f,1.f };
			_float4vector position	= { 0.f,0.f,0.f,0.f };
		}lightsource[8];
	}pscBuff;

	ConstantBuffer<VSconstBuffer>* pVSCB = NULL;
	ConstantBuffer<PSconstBuffer>* pPSCB = NULL;

};