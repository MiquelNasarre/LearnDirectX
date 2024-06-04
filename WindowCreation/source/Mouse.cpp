#include "Mouse.h"
#include <stdlib.h>

// Definition of static variables

bool*							Mouse::buttonStates = nullptr;
Mouse::event**					Mouse::buttonBuffer = nullptr;
Vector2i						Mouse::Position;
Vector2i						Mouse::ScPosition;
int								Mouse::deltaWheel = 0;

// Function implementations

void Mouse::init()
{
	buttonBuffer = (event**)calloc(maxBuffer, sizeof(void*));
	buttonStates = (bool*)calloc(nKeys, sizeof(bool));
	resetWheel();
	clearBuffer();
}

void Mouse::setButtonPressed(unsigned char buttonCode)
{
	buttonStates[buttonCode] = true;
}

void Mouse::setButtonReleased(unsigned char buttonCode)
{
	buttonStates[buttonCode] = false;
}

void Mouse::setPosition(Vector2i& position)
{
	Position.x = position.x;
	Position.y = position.y;
}

void Mouse::setPosition(int x, int y)
{
	Position.x = x;
	Position.y = y;
}

void Mouse::setScPosition(Vector2i& position)
{
	ScPosition.x = position.x;
	ScPosition.y = position.y;
}

void Mouse::setScPosition(int x, int y)
{
	ScPosition.x = x;
	ScPosition.y = y;
}

void Mouse::increaseWheel(int delta)
{
	deltaWheel += delta;
}

void Mouse::pushEvent(event::Type type, unsigned char buttonCode, Vector2i position)
{
	unsigned int n = maxBuffer - 1u;
	for (unsigned int i = 0; i < maxBuffer; i++)
	{
		if (!buttonBuffer[i])
		{
			n = i;
			break;
		}
	}

	if (n == maxBuffer - 1u)
	{
		delete buttonBuffer[0];
		for (unsigned int i = 0; i < maxBuffer - 1u; i++)
			buttonBuffer[i] = buttonBuffer[i + 1];
	}

	buttonBuffer[n] = new event(type, buttonCode, position);
}

//	Client functions

void Mouse::resetWheel()
{
	deltaWheel = 0;
}

int Mouse::getWheel()
{
	int d = deltaWheel;
	deltaWheel = 0;
	return d;
}

Vector2i Mouse::getPosition()
{
	return Position;
}

Vector2i Mouse::getScPosition()
{
	return ScPosition;
}

bool Mouse::isButtonPressed(unsigned char buttonCode)
{
	return buttonStates[buttonCode];
}

void Mouse::clearBuffer()
{
	for (unsigned int i = 0; i < maxBuffer; i++)
	{
		if (!buttonBuffer[i])
			break;
		delete buttonBuffer[i];
		buttonBuffer[i] = nullptr;
	}

}

bool Mouse::eventIsEmpty()
{
	if (!buttonBuffer[0])
		return true;
	return false;
}

Mouse::event Mouse::popEvent()
{
	if (!buttonBuffer[0])
		return event();

	event ev = *buttonBuffer[0];
	delete buttonBuffer[0];

	for (unsigned int i = 0; i < maxBuffer - 1u; i++)
		buttonBuffer[i] = buttonBuffer[i + 1];
	buttonBuffer[maxBuffer - 1u] = nullptr;

	return ev;
}
