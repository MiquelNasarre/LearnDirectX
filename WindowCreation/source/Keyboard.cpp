#include "Keyboard.h"
#include <stdlib.h>

// Definition of static variables

bool*							Keyboard::keyStates = nullptr;
char**							Keyboard::charBuffer = nullptr;
Keyboard::event**				Keyboard::keyBuffer = nullptr;
bool							Keyboard::autoRepeat = false;

// Function implementations

void Keyboard::init()
{
	keyStates = (bool*)calloc(nKeys, sizeof(bool));
	keyBuffer = (event**)calloc(maxBuffer, sizeof(void*));
	charBuffer = (char**)calloc(maxBuffer, sizeof(void*));
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
	for (unsigned int i = 0; i < nKeys; i++)
		keyStates[i] = false;
}

void Keyboard::pushChar(char character)
{
	unsigned int n = maxBuffer - 1u;
	for (unsigned int i = 0; i < maxBuffer; i++)
	{
		if (!charBuffer[i])
		{
			n = i;
			break;
		}
	}

	if (n == maxBuffer - 1u)
	{
		delete charBuffer[0];
		for (unsigned int i = 0; i < maxBuffer - 1u; i++)
			charBuffer[i] = charBuffer[i + 1];
	}

	charBuffer[n] = new char(character);
}

void Keyboard::pushEvent(event::Type type, unsigned char keyCode)
{
	unsigned int n = maxBuffer - 1u;
	for (unsigned int i = 0; i < maxBuffer; i++)
	{
		if (!keyBuffer[i])
		{
			n = i;
			break;
		}
	}

	if (n == maxBuffer - 1u)
	{
		delete keyBuffer[0];
		for (unsigned int i = 0; i < maxBuffer - 1u; i++)
			keyBuffer[i] = keyBuffer[i + 1];
	}

	keyBuffer[n] = new event(type, keyCode);
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
	for (unsigned int i = 0; i < maxBuffer; i++)
	{
		if (!charBuffer[i])
			break;
		delete charBuffer[i];
		charBuffer[i] = nullptr;
	}

	for (unsigned int i = 0; i < maxBuffer; i++)
	{
		if (!keyBuffer[i])
			break;
		delete keyBuffer[i];
		keyBuffer[i] = nullptr;
	}
}

bool Keyboard::charIsEmpty()
{
	if (!charBuffer[0])
		return true;
	return false;
}

bool Keyboard::isKeyPressed(unsigned char keycode)
{
	return keyStates[keycode];
}

bool Keyboard::eventIsEmpty()
{
	if (!keyBuffer[0])
		return true;
	return false;
}

char Keyboard::popChar()
{
	if (!charBuffer[0])
		return 0;

	char ev = *charBuffer[0];
	delete charBuffer[0];

	for (unsigned int i = 0; i < maxBuffer - 1u; i++)
		charBuffer[i] = charBuffer[i + 1];
	charBuffer[maxBuffer - 1u] = nullptr;

	return ev;
}

Keyboard::event Keyboard::popEvent()
{
	if (!keyBuffer[0])
		return event();

	event ev = *keyBuffer[0];
	delete keyBuffer[0];

	for (unsigned int i = 0; i < maxBuffer - 1u; i++)
		keyBuffer[i] = keyBuffer[i + 1];
	keyBuffer[maxBuffer - 1u] = nullptr;

	return ev;
}