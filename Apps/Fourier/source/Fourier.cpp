#include "Fourier.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "IG_Fourier.h"
#include "Functions.h"

float IG_DATA::THETA = 0.f;
float IG_DATA::PHI   = 0.f;
float IG_DATA::SPEED = 0.f;
int IG_DATA::L = 0;
int IG_DATA::M = 0;
bool IG_DATA::UPDATE = false;

Fourier::Fourier()
	: window(640, 480, "Fourier", "", true),

	positive(window.graphics, SURFACE_SHAPE(_PARAMETRIC, Ylm, false, { 0.f,0.f }, { 2 * pi,pi }, 200u, 200u), SURFACE_COLORING(Color::Blue).ptr()),
	negative(window.graphics, SURFACE_SHAPE(_PARAMETRIC, Ylm, false, { 0.f,pi }, { 2 * pi,2 * pi }, 200u, 200u), SURFACE_COLORING(Color::Yellow).ptr())
{
	window.setFramerateLimit(60);

	positive.clearLights(window.graphics);
	positive.updateLight(window.graphics, 0, { 100,50 }, Color::White, Vector3f(10, 0, 10));
	negative.clearLights(window.graphics);
	negative.updateLight(window.graphics, 0, { 100,50 }, Color::White, Vector3f(10, 0, 10));

}

int Fourier::Run()
{
	while (window.processEvents())
		doFrame();
	return 0;
}

void Fourier::drag_dynamic_space()
{
	Vector3f obs = window.graphics.getObserver();
	Vector3f ex = -(obs * Vector3f(0.f, 0.f, 1.f)).normalize();
	Vector3f ey = ex * obs;

	Vector3f lastMouse = (-obs + (ex * (lastPos.x - window.getDimensions().x / 2) + ey * (lastPos.y - window.getDimensions().y / 2)) / scale).normalize();
	lastPos = Mouse::getPosition();
	Vector3f newMouse = (-obs + (ex * (lastPos.x - window.getDimensions().x / 2) + ey * (lastPos.y - window.getDimensions().y / 2)) / scale).normalize();

	Vector3f axis0 = lastMouse * newMouse;
	if (!axis0) axis0 = newMouse;
	axis0.normalize();
	float dangle0 = -acosf(lastMouse ^ newMouse);
	if ((lastMouse ^ newMouse) > 1.f)dangle0 = 0.f;
	if (lastMouse == newMouse) dangle0 = 0.f;

	constexpr float s = 1.f / 1.5f;

	Quaternion newRot = rotationQuaternion(newMouse, Mouse::getWheel() / 18000.f) * rotationQuaternion(axis0, dangle0) * rotationQuaternion(axis, (dangle * (1 - s + fabs(axis ^ newMouse) * s)));

	dangle = 2 * acosf(newRot.r);
	axis = newRot.getVector();
	if (!axis)axis = newMouse;
	axis.normalize();
}

void Fourier::magneticReturn()
{
	Quaternion rot = positive.getRotation();
	if (rot.r < 0)rot = -rot;
	constexpr float pull = 0.03f;

	float angle = 2 * acosf(rot.r);
	Vector3f axisRot = rot.getVector();

	Quaternion newRot;
	if (angle < pull)
		newRot = rotationQuaternion(axisRot, -pull / 40.f) * rotationQuaternion(axis, dangle);
	else
		newRot = rotationQuaternion(axisRot, -angle / 40.f) * rotationQuaternion(axis, dangle);

	axis = newRot.getVector();
	dangle = 2 * acos(newRot.r);

	dangle *= 0.95f;

	if (fabs(angle) < 0.01f && fabs(dangle) < 0.005f)
	{
		positive.updateRotation(window.graphics, Vector3f(), 0.f);
		negative.updateRotation(window.graphics, Vector3f(), 0.f);
		returning = false;
		dangle = 0.f;
	}
}

void Fourier::strictReturn()
{
	dangle = 0;

	Quaternion rot = positive.getRotation();
	if (rot.r < 0)rot = -rot;

	float angle = 2 * acosf(rot.r);
	Vector3f axisRot = rot.getVector();

	axis = axisRot;
	dangle = -powf(angle, 0.65f) / 40.f;

	if (fabs(angle) < 0.01f)
	{
		positive.updateRotation(window.graphics, Vector3f(), 0.f);
		positive.updateRotation(window.graphics, Vector3f(), 0.f);

		strict = false;
		dangle = 0.f;
	}
}

void Fourier::eventManager()
{
	//	Keyboard and Mouse events

		// returning motion

	if (Keyboard::isKeyPressed('R'))
		magneticReturn();

	if (Keyboard::isKeyPressed('T'))
		strictReturn();

	if (Keyboard::isKeyPressed('S'))
		dangle = 0.f;


	if (Mouse::isButtonPressed(Mouse::Left) && !dragging)
	{
		dragging = true;
		lastPos = Mouse::getPosition();
	}

	if (dragging && !Mouse::isButtonPressed(Mouse::Left))
		dragging = false;

	if (dragging)
			drag_dynamic_space();

	else
		scale *= powf(1.1f, Mouse::getWheel() / 120.f);

	//	Calculate observer vector

	observer = {
			-cosf(IG_DATA::PHI) * cosf(IG_DATA::THETA) ,
			-cosf(IG_DATA::PHI) * sinf(IG_DATA::THETA) ,
			-sinf(IG_DATA::PHI)
	};

	if (IG_DATA::UPDATE)
	{
		IG_DATA::UPDATE = false;
		positive.updateShape(window.graphics, SURFACE_SHAPE(_PARAMETRIC, Ylm, false, { 0.f,0.f }, { 2 * pi,pi }, 200u, 200u));
		negative.updateShape(window.graphics, SURFACE_SHAPE(_PARAMETRIC, Ylm, false, { 0.f,pi }, { 2 * pi,2 * pi }, 200u, 200u));
	}
}

void Fourier::doFrame()
{
	eventManager();

	//	Update objects

	window.graphics.updatePerspective(observer, center, scale);

	positive.updateRotation(window.graphics, axis, dangle, true);
	negative.updateRotation(window.graphics, axis, dangle, true);

	window.setTitle(positive.getRotation().str() + "  -  " + std::to_string(int(std::round(window.getFramerate()))) + "fps");

	//	Rendering

	window.graphics.clearBuffer(Color::Black);

	positive.Draw(window.graphics);
	if (IG_DATA::L % 2) negative.Draw(window.graphics);


	//	ImGui crap

	IG_Fourier::render();

	//	Push the frame to the scriin

	window.graphics.pushFrame();
}

//	Functions

float exampleRadius(float theta, float phi)
{
	return 1.f + (sinf(theta) * sinf(theta) * cosf(phi) + cosf(phi) * cosf(phi) * sinf(phi)) * sinf(5 * theta) * cosf(3 * phi) / 2.f;
}

Vector3f Ylm(float phi, float theta)
{
	int neg = 1;
	if (theta > pi) {
		neg = -1;
		theta -= pi;
	}
	return neg * Functions::Yfunc(IG_DATA::L, IG_DATA::M, phi, theta) * Vector3f(sinf(theta) * cosf(phi), sinf(theta) * sinf(phi), cosf(theta));
}
