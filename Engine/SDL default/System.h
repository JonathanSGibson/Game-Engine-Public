#pragma once
#include "SDL.h"
#include "ResourceManager.h"
#include "ScreenManager.h"
#include "InputManager.h"
#include "Time.h"
#include "ObjectManager.h"
class System
{
private:
	ResourceManager* resourceManager;
	ScreenManager* screenManager;
	InputManager* inputManager;
	MyTime* time;
	ObjectManager* objectManager;
	static System* instance;
public:
	/// Sets up the screen manager, resource manager, input manager, time and object manager
	System();

	/// Deconstructs the resource manager and screen manager
	~System();

	/// Gets the resource manager
	/// 
	/// @returns a pointer to the ResourceManager
	ResourceManager* GetResourceManager(){ return resourceManager;}

	/// Gets the screen manager
	/// 
	/// @returns a pointer to the ScreenManager
	ScreenManager* GetScreenManager() { return screenManager; }

	/// Gets the input manager
	/// 
	/// @returns a pointer to the InputManager
	InputManager* GetInputManager() { return inputManager; }

	/// Gets the time
	/// 
	/// @returns a pointer to the MyTime
	MyTime* GetTime() { return time; }

	/// Gets the object manager
	/// 
	/// @returns a pointer to the ObjectManager
	ObjectManager* GetObjectManager() { return objectManager; }
	
	/// Gets the instance of the System
	/// 
	/// @returns A pointer to the System instance
	/// @notes System is a singleton so this should be called instead of creating a new one
	static System* GetInstance();
};

