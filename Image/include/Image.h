#pragma once
#include "Color.h"

class Image
{
public:
	Color* Pixels = nullptr;

	unsigned int width;
	unsigned int height;

	Image() {};
	Image(const char* filename);
	Image(const Image& other);
	Image(Color* pixels, unsigned int width, unsigned int height);
	Image(unsigned int width, unsigned int height, Color color = Color::Transparent);
	~Image();

	void loadFromFile(const char* filename);
	void create(unsigned int width, unsigned int height, Color color = Color::Transparent);

	bool save(const char* filename) const;
};