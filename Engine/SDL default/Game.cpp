#include <stdio.h>
#include "Game.h"

Game* Game::instance = NULL;



Game* Game::GetInstance()
{
	if (instance == NULL)
		instance = new Game();
	return instance;
}

Game::Game()
{
	system = System::GetInstance();
	screen = system->GetScreenManager();
	resourceManager = system->GetResourceManager();
}

Game::~Game()
{
	system->~System();
}
System* Game::GetSystem()
{
	return system;
}