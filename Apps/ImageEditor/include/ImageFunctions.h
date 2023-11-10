#pragma once
#include "Image.h"

Image gradient();

Image EarthDiagram();

Image createNightSky(unsigned int width, unsigned int height);

Image createAzimuth(const Image mercator, const unsigned int height, bool KeepAllStars = false);

Image chalk(const Image image);

Image topDown(const Image image);

Image invert(const Image image);