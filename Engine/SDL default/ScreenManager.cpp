#include "ScreenManager.h"
#include <string>
#include "Constants.h"

ScreenManager* ScreenManager::instance = NULL;

ScreenManager::ScreenManager()
{
	SDL_Init(SDL_INIT_VIDEO);
	m_window = SDL_CreateWindow(
		"Test Window", //title
		screenX, //initial x position
		screenY, //initial y position
		screenWidth, //width, in pixels
		screenHeight, //height, in pixels
		0 //window behaviour flags
	);

	if (!m_window)
	{
		printf("WINDOW initilisation failed: %s\n", SDL_GetError());
		printf("Press any key to continue\n");
		getchar();
		return;
	}

	m_renderer = SDL_CreateRenderer(
		m_window, //link the renderer to the newly created window
		-1, //index rendering driver
		0 //renderer behaviour flags
	);

	if (!m_renderer)
	{
		printf("RENDERER initilisation failed: %s\n", SDL_GetError());
		printf("Press any key to continue\n");
		getchar();
		return;
	}
}


ScreenManager::~ScreenManager()
{
	if (m_renderer)
	{
		SDL_DestroyRenderer(m_renderer);
	}

	if (m_window)
	{
		SDL_DestroyWindow(m_window);
	}
}

void ScreenManager::SetDisplayColour(int r, int g, int b)
{
	if (m_renderer)
	{
		SDL_SetRenderDrawColor(
			m_renderer,
			r%255,
			g%255,
			b%255,
			255);
	}
	else
	{
		printf("RENDERER initilisation failed: %s\n", SDL_GetError());
		printf("Press any key to continue\n");
		getchar();
		return;
	}
}

ScreenManager* ScreenManager::GetInstance()
{
	if (instance == NULL)
		instance = new ScreenManager();
	return instance;
}
