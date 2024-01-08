#include "QuaternionMotion.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "IG_QuaternionMotion.h"

DRAG_TYPE IG_DATA::TYPE = DYNAMIC_SPACE;

QuaternionMotion::QuaternionMotion()
	: window(640, 480, "QuaternionMotion", "", true),

	shape_0(window.graphics, SURFACE_SHAPE(_EXPLICIT_SPHERICAL, exampleRadius))
{
	window.setFramerateLimit(60);

	Vector3f vertexs[8] = {
	Vector3f(1.f, 1.f, 1.f),
	Vector3f(-1.f, 1.f, 1.f),
	Vector3f(-1.f,-1.f, 1.f),
	Vector3f(1.f,-1.f, 1.f),
	Vector3f(1.f, 1.f,-1.f),
	Vector3f(-1.f, 1.f,-1.f),
	Vector3f(-1.f,-1.f,-1.f),
	Vector3f(1.f,-1.f,-1.f),
	};

	Vector3i triangles[12] = {
		Vector3i(0, 1, 2),
		Vector3i(2, 3, 0),
		Vector3i(4, 5, 6),
		Vector3i(6, 7, 4),
		Vector3i(0, 1, 4),
		Vector3i(4, 5, 1),
		Vector3i(2, 3, 6),
		Vector3i(6, 7, 3),
		Vector3i(0, 3, 4),
		Vector3i(3, 4, 7),
		Vector3i(1, 2, 5),
		Vector3i(2, 5, 6),
	};

	shape_1.create(window.graphics, vertexs, triangles, 12);
	shape_2.create(window.graphics, vertexs, triangles, 12);
	
}

int QuaternionMotion::Run()
{
	while (window.processEvents())
		doFrame();
	return 0;
}

void QuaternionMotion::eventManager()
{
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
	{
		switch (IG_DATA::TYPE)
		{
		case RIGID_PLANE:
			drag_rigid_plane();
			break;
		case RIGID_SPACE:
			drag_rigid_space();
			break;
		case DYNAMIC_PLANE:
			drag_dynamic_plane();
			break;
		case DYNAMIC_SPACE:
			drag_dynamic_space();
			break;
		default:
			break;
		}
	}
	else
		scale *= powf(1.1f, Mouse::getWheel() / 120.f);

}

void QuaternionMotion::doFrame()
{
	eventManager();

	//	Update objects

	window.graphics.updatePerspective(observer, center, scale);

	shape_0.updateRotation(window.graphics, axis, dangle, true);
	shape_1.updateRotation(window.graphics, axis, dangle, true);
	shape_2.updateRotation(window.graphics, axis, dangle, true);

	window.setTitle(shape_0.getRotation().str() + "  -  " + std::to_string(int(std::round(window.getFramerate()))) + "fps");

	//	Rendering

	window.graphics.clearBuffer(Color::Black);

	shape_1.Draw(window.graphics);

	//	ImGui crap

	IG_QuaternionMotion::render();

	//	Push the frame to the scriin

	window.graphics.pushFrame();
}

//	Movement functions

void QuaternionMotion::magneticReturn()
{
	Quaternion rot = shape_0.getRotation();
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
		shape_0.updateRotation(window.graphics, Vector3f(), 0.f);
		shape_1.updateRotation(window.graphics, Vector3f(), 0.f);
		shape_2.updateRotation(window.graphics, Vector3f(), 0.f);
		returning = false;
		dangle = 0.f;
	}
}

void QuaternionMotion::strictReturn()
{
	dangle = 0;

	Quaternion rot = shape_0.getRotation();
	if (rot.r < 0)rot = -rot;

	float angle = 2 * acosf(rot.r);
	Vector3f axisRot = rot.getVector();

	axis = axisRot;
	dangle = -powf(angle, 0.65f) / 40.f;

	if (fabs(angle) < 0.01f)
	{
		shape_0.updateRotation(window.graphics, Vector3f(), 0.f);
		shape_1.updateRotation(window.graphics, Vector3f(), 0.f);
		shape_2.updateRotation(window.graphics, Vector3f(), 0.f);
		strict = false;
		dangle = 0.f;
	}
}

void QuaternionMotion::drag_rigid_plane()
{
	Vector3f obs = window.graphics.getObserver();
	Vector3f ex = -(obs * Vector3f(0.f, 0.f, 1.f)).normalize();
	Vector3f ey = ex * obs;

	Vector2i movement = Mouse::getPosition() - lastPos;
	lastPos = Mouse::getPosition();

	if (!movement)
	{
		if (axis != obs)
		{
			axis = obs;
			dangle = 0.f;
		}
		else
			dangle += Mouse::getWheel() / 18000.f;
	}
	else
	{
		axis = movement.y * ex - movement.x * ey;
		dangle = movement.abs() / scale;
	}
}

void QuaternionMotion::drag_rigid_space()
{
	Vector3f obs = window.graphics.getObserver();
	Vector3f ex = -(obs * Vector3f(0.f, 0.f, 1.f)).normalize();
	Vector3f ey = ex * obs;

	Vector3f lastMouse = (-obs + (ex * (lastPos.x - window.getDimensions().x / 2) + ey * (lastPos.y - window.getDimensions().y / 2)) / scale).normalize();
	lastPos = Mouse::getPosition();
	Vector3f newMouse = (-obs + (ex * (lastPos.x - window.getDimensions().x / 2) + ey * (lastPos.y - window.getDimensions().y / 2)) / scale).normalize();

	if (lastMouse == newMouse)
	{
		if (axis != newMouse)
		{
			axis = newMouse;
			dangle = 0.f;
		}
		else
			dangle += Mouse::getWheel() / 18000.f;
	}
	else
	{
		axis = lastMouse * newMouse;
		dangle = -acosf(lastMouse ^ newMouse);
	}
}

void QuaternionMotion::drag_dynamic_plane()
{
	Vector3f obs = window.graphics.getObserver();
	Vector3f ex = -(obs * Vector3f(0.f, 0.f, 1.f)).normalize();
	Vector3f ey = ex * obs;

	Vector2i movement = Mouse::getPosition() - lastPos;
	lastPos = Mouse::getPosition();

	Vector3f desiredAxis;
	float desiredAngle;

	if (!movement)
	{
		desiredAxis = obs;
		desiredAngle = Mouse::getWheel() / 18000.f;
	}
	else
	{
		desiredAxis = (movement.y * ex - movement.x * ey).normalize();
		desiredAngle = movement.abs() / scale;
	}

	Vector3f preAxis = axis;
	axis = (axis * dangle + desiredAxis * desiredAngle);
	if (axis)axis.normalize();
	else axis = desiredAxis;
	dangle *= axis ^ preAxis;
	dangle += (desiredAxis ^ axis) * (desiredAngle - dangle) / 20.f;

	if (dangle > 0.2f)dangle = 0.2f;
	if (dangle < -0.2f)dangle = -0.2f;
}

void QuaternionMotion::drag_dynamic_space()
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

	constexpr float s = 1.f / 2.f;

	Quaternion newRot = rotationQuaternion(newMouse, Mouse::getWheel() / 18000.f) * rotationQuaternion(axis0, dangle0) * rotationQuaternion(axis, (dangle * (1 - s + fabs(axis ^ newMouse) * s)));
	
	dangle = 2 * acosf(newRot.r);
	axis = newRot.getVector();
	if (!axis)axis = newMouse;
	axis.normalize();
}

//	Functions

float exampleRadius(float theta, float phi)
{
	return 1.f + (sinf(theta) * sinf(theta) * cosf(phi) + cosf(phi) * cosf(phi) * sinf(phi)) * sinf(5 * theta) * cosf(3 * phi) / 2.f;
}
