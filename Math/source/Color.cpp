#include "Color.h"

const Color Color::Black		= Color(  0,  0,    0, 255);
const Color Color::White		= Color(255, 255, 255, 255);
const Color Color::Red			= Color(255,   0,   0, 255);
const Color Color::Green		= Color(  0, 255,   0, 255);
const Color Color::Blue			= Color(  0,   0, 255, 255);
const Color Color::Yellow		= Color(255, 255,   0, 255);
const Color Color::Cyan			= Color(  0, 255, 255, 255);
const Color Color::Purple		= Color(255,   0, 255, 255);
const Color Color::Gray			= Color(127, 127, 127, 255);
const Color Color::Orange		= Color(255, 127,   0, 255);
const Color Color::Transparent	= Color(  0,   0,   0,   0);

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	:R{ r }, G{ g }, B{ b }, A{ a }
{
}
