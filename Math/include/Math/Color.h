#pragma once

struct Color {
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;

	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255u);
};