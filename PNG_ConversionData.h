#ifndef _PNG_CONVERSION_DATA_H_
#define _PNG_CONVERSION_DATA_H_

#include "Node.h"
#include "SDL_image.h"
#include "MyWindow.h"

struct PNG_ConversionData {
	size_t startX;
	size_t startY;
	size_t endX;
	size_t endY;
	size_t tempDist;
	size_t cols;
	Uint32 *pixels;
	Uint32 blackPixel;
	Uint32 greyPixel;
	SDL_PixelFormat *mappingFormat;

	void setPNG_ConversionData(SDL_Surface *&pngSurface) {
		mappingFormat = pngSurface->format;
		SDL_LockSurface(pngSurface);

		blackPixel = SDL_MapRGB(mappingFormat, 0x00, 0x00, 0x00);
		greyPixel = SDL_MapRGB(mappingFormat, 128, 128, 128);

		pixels = static_cast<Uint32 *>(pngSurface->pixels);
		cols = pngSurface->pitch / sizeof(pixels);

		tempDist = 0;
		startX = 0;
		startY = 0;
		endX = pngSurface->pitch / sizeof(pixels);
		endY = static_cast<size_t>(pngSurface->h);

		SDL_UnlockSurface(pngSurface);
	}
};

#endif // !_PNG_CONVERSION_DATA_H_