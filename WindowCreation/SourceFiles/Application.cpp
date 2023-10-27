#include "Application.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "iGManager.h"

App::App()
	: window(640, 480, L"Hello World"),
	Earth(window.graphics, _RADIAL_SPHERICAL, weirdRadius, "Resources/EarthTexture.jpg", "Resources/nightEarthTexture.jpg"),
	Moon(window.graphics, _RADIAL_SPHERICAL, constantRadius03, "Resources/MoonTexture.jpg", ""),
	back(window.graphics, "Resources/hpfNightSky.jpg", true, PT_AZIMUTHAL)
{
	imGuiData = iGManager::getData();
	imGuiData[IMGUIDATA_THETA] = pi / 2.f;
	imGuiData[IMGUIDATA_FOV] = 1.f;

	window.setFramerateLimit(60);
	timer.reset();
}

int App::Run()
{
	while (window.processEvents())
		doFrame();
	return 0;
}

void App::eventManager()
{
	//	Keyboard and Mouse events

	scale *= powf(1.1f, Mouse::getWheel() / 120.f);

	if (Keyboard::isKeyPressed('W'))
		center.y += 0.01f;
	if (Keyboard::isKeyPressed('S'))
		center.y -= 0.01f;
	if (Keyboard::isKeyPressed('A'))
		center.x -= 0.01f;
	if (Keyboard::isKeyPressed('D'))
		center.x += 0.01f;
	if (Keyboard::isKeyPressed('R'))
		center.z += 0.01f;
	if (Keyboard::isKeyPressed('F'))
		center.z -= 0.01f;

	if (Mouse::isButtonPressed(Mouse::Left) && !dragging) {
		dragging = true;
		initialDrag = Mouse::getPosition();
		initialDragAngles = { imGuiData[IMGUIDATA_EARTH_THETA],imGuiData[IMGUIDATA_EARTH_PHI] };
	}
	if (dragging && !Mouse::isButtonPressed(Mouse::Left))
		dragging = false;

	if (dragging) {
		Vector2i movement = Mouse::getPosition() - initialDrag;
		float temp = initialDragAngles.x - 2.f * (float)movement.x / scale;
		imGuiData[IMGUIDATA_SPEED] = -10.f * (temp - imGuiData[IMGUIDATA_EARTH_THETA]);
		imGuiData[IMGUIDATA_EARTH_THETA] = temp;
		imGuiData[IMGUIDATA_EARTH_PHI] = initialDragAngles.y + 2.f * (float)movement.y / scale;
		if (imGuiData[IMGUIDATA_EARTH_PHI] > pi / 2.f) {
			imGuiData[IMGUIDATA_EARTH_PHI] = pi / 2.f - 0.0001f;
			initialDrag.y = Mouse::getPosition().y - int((pi / 2.f - initialDragAngles.y) * scale / 2.f);
		}
		if (imGuiData[IMGUIDATA_EARTH_PHI] < -pi / 2.f) {
			imGuiData[IMGUIDATA_EARTH_PHI] = -pi / 2.f + 0.0001f;
			initialDrag.y = Mouse::getPosition().y - int((-pi / 2.f - initialDragAngles.y) * scale / 2.f);
		}
	}
	else {
		if (imGuiData[IMGUIDATA_SPEED] > 1.f) {
			imGuiData[IMGUIDATA_SPEED] -= imGuiData[IMGUIDATA_SPEED] / 100.f;
			if (imGuiData[IMGUIDATA_SPEED] < 1.f)
				imGuiData[IMGUIDATA_SPEED] = 1.f;
		}
		if (imGuiData[IMGUIDATA_SPEED] < -1.f) {
			imGuiData[IMGUIDATA_SPEED] -= imGuiData[IMGUIDATA_SPEED] / 100.f;
			if (imGuiData[IMGUIDATA_SPEED] > -1.f)
				imGuiData[IMGUIDATA_SPEED] = -1.f;
		}

		imGuiData[IMGUIDATA_EARTH_THETA] += -imGuiData[IMGUIDATA_SPEED] / 10.f;
	}

	//	Calculate observer vector

	observer = { 
			-cosf(imGuiData[IMGUIDATA_PHI]) * cosf(imGuiData[IMGUIDATA_THETA]) , 
			-cosf(imGuiData[IMGUIDATA_PHI]) * sinf(imGuiData[IMGUIDATA_THETA]) , 
			-sinf(imGuiData[IMGUIDATA_PHI]) 
	};
}

void App::doFrame()
{
	eventManager();

	//	Update objects

	window.graphics.updatePerspective(observer, center, scale);
	back.updateObserver(window.graphics, observer);
	back.updateWideness(window.graphics, imGuiData[IMGUIDATA_FOV], (Vector2f)window.getDimensions());

	Earth.updateRotation(window.graphics, imGuiData[IMGUIDATA_EARTH_THETA], imGuiData[IMGUIDATA_EARTH_PHI]);
	Moon.updateRotation(window.graphics, -timer.check()/5.f, 0.f, Vector3f(10.f * cosf(timer.check() / 5.f), 10.f * sinf(timer.check() / 5.f), 0.f));

	window.setTitle("Hello World  -  " + std::to_string(int(std::round(window.getFramerate()))) + "fps");

	//	Rendering

	window.graphics.clearDepthBuffer();
	back.Draw(window.graphics);

	Earth.Draw(window.graphics);
	Moon.Draw(window.graphics);

	//	ImGui crap

	iGManager::render();

	//	Push the frame to the scriin

	window.graphics.pushFrame();
}

//	Surface functions

float SincFunction(float x, float y)
{
	if (x == 0.f && y == 0.f)
		return 3;
	return sinf(3 * sqrtf(x * x + y * y)) / sqrtf(x * x + y * y);
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
	return 1.f + (sinf(theta) * sinf(theta) * cosf(phi) + cosf(phi) * cosf(phi) * sinf(phi)) * sinf(5*theta) * cosf(3*phi) / 2.f;
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
	return x * x + y * y + z * z - 1;
}
