#include "Apps/Fourier.h"
#include "ImGui/IG_Fourier.h"
#include "Keyboard.h"
#include "Mouse.h"

Fourier::Fourier()
	:window(640, 480, "Fourier Plotter")
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

	IG_Fourier::render();
	window.graphics.pushFrame();
}