#pragma once

struct Color {
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;

	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255u);

	static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Yellow;
	static const Color Cyan;
	static const Color Purple;
	static const Color Gray;
	static const Color Orange;
	static const Color Transparent;
};