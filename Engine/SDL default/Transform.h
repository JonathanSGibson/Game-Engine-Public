#pragma once
#include "BaseComponent.h"


class Transform: public BaseComponent
{
protected:
	/// Local coordinates of the owning object
	/// @note As screen coordinates are measured from the top left of the screen a higher Y value is lower on the screen
	Vector2f localPos;
public:

	/// Constructor for the Transform component
	/// 
	/// Sets up required variables for the component
	/// @param x The X coordinate of the position
	/// @param y The Y coordinate of the position
	/// @param owner_ The owner of the component
	Transform(int x, int y, BaseObject* owner_);

	/// Constructor for the Transform component
	/// 
	/// As Transform(int x, int y, BaseObject* owner_) but taking a Vector2f instead of two ints
	/// @param vec The vector of the local position coordinates
	/// @param owner_ the owner of the component
	Transform(Vector2f vec, BaseObject* owner_);

	/// Outputs information to the ImGui window
	/// 
	/// Outputs the local position as two editable floats
	void DrawGui();

	/// Gets the local position
	/// 
	/// @returns The local position
	Vector2f GetLocalPos() { return localPos; }

	/// Gets the global position
	/// 
	/// Recursively goes up the parentage hierarchy adding the positions together to get the global position
	/// @returns The global position
	Vector2f GetGlobalPos();

	/// Sets the local position
	/// 
	/// @param newPos The position to set the object to
	void SetPosition(Vector2f newPos) { localPos = newPos; }

	/// Sets the Y coordinate of the local position
	/// 
	/// @param newPos The new y position
	void SetYPosition(float newPos) { localPos.y = newPos; }

	/// Sets the X coordinate of the local position
	/// 
	/// @param newPos The new X position
	void SetXPosition(float newPos) { localPos.x = newPos; };
};
