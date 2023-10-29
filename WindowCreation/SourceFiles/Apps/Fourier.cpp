#include "Apps/Fourier.h"
#include "ImGui/IG_Fourier.h"
#include "Keyboard.h"
#include "Mouse.h"

Fourier::Fourier()
	:window(640, 480, "Fourier Plotter"),
	surf(window.graphics,_RADIAL_SPHERICAL,weirdRadius)
{
}

int Fourier::Run()
{
	while (window.processEvents())
		doFrame();
	return 0;
}

void Fourier::eventManager()
{

}

void Fourier::doFrame()
{
	eventManager();

	window.graphics.clearBuffer(Color::Black);

	surf.Draw(window.graphics);

	IG_Fourier::render();
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
