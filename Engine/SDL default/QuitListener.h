#pragma once
#include "BaseScript.h"
class QuitListener :
    public BaseScript
{
private:
public:
    /// Sets owner and adds this to the EventManager's listeners for the quit event
    /// 
    /// @param owner_ The component's owner
    QuitListener(BaseObject* owner_);

    /// Calls Game's Quit() on quit event
    /// @param currentEvent The event to be handled
    void HandleEvent(BaseEvent* currentEvent);

    /// Outputs text saying that it listens to quit events (Mostly just so you can see the component is attached)
    void DrawGui();
};

