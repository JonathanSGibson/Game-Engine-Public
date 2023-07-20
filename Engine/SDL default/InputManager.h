#pragma once
#include "SDL.h"

class EventManager;

class InputManager
{
public:
	/// Gets and stores the EventManager instance
	InputManager();

	/// Polls for inputs and queues events
	void Update();

	/// Gets the instance of the InputManager
	/// 
	/// @returns A pointer to the InputManager instance
	/// @notes InputManager is a singleton so this should be called instead of creating a new one
	static InputManager* GetInstance();
private:
	SDL_Event m_event;
	EventManager* eventManager;
	static InputManager* instance;
};

