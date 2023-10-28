#include "Application.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "iGManager.h"

App::App()
	: window(640, 480, L"Hello World"),

	TexEarth			(window.graphics, "Resources/earthTextures/default.jpg"),
	TexNEarth			(window.graphics, "Resources/earthTextures/defaultNight.jpg"),
	TexEarthInverted	(window.graphics, "Resources/earthTextures/inverted.jpg"),
	TexNEarthInverted	(window.graphics, "Resources/earthTextures/invertedNight.jpg"),
	TexEarthChalked		(window.graphics, "Resources/earthTextures/chalked.jpg"),
	TexNEarthChalked	(window.graphics, "Resources/earthTextures/chalkedNight.jpg"),
	TexMoon				(window.graphics, "Resources/moonTextures/default.jpg"),
	TexMoonInverted		(window.graphics, "Resources/moonTextures/inverted.jpg"),
	TexMoonChalked		(window.graphics, "Resources/moonTextures/chalked.jpg"),
	TexBack				(window.graphics, "Resources/nightSky/highperformance.jpg"),
	TexBackInverted		(window.graphics, "Resources/nightSky/inverted.jpg"),
	TexBackEarth		(window.graphics, "Resources/earthTextures/projected.jpg"),
	TexBackMoon			(window.graphics, "Resources/moonTextures/projected.jpg"),

	Earth	(window.graphics, _RADIAL_SPHERICAL, constantRadius, TexEarth, TexNEarth),
	Moon	(window.graphics, _RADIAL_SPHERICAL, constantRadius03, TexMoon, TexMoon),
	back	(window.graphics, TexBack, true, PT_AZIMUTHAL)
{
	imGuiData = iGManager::getData();
	imGuiData[IMGUIDATA_THETA] = pi / 2.f;
	imGuiData[IMGUIDATA_FOV] = 1.f;
	imGuiData[IMGUIDATA_MOON_SPEED] = 1.f;

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

	imGuiData[IMGUIDATA_MOON_POS] += imGuiData[IMGUIDATA_MOON_SPEED] / 300.f;

	//	Calculate observer vector

	observer = { 
			-cosf(imGuiData[IMGUIDATA_PHI]) * cosf(imGuiData[IMGUIDATA_THETA]) , 
			-cosf(imGuiData[IMGUIDATA_PHI]) * sinf(imGuiData[IMGUIDATA_THETA]) , 
			-sinf(imGuiData[IMGUIDATA_PHI]) 
	};

	//	Set textures

	if (((int*)imGuiData)[IMGUIDATA_TEXTURE_EARTH] != earthtex) {
		earthtex = ((int*)imGuiData)[IMGUIDATA_TEXTURE_EARTH];
		if (earthtex == 0)
			Earth.updateTextures(window.graphics, TexEarth, TexNEarth);
		if (earthtex == 1)
			Earth.updateTextures(window.graphics, TexEarthInverted, TexNEarthInverted);
		if (earthtex == 2)
			Earth.updateTextures(window.graphics, TexEarthChalked, TexNEarthChalked);
		if (earthtex == 3)
			Earth.updateTextures(window.graphics, TexMoon, TexMoon);
	}
	if (((int*)imGuiData)[IMGUIDATA_TEXTURE_MOON] != moontex) {
		moontex = ((int*)imGuiData)[IMGUIDATA_TEXTURE_MOON];
		if (moontex == 0)
			Moon.updateTextures(window.graphics, TexMoon, TexMoon);
		if (moontex == 1)
			Moon.updateTextures(window.graphics, TexMoonInverted, TexMoonInverted);
		if (moontex == 2)
			Moon.updateTextures(window.graphics, TexMoonChalked, TexMoonChalked);
		if (moontex == 3)
			Moon.updateTextures(window.graphics, TexEarth, TexNEarth);
	}
	if (((int*)imGuiData)[IMGUIDATA_TEXTURE_BACKGROUND] != backtex) {
		backtex = ((int*)imGuiData)[IMGUIDATA_TEXTURE_BACKGROUND];
		if (backtex == 0)
			back.updateTexture(window.graphics, TexBack);
		if (backtex == 1)
			back.updateTexture(window.graphics, TexBackInverted);
		if (backtex == 2)
			back.updateTexture(window.graphics, TexBackEarth);
		if (backtex == 3)
			back.updateTexture(window.graphics, TexBackMoon);
	}
}

void App::doFrame()
{
	eventManager();

	//	Update objects

	window.graphics.updatePerspective(observer, center, scale);
	back.updateObserver(window.graphics, observer);
	back.updateWideness(window.graphics, imGuiData[IMGUIDATA_FOV], (Vector2f)window.getDimensions());

	Earth.updateRotation(window.graphics, imGuiData[IMGUIDATA_EARTH_THETA], imGuiData[IMGUIDATA_EARTH_PHI]);
	Moon.updateRotation(window.graphics, -imGuiData[IMGUIDATA_MOON_POS], 0.f, Vector3f(10.f * cosf(imGuiData[IMGUIDATA_MOON_POS]), 10.f * sinf(imGuiData[IMGUIDATA_MOON_POS]), 0.f));

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
