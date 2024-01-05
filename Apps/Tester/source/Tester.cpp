#include "Tester.h"
#include "IG_Tester.h"
#include "Keyboard.h"
#include "Mouse.h"

float IG_DATA::THETA = pi / 2.f;
float IG_DATA::PHI = 0.f;

Tester::Tester()
	:window(640, 480, "Plotter"),
	surf(window.graphics, { _EXPLICIT_SPHERICAL, weirdRadius }),
	light(window.graphics, Color::Black, { 3.f , 0.f , 2.f }, 1.f),
	curve(window.graphics, curveF, { 0.f ,2 * pi }, 1000, { Color::Red / 3, Color::Yellow / 3, Color::Green / 3, Color::Cyan / 3, Color::Blue / 3, Color::Purple / 3, Color::Red / 3 }),
	Klein(window.graphics, { _PARAMETRIC, KleinBottle, false, Vector2f(0, 0), Vector2f(pi, 2 * pi) }),
	point(window.graphics, { 2.f,0.f,0.f }, 8.f),
	impl(window.graphics, { _IMPLICIT, sphere }),
	test(window.graphics, SURFACE_SHAPE(_EXPLICIT, returnX)),
	shape(_EXPLICIT, SincFunction, var, false, { -5.f,-5.f }, { 5.f,5.f })
{
	window.setFramerateLimit(60);
	srand(143452);
	surf.clearLights(window.graphics);
	timer.reset();

	Vector3f vertexs[8] = {
		Vector3f( 1.f, 1.f, 1.f),
		Vector3f(-1.f, 1.f, 1.f),
		Vector3f(-1.f,-1.f, 1.f),
		Vector3f( 1.f,-1.f, 1.f),
		Vector3f( 1.f, 1.f,-1.f),
		Vector3f(-1.f, 1.f,-1.f),
		Vector3f(-1.f,-1.f,-1.f),
		Vector3f( 1.f,-1.f,-1.f),
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

	poli.create(window.graphics, vertexs, triangles, 12);
}

int Tester::Run()
{
	while (window.processEvents())
		doFrame();
	return 0;
}

void Tester::eventManager()
{
	if (Keyboard::isKeyPressed('W'))
		center.y += 0.02f;
	if (Keyboard::isKeyPressed('S'))
		center.y -= 0.02f;
	if (Keyboard::isKeyPressed('A'))
		center.x -= 0.02f;
	if (Keyboard::isKeyPressed('D'))
		center.x += 0.02f;
	if (Keyboard::isKeyPressed('R'))
		center.z += 0.02f;
	if (Keyboard::isKeyPressed('F'))
		center.z -= 0.02f;

	if (Mouse::isButtonPressed(Mouse::Left) && !dragging) {
		dragging = true;
		initialDrag = Mouse::getPosition();
		lastPos = Mouse::getPosition();
		dangle = 0.f;
		initialDragAngles = { theta, phi };
	}
	if (dragging && !Mouse::isButtonPressed(Mouse::Left))
		dragging = false;

	if (dragging) {
		Vector2i movement = Mouse::getPosition() - initialDrag;

		float temp = initialDragAngles.x - 2.f * (float)movement.x / scale;
		speed = -10.f * (temp - theta);
		theta = temp;
		phi = initialDragAngles.y + 2.f * (float)movement.y / scale;

		if (phi > pi / 2.f) {
			phi = pi / 2.f - 0.0001f;
			initialDrag.y = Mouse::getPosition().y - int((pi / 2.f - initialDragAngles.y) * scale / 2.f);
		}
		if (phi < -pi / 2.f) {
			phi = -pi / 2.f + 0.0001f;
			initialDrag.y = Mouse::getPosition().y - int((-pi / 2.f - initialDragAngles.y) * scale / 2.f);
		}

		movement = Mouse::getPosition() - lastPos;
		lastPos = Mouse::getPosition();

		if (!movement)
		{
			if (axis != window.graphics.getObserver())
			{
				axis = window.graphics.getObserver();
				int wheel = Mouse::getWheel();
				dangle = 0.f;
			}

			else
				dangle += Mouse::getWheel() / 18000.f;
		}
		else
		{
			Vector3f obs = window.graphics.getObserver();
			Vector3f ex = -(obs * Vector3f(0.f, 0.f, 1.f)).normalize();
			Vector3f ey = (ex * obs).normalize();
			axis = movement.y * ex - movement.x * ey;
			dangle = movement.abs() / scale;
		}



	}
	else {
		scale *= powf(1.1f, Mouse::getWheel() / 120.f);

		if (speed > 1.f) {
			speed *= 0.99f;
			if (speed < 1.f)
				speed = 1.f;
		}
		if (speed < -1.f) {
			speed *= 0.99f;
			if (speed > -1.f)
				speed = -1.f;
		}
		theta += -speed / 10.f;
	}


	if (IG_DATA::PHI > 3.1415f / 2.f)
		IG_DATA::PHI = 3.1415f / 2.f;

	if (IG_DATA::PHI < -3.1415f / 2.f)
		IG_DATA::PHI = -3.1415f / 2.f;

}

void Tester::doFrame()
{
	eventManager();

	window.setTitle("Plotter - " + std::to_string(int(window.getFramerate())) + "fps");
	window.graphics.clearBuffer(Color::Black);

	window.graphics.updatePerspective({ -cosf(IG_DATA::PHI) * cosf(IG_DATA::THETA), -cosf(IG_DATA::PHI) * sinf(IG_DATA::THETA), -sinf(IG_DATA::PHI) }, center, scale);

	static int count = 0;
	if (!(++count % 100)) {
		Color col(rand() % 255, rand() % 255, rand() % 255);
		float rad = rand() / 32728.f;
		light.updateColor(window.graphics, col);
		light.updateRadius(window.graphics, rad);
		surf.updateLight(window.graphics, 0, { 3 * rad, rad }, col, { 3.f , 0.f , 2.f });
	}

	//surf.updateRotation(window.graphics, -phi, 0.f, -theta);
	//curve.updateRotation(window.graphics, -phi, 0.f, -theta);
	//Klein.updateRotation(window.graphics, -phi, 0.f, -theta);
	//impl.updateRotation(window.graphics, -phi, 0.f, -theta);
	//test.updateRotation(window.graphics, -phi, 0.f, -theta);
	//poli.updateRotation(window.graphics, -phi, 0.f, -theta);

	surf.updateRotation(window.graphics, axis, dangle, true);
	curve.updateRotation(window.graphics, axis, dangle, true);
	Klein.updateRotation(window.graphics, axis, dangle, true);
	impl.updateRotation(window.graphics, axis, dangle, true);
	test.updateRotation(window.graphics, axis, dangle, true);
	poli.updateRotation(window.graphics, axis, dangle, true);

	test.updateShape(window.graphics, shape);
	test.Draw(window.graphics);
	//surf.Draw(window.graphics);
	//light.Draw(window.graphics);
	//curve.Draw(window.graphics);
	//Klein.Draw(window.graphics);
	//point.Draw(window.graphics);
	//impl.Draw(window.graphics);
	//poli.Draw(window.graphics);

	var = 3.f * cosf(timer.check());
	shape.param = var;




	IG_Tester::render();
	window.graphics.pushFrame();
}

//	Surface functions

Vector3f curveF(float t)
{
	Vector3f axis = { 5.f * cosf(t) , 5.f * sinf(t) , 0.f };
	Vector3f V = (axis * Vector3f(0, 0, 1)).normalize();
	Vector3f X = -axis / axis.abs();
	Vector3f Y = X * V;
	return axis + X * cosf(20 * t) + Y * sinf(20 * t);
}

float curveX(float t)
{
	return cosf(t);
}

float curveY(float t)
{
	return sinf(t);
}

Vector3f KleinBottle(float u, float v)
{
	float c_u = cosf(u);
	float s_u = sinf(u);
	float c_v = cosf(v);
	float s_v = sinf(v);

	float x = -2.f / 15.f * c_u * (3 * c_v - 30 * s_u + 90 * powf(c_u, 4.f) * s_u - 60 * powf(c_u, 6.f) * s_u + 5 * c_u * c_v * s_u);
	float y = -1.f / 15.f * s_u * (3 * c_v - 3 * c_u * c_u * c_v - 48 * powf(c_u, 4.f) * c_v + 48 * powf(c_u, 6.f) * c_v - 60 * s_u + 5 * c_u * c_v * s_u - 5 * powf(c_u, 3) * c_v * s_u - 80 * powf(c_u, 7.f) * c_v * s_u) - 1;
	float z = 2.f / 15.f * (3 + 5 * c_u * s_u) * s_v;
	return Vector3f(x, y, z);
}

float curveZ(float t)
{
	return t / 5.f;
}

float SincFunction(float x, float y, const float& n)
{
	if (x == 0.f && y == 0.f)
		return n;
	return sinf(n * sqrtf(x * x + y * y)) / sqrtf(x * x + y * y);
}

float constantRadius(float, float)
{
	return 1.f;
}

float constantRadius03(float, float)
{
	return 0.3f;
}

float weirdRadius(float theta, float phi)
{
	return 1.f + (sinf(theta) * sinf(theta) * cosf(phi) + cosf(phi) * cosf(phi) * sinf(phi)) * sinf(5 * theta) * cosf(3 * phi) / 2.f;
}

float returnX(float x, float)
{
	return x;
}

float returnY(float, float y)
{
	return y;
}

float sphere(float x, float y, float z)
{
	//return x * x + y * y - z * z - 1;
	//return z;
	//return (z + 1) * (z + 1) * (z + 1) - x * x * sqrt(z + 1) - y * y;
	return y * y + z * z - expf(x) + 1;
	return x * x + y * y + z * z - 1;
}

float weirdRadiusDynamic(float theta, float phi, const float& t)
{
	return 1.f + cosf(t * 3.f) * (sinf(theta + sinf(t)) * sinf(theta) * cosf(phi) + cosf(phi) * cosf(phi + sinf(t)) * sinf(phi)) * sinf(5 * theta) * cosf(3 * phi + sinf(t)) / 2.f;
}
