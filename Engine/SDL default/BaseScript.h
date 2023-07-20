#pragma once
#include "BaseComponent.h"
#include "Events.h"
class BaseScript :
    public BaseComponent
{
public:
    /// Constructor automatically set the type
    /// 
    /// Sets the type to SCRIPT
    /// @note This means that any scripts created from BaseScript do not require their type being set in their constructor
    BaseScript() { type = SCRIPT; }

    /// Function to handle events
    /// 
    /// If this script is set as a listener in EventManager it will be passed events through this function
    /// This means that each script can take in specific events and act accordingly
    /// @param currentEvent The event to be handled, contains different information depending on the event type
    virtual void HandleEvent(BaseEvent* currentEvent) {};
};

