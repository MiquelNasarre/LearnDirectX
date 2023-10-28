#include "Application.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "iGManager.h"

float IG_DATA::EARTH_THETA			= 0.f;
float IG_DATA::EARTH_PHI			= 0.f;
float IG_DATA::SPEED				= 0.f;
float IG_DATA::MOON_SPEED			= 1.f;
float IG_DATA::MOON_POS				= 0.f;
float IG_DATA::THETA				= pi / 2.f;
float IG_DATA::PHI					= 0.f;
float IG_DATA::FOV					= 1.f;
int	  IG_DATA::TEXTURE_EARTH		= 0;
int   IG_DATA::TEXTURE_MOON			= 0;
int   IG_DATA::TEXTURE_BACKGROUND	= 0;
int	  IG_DATA::UPDATE_LIGHT			= -1;

IG_DATA::lightsource* IG_DATA::LIGHTS = (IG_DATA::lightsource*)calloc(sizeof(IG_DATA::lightsource), 8);

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

	IG_DATA::LIGHTS[0].is_on = true;
	IG_DATA::LIGHTS[0].color = Color::White.getColor4();
	IG_DATA::LIGHTS[0].intensities = { 32000.f,5000.f };
	IG_DATA::LIGHTS[0].position = { 160.f,0.f,60.f };

	window.setDarkTheme(true);
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
		initialDragAngles = { IG_DATA::EARTH_THETA,IG_DATA::EARTH_PHI };
	}
	if (dragging && !Mouse::isButtonPressed(Mouse::Left))
		dragging = false;

	if (dragging) {
		Vector2i movement = Mouse::getPosition() - initialDrag;
		float temp = initialDragAngles.x - 2.f * (float)movement.x / scale;
		IG_DATA::SPEED = -10.f * (temp - IG_DATA::EARTH_THETA);
		IG_DATA::EARTH_THETA = temp;
		IG_DATA::EARTH_PHI = initialDragAngles.y + 2.f * (float)movement.y / scale;
		if (IG_DATA::EARTH_PHI > pi / 2.f) {
			IG_DATA::EARTH_PHI = pi / 2.f - 0.0001f;
			initialDrag.y = Mouse::getPosition().y - int((pi / 2.f - initialDragAngles.y) * scale / 2.f);
		}
		if (IG_DATA::EARTH_PHI < -pi / 2.f) {
			IG_DATA::EARTH_PHI = -pi / 2.f + 0.0001f;
			initialDrag.y = Mouse::getPosition().y - int((-pi / 2.f - initialDragAngles.y) * scale / 2.f);
		}
	}
	else {
		if (IG_DATA::SPEED > 1.f) {
			IG_DATA::SPEED *= 0.99f;
			if (IG_DATA::SPEED < 1.f)
				IG_DATA::SPEED = 1.f;
		}
		if (IG_DATA::SPEED < -1.f) {
			IG_DATA::SPEED *= 0.99f;
			if (IG_DATA::SPEED > -1.f)
				IG_DATA::SPEED = -1.f;
		}

		IG_DATA::EARTH_THETA += -IG_DATA::SPEED / 10.f;
	}

	IG_DATA::MOON_POS += IG_DATA::MOON_SPEED / 300.f;

	//	Calculate observer vector

	observer = { 
			-cosf(IG_DATA::PHI) * cosf(IG_DATA::THETA) ,
			-cosf(IG_DATA::PHI) * sinf(IG_DATA::THETA) ,
			-sinf(IG_DATA::PHI)
	};

	//	Set textures

	if (IG_DATA::TEXTURE_EARTH != earthtex) {
		earthtex = IG_DATA::TEXTURE_EARTH;
		if (earthtex == 0)
			Earth.updateTextures(window.graphics, TexEarth, TexNEarth);
		if (earthtex == 1)
			Earth.updateTextures(window.graphics, TexEarthInverted, TexNEarthInverted);
		if (earthtex == 2)
			Earth.updateTextures(window.graphics, TexEarthChalked, TexNEarthChalked);
		if (earthtex == 3)
			Earth.updateTextures(window.graphics, TexMoon, TexMoon);
	}
	if (IG_DATA::TEXTURE_MOON != moontex) {
		moontex = IG_DATA::TEXTURE_MOON;
		if (moontex == 0)
			Moon.updateTextures(window.graphics, TexMoon, TexMoon);
		if (moontex == 1)
			Moon.updateTextures(window.graphics, TexMoonInverted, TexMoonInverted);
		if (moontex == 2)
			Moon.updateTextures(window.graphics, TexMoonChalked, TexMoonChalked);
		if (moontex == 3)
			Moon.updateTextures(window.graphics, TexEarth, TexNEarth);
	}
	if (IG_DATA::TEXTURE_BACKGROUND != backtex) {
		backtex = IG_DATA::TEXTURE_BACKGROUND;
		if (backtex == 0)
			back.updateTexture(window.graphics, TexBack);
		if (backtex == 1)
			back.updateTexture(window.graphics, TexBackInverted);
		if (backtex == 2)
			back.updateTexture(window.graphics, TexBackEarth);
		if (backtex == 3)
			back.updateTexture(window.graphics, TexBackMoon);
	}

	//	Set lights

	int l = IG_DATA::UPDATE_LIGHT;
	if (l > -1) {
		Earth.updateLight(window.graphics, l, IG_DATA::LIGHTS[l].intensities, IG_DATA::LIGHTS[l].color, IG_DATA::LIGHTS[l].position);
		Moon.updateLight(window.graphics, l, IG_DATA::LIGHTS[l].intensities, IG_DATA::LIGHTS[l].color, IG_DATA::LIGHTS[l].position);
		IG_DATA::UPDATE_LIGHT = -1;
	}
}

void App::doFrame()
{
	eventManager();

	//	Update objects

	window.graphics.updatePerspective(observer, center, scale);
	back.updateObserver(window.graphics, observer);
	back.updateWideness(window.graphics, IG_DATA::FOV, (Vector2f)window.getDimensions());

	Earth.updateRotation(window.graphics, IG_DATA::EARTH_THETA, IG_DATA::EARTH_PHI);
	Moon.updateRotation(window.graphics, -IG_DATA::MOON_POS, 0.f, Vector3f(10.f * cosf(IG_DATA::MOON_POS), 10.f * sinf(IG_DATA::MOON_POS), 0.f));

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
