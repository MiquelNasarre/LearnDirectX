#include "Keyboard.h"

// Definition of static variables

std::bitset<Keyboard::nKeys>	Keyboard::keyStates;
std::queue<char>				Keyboard::charBuffer;
std::queue<Keyboard::event>		Keyboard::keyBuffer;
bool							Keyboard::autoRepeat = false;

// Function implementations

void Keyboard::init()
{
	clearKeyStates();
	clearBuffers();
	setAutorepeat(true);
}

void Keyboard::setKeyPressed(unsigned char keycode)
{
	keyStates[keycode] = true;
}

void Keyboard::setKeyReleased(unsigned char keycode)
{
	keyStates[keycode] = false;
}

void Keyboard::clearKeyStates()
{
	keyStates.reset();
}

void Keyboard::pushChar(char character)
{
	charBuffer.push(character);
	if (charBuffer.size() > maxBuffer)
		charBuffer.pop();
}

void Keyboard::pushEvent(event::Type type, unsigned char keyCode)
{
	keyBuffer.push(event(type, keyCode));
	if (keyBuffer.size() > maxBuffer)
		keyBuffer.pop();
}

void Keyboard::setAutorepeat(bool state)
{
	autoRepeat = state;
}

bool Keyboard::getAutorepeat()
{
	return autoRepeat;
}

void Keyboard::clearBuffers()
{
	while (charBuffer.size())
		charBuffer.pop();
	while (keyBuffer.size())
		keyBuffer.pop();
}

bool Keyboard::charIsEmpty()
{
	if (!charBuffer.size())
		return true;
	return false;
}

char Keyboard::popChar()
{
	if (!charBuffer.size())
		return 0;
	char character = charBuffer.front();
	charBuffer.pop();
	return character;
}

bool Keyboard::isKeyPressed(unsigned char keycode)
{
	return keyStates[keycode];
}

bool Keyboard::eventIsEmpty()
{
	if (!keyBuffer.size())
		return true;
	return false;
}

Keyboard::event Keyboard::popEvent()
{
	if (!keyBuffer.size())
		return event();
	event ev = keyBuffer.front();
	keyBuffer.pop();
	return ev;
}