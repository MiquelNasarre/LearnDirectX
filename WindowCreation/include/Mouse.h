#pragma once

#include "Math/Vectors.h"

class Mouse {
	friend class Window;

public:
	class event {
	public:
		enum class Type {
			Pressed,
			Released,
			Moved,
			Wheel,
			Invalid
		};
		Type type;
		unsigned char buttonCode;
		Vector2i position;

		event() : type{ Type::Invalid }, buttonCode{ 0 }, position{ Vector2i() } {}
		event(Type t, unsigned char kc, Vector2i pos) : type{ t }, buttonCode{ kc }, position{ pos } {}
	};

private:

	static constexpr unsigned int maxBuffer = 16u;
	static constexpr unsigned int nKeys = 4u;

	static bool* buttonStates;
	static event** buttonBuffer;
	static Vector2i Position;
	static Vector2i ScPosition;
	static int deltaWheel;

	static void init();

	static void setButtonPressed(unsigned char buttonCode);
	static void setButtonReleased(unsigned char buttonCode);
	static void setPosition(Vector2i& Position);
	static void setPosition(int x, int y);
	static void setScPosition(Vector2i& Position);
	static void setScPosition(int x, int y);
	static void increaseWheel(int delta);

	static void pushEvent(event::Type type, unsigned char buttonCode, Vector2i position);

public:
	enum :unsigned char {
		Left,
		Right,
		Middle,
		None
	};

	static void resetWheel();
	static int getWheel();
	static Vector2i getPosition();
	static Vector2i getScPosition();
	static bool isButtonPressed(unsigned char buttonCode);

	static void clearBuffer();
	static bool eventIsEmpty();
	static event popEvent();
};