#include "Mouse.h"

// Definition of static variables

std::bitset<Mouse::nKeys>		Mouse::buttonStates;
std::queue<Mouse::event>		Mouse::buttonBuffer;
Vector2i						Mouse::Position;
Vector2i						Mouse::ScPosition;
int								Mouse::deltaWheel = 0;

// Function implementations

void Mouse::init()
{
	buttonStates.reset();
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
	buttonBuffer.push(event(type, buttonCode, position));
	if (buttonBuffer.size() > maxBuffer)
		buttonBuffer.pop();
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
	while (buttonBuffer.size())
		buttonBuffer.pop();
}

bool Mouse::eventIsEmpty()
{
	if (!buttonBuffer.size())
		return true;
	return false;
}

Mouse::event Mouse::popEvent()
{
	if (!buttonBuffer.size())
		return event();
	event ev = buttonBuffer.front();
	buttonBuffer.pop();
	return ev;
}
