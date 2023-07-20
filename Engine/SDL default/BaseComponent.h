#pragma once
#include "BaseObject.h"

class BaseComponent
{
public:
	/// Virtual function to update every frame
	/// 
	/// Used by components to run any code that needs to run every frame
	virtual void Update() {};

	///Function to get the component's owner
	/// 
	/// @returns The owner of the component
	BaseObject* GetOwner() { return owner; }

	/// Function to return the type of the component
	/// 
	/// @returns The type of the component as a ComponentType enum
	ComponentType GetType() { return type; }

	/// Virtual function to draw GUI information
	/// 
	/// Used by components to draw any information required to the inspector
	/// @warning this does not create an ImGui window which must be done outside of this function (Such as in ObjectManager) (Technically you could make the window in the inheriting class but that is not how it works as implemented currently)
	virtual void DrawGui() {};
protected:
	/// The owner of the component
	BaseObject* owner;

	/// The type of the component
	ComponentType type;
};

