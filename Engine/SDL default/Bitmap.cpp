#include <string>
#include "Bitmap.h"

#include "SDL.h"
#include "SDL_render.h"
#include <iostream>

using namespace std;

Bitmap::Bitmap(SDL_Renderer* renderer, string fileName, bool useTransparency)
{
	//store the renderer for future configuring and drawing
	m_pRenderer = renderer;

	//create the bitmap surface
	m_pbitmapSurface = SDL_LoadBMP(fileName.c_str());
	if (!m_pbitmapSurface)
	{
		//bitmap not loaded? output the error
		printf("SURFACE for bitmap '%s' not loaded! \n", fileName.c_str());
		printf("%s\n", SDL_GetError());
	}
	else
	{
		//if we are to use the transparency, going to assume
		//the colour key is magenta (R=255, g=0, b=255)
		if (useTransparency)
		{
			Uint32 colourKey = SDL_MapRGB(m_pbitmapSurface->format, 255, 0, 255);
			SDL_SetColorKey(m_pbitmapSurface, SDL_TRUE, colourKey);
		}

		//create the texture
		m_pbitmapTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_pbitmapSurface);
		if (!m_pbitmapTexture)
		{
			//texture not loaded? Output the error
			printf("TEXTURE for bitmap '%s' not loaded! \n", fileName.c_str());
			printf(" % s\n", SDL_GetError());
		}
	}

}

void Bitmap::draw()
{
	//Render the bitmap at the x/y coordinates
	if (m_pbitmapTexture)
	{
		SDL_Rect destRect = { m_x, m_y, m_pbitmapSurface->w, m_pbitmapSurface-> h };
		SDL_RenderCopy(m_pRenderer, m_pbitmapTexture, NULL, &destRect);
	}
}

Bitmap::~Bitmap()
{
	if (m_pbitmapTexture)
		SDL_DestroyTexture(m_pbitmapTexture);
	if (m_pbitmapSurface)
		SDL_FreeSurface(m_pbitmapSurface);
}