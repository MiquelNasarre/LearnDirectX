#pragma once

#include "Header.h"
#include <bitset>
#include <queue>

class Keyboard {
	friend class Window;
	
public:
	class event {
	public:
		enum class Type {
			Pressed,
			Released,
			Invalid
		};
		Type type;
		unsigned char keyCode;

		event() : type{ Type::Invalid }, keyCode{ 0 } {}
		event(Type t, unsigned char kc) : type{ t }, keyCode{ kc } {}
	};

private:
	static constexpr unsigned int maxBuffer = 16u;
	static constexpr unsigned int nKeys = 256u;
	static bool autoRepeat;

	static std::bitset<nKeys> keyStates;
	static std::queue<char> charBuffer;
	static std::queue<event> keyBuffer;

	static void init();
	static void setKeyPressed(unsigned char keycode);
	static void setKeyReleased(unsigned char keycode);
	static void clearKeyStates();

	static void pushChar(char character);
	static void pushEvent(event::Type type, unsigned char keyCode);

public:
	static void setAutorepeat(bool state);
	static bool getAutorepeat();

	static void clearBuffers();
	static bool charIsEmpty();
	static char popChar();
	static bool isKeyPressed(unsigned char keycode);

	static bool eventIsEmpty();
	static event popEvent();
};