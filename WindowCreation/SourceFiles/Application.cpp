#include "Application.h"

App::App()
	:window(640, 480, L"Hello World")
{
}

int App::Run()
{

	while (window.pollEvent())
		doFrame();

	return 0;
}

void App::doFrame()
{
	if (Keyboard::isKeyPressed('W'))
		window.setTitle("UwU");
}

