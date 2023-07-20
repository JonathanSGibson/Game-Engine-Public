#pragma once
#include "GUIButton.h"
class NextSceneButton :
    public GUIButton
{
public:
    /// Sets the cnstructor to use the default GUIButton constructor
    /// 
    /// @param owner_ The owner of the component
    NextSceneButton(BaseObject* owner_) :GUIButton(owner_) {};

    /// Function called when the button is pressed
    /// 
    /// Sets the currently active scene to be the next scene
    void ButtonPressed();

    /// Outputs which scene this button will load to the ImGui window
    void DrawGui();
};

