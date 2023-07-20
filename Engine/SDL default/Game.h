#pragma once

#include <string>

#include "SDL.h"
#include "SDL_ttf.h"
#include "System.h"

class Game {
private:
	System* system;
	ScreenManager* screen;
	ResourceManager* resourceManager;
	static Game* instance;
	bool quit = false;
public:
	// Gets the instance of the Game
	/// 
	/// @returns A pointer to the Game instance
	/// @notes Game is a singleton so this should be called instead of creating a new one
	static Game* GetInstance();

	/// Gets the system, screen and resource manager
	Game();

	///Deconstructs the system
	~Game();

	/// Gets the system
	/// 
	/// @returns Pointer to the System
	System* GetSystem();

	/// Sets quit to be true
	/// 
	/// @note Your game while loop should use GetQuit() as its parameter
	void Quit() { quit = true; }

	///	Gets quit
	/// 
	/// @return Quit bool
	/// /// @note Your game while loop should use this as its parameter
	bool GetQuit() { return quit; }

	/// Bool used to determine if editor GUI should be shown
	bool guiOn;
};