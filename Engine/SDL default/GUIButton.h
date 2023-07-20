#pragma once
#include "BaseScript.h"
class GUIButton :
    public BaseScript
{
private:

    /// The Y position of the top of the button
    float top;
    /// The X position of the right of the button
    float right;
    /// The Y position of the bottom of the button
    float bottom;
    /// The X position of the left of the button
    float left;

public:
    /// Constructor for generic GUI button
    /// 
    /// Sets the owner of the component andautomatically adds the button as a listener to the EventManager
    /// @param owner_ Passes in the owner of the button
    /// @warning This script requires the owner has a sprite component (Make sure to add the sprite first!)
    GUIButton(BaseObject* owner_);

    /// Virtual function that runs on button press
    /// 
    /// Once inherited contains all the code that runs on button press
    virtual void ButtonPressed() {};

    /// Handles mouse events
    /// 
    /// Works the same as in BaseScript
    /// As the constructor sets it as a listener for left mouse button events it will be passed those events
    /// It then calls CheckButtonPressed(Vector2f mousePos) and if that returns true it will call ButtonPressed()
    ///@param event_ The event to be handled
    void HandleEvent(BaseEvent* event_);

    /// Checks if the button is placed
    /// 
    /// Takes the mouse position and checks if that is within the bounds of the button
    /// @parem mousePos The position of the mouse cursor
    /// @returns Whether the cursor is within the bounds of the button or not
    bool CheckButtonPressed(Vector2f mousePos);

    /// Updates the collidor bounds
    /// 
    /// Updates the bounds of the button using the sprite component of the script's owner
    void UpdateColliderBounds();
};

