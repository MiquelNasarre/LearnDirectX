#include "Fourier.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "IG_Fourier.h"

float IG_DATA::THETA = 0.f;
float IG_DATA::PHI   = 0.f;
float IG_DATA::SPEED = 0.f;
int IG_DATA::L = 0;
int IG_DATA::M = 0;
float IG_DATA::theta = 0.f;
float IG_DATA::phi = 0.f;
bool IG_DATA::UPDATE = false;
bool IG_DATA::UPDATE_CURVES = false;
bool IG_DATA::CURVES = false;

Fourier::Fourier()
	: window(640, 480, "Fourier", "", true),

	Yphi(window.graphics, Ylmphi, Vector2f(0.f, pi), 200),
	Ytheta(window.graphics, Ylmtheta, Vector2f(0.f, 2 * pi), 200),
	Ypos(window.graphics, 1.005 * Ylm(IG_DATA::phi,IG_DATA::theta), 8.f, Color(80,80,80))
{
	window.setFramerateLimit(60);

	harmonics.create(window.graphics, FourierSurface::FileManager::calculateCoefficients("Cube",25u), 676u);
	harmonics.updateLight(window.graphics, 0, { 600,320 }, Color::White, Vector3f(30, 10, 20));
	
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
	Quaternion rot = harmonics.getRotation();
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
		harmonics.updateRotation(window.graphics, Vector3f(), 0.f);
		returning = false;
		dangle = 0.f;
	}
}

void Fourier::strictReturn()
{
	dangle = 0;

	Quaternion rot = harmonics.getRotation();
	if (rot.r < 0)rot = -rot;

	float angle = 2 * acosf(rot.r);
	Vector3f axisRot = rot.getVector();

	axis = axisRot;
	dangle = -powf(angle, 0.65f) / 40.f;

	if (fabs(angle) < 0.01f)
	{
		harmonics.updateRotation(window.graphics, Vector3f(), 0.f);

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

		C.L = (unsigned int)IG_DATA::L;
		C.M = IG_DATA::M;

		harmonics.updateShape(window.graphics, &C, 1u);

		if (IG_DATA::CURVES)
		{
			Yphi.updateShape(window.graphics, Ylmphi, Vector2f(0.f, pi), 200, Color::White);
			Ytheta.updateShape(window.graphics, Ylmtheta, Vector2f(0.f, 2 * pi), 200, Color::White);
			Ypos.updatePosition(window.graphics, 1.005 * Ylm(IG_DATA::phi, IG_DATA::theta));
		}
	}

	if (IG_DATA::UPDATE_CURVES)
	{
		Yphi.updateShape(window.graphics, Ylmphi, Vector2f(0.f, pi), 200, Color::White);
		Ytheta.updateShape(window.graphics, Ylmtheta, Vector2f(0.f, 2 * pi), 200, Color::White);
		Ypos.updatePosition(window.graphics, 1.005 * Ylm(IG_DATA::phi, IG_DATA::theta));
	}
}

void Fourier::doFrame()
{
	eventManager();

	//	Update objects

	window.graphics.updatePerspective(observer, center, scale);

	harmonics.updateRotation(window.graphics, axis, dangle, true);
	Yphi.updateRotation(window.graphics, axis, dangle, true);
	Ytheta.updateRotation(window.graphics, axis, dangle, true);
	Ypos.updateRotation(window.graphics, axis, dangle, true);

	window.setTitle(harmonics.getRotation().str() + "  -  " + std::to_string(int(std::round(window.getFramerate()))) + "fps");

	//	Rendering

	window.graphics.clearBuffer(Color::Black);

	harmonics.Draw(window.graphics);

	if (IG_DATA::CURVES)
	{
		Yphi.Draw(window.graphics);
		Ytheta.Draw(window.graphics);
		Ypos.Draw(window.graphics);
	}

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

Vector3f YlmD(float phi, float theta)
{
	float cost = cosf(theta);
	float sint = sinf(theta);
	float cosp = cosf(phi);
	float sinp = sinf(phi);
	float Y = FourierSurface::Functions::Ylm(IG_DATA::L, IG_DATA::M, phi, theta);

	return Vector3f(Y * sint * cosp, Y * sint * sinp, Y * cost);
}

Vector3f Ylm(float phi, float theta)
{
	float cost = cosf(theta);
	float sint = sinf(theta);
	float cosp = cosf(phi);
	float sinp = sinf(phi);
	float Y = FourierSurface::Functions::Ylm(IG_DATA::L, IG_DATA::M, phi, theta);
	return Vector3f(Y * sint * cosp, Y * sint * sinp, Y * cost);
}

Vector3f Ylmphi(float theta)
{
	return 1.005 * Ylm(IG_DATA::phi,theta);
}

Vector3f Ylmtheta(float phi)
{
	return 1.005 * Ylm(phi,IG_DATA::theta);
}
