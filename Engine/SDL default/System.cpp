#include "System.h"

System* System::instance = NULL;


System::System()
{
	screenManager = ScreenManager::GetInstance();
	resourceManager = ResourceManager::GetInstance();
	inputManager = InputManager::GetInstance();
	time = MyTime::GetInstance();
	objectManager = ObjectManager::GetInstance();
}

System* System::GetInstance()
{
	if (instance == NULL)
		instance = new System();
	return instance;
}

System::~System()
{
	resourceManager->~ResourceManager();
	screenManager->~ScreenManager();
}