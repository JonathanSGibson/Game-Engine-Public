#pragma once
#include "GUIButton.h"
class QuitButton :
    public GUIButton
{
public:
    /// Sets the cnstructor to use the default GUIButton constructor
    /// 
    /// @param owner_ The owner of the component
    QuitButton(BaseObject* owner_) :GUIButton(owner_) {};

    /// Function called when the button is pressed
    /// 
    /// Queues a quit event
    void ButtonPressed();

    /// Outputs text saying that it quits when pressed (Mostly just so you can see the component is attached)
    void DrawGui();
};

