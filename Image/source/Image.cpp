#include "Image.h"

#include "FreeImage.h"
#include <exception>
#include <string>

Image::Image(const char* filename)
{
	loadFromFile(filename);
}

Image::Image(const Image& other)
	:width(other.width), height(other.height)
{
	Pixels = (Color*)calloc(width * height, sizeof(Color));
	memcpy(Pixels, other.Pixels, width * height * sizeof(Color));
}

Image::Image(Color* pixels, unsigned int width, unsigned int height)
	:width(width), height(height), Pixels(pixels)
{
}

Image::Image(unsigned int w, unsigned int h, Color color)
{
	create(w, h, color);
}

Image::~Image()
{
	if (Pixels != nullptr)
		free(Pixels);
}

bool Image::save(const char* filename) const
{
	if (!width || !height || !Pixels)
		throw std::exception(("Can't save an empty image at [" + std::string(filename) + "]").c_str());

	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32);

	for (unsigned int i = 0; i < height; i++) {
		for (unsigned int j = 0; j < width; j++)
			FreeImage_SetPixelColor(bitmap, j, height - i - 1, (RGBQUAD*)&Pixels[i * width + j]);
	}

	return FreeImage_Save(FIF_PNG, bitmap, filename);
}

void Image::loadFromFile(const char* filename)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename);
	if (format == FIF_UNKNOWN)      format = FreeImage_GetFIFFromFilename(filename);
	if (format == FIF_UNKNOWN)      throw std::exception(("The file [" + std::string(filename) + "] could not be correctly loaded").c_str());

	FIBITMAP* bitmap = FreeImage_Load(format, filename);
	FIBITMAP* bitmap2 = FreeImage_ConvertTo32Bits(bitmap);
	FreeImage_Unload(bitmap);

	width = FreeImage_GetWidth(bitmap2);
	height = FreeImage_GetHeight(bitmap2);

	Pixels = (Color*)calloc(width * height, sizeof(Color));

	FreeImage_ConvertToRawBits((BYTE*)Pixels, bitmap2, FreeImage_GetWidth(bitmap2) * 4, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, true);

	FreeImage_Unload(bitmap2);
}

void Image::create(unsigned int w, unsigned int h, Color color)
{
	width = w;
	height = h;

	Pixels = (Color*)calloc(width * height, sizeof(Color));

	if (color != Color::Transparent) {
		for (unsigned int i = 0; i < width * height; i++)
			Pixels[i] = color;
	}
}
