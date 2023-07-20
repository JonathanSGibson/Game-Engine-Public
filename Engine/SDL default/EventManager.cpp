#include "EventManager.h"
#include "GameObject.h"

using namespace std;

EventManager* EventManager::inst_ = NULL;

//Used this for help with hashing https://stackoverflow.com/questions/8029121/how-to-hash-stdstring
size_t EventManager::HashString(std::string stringToHash)
{
    std::hash<std::string> hasher;

    return hasher(stringToHash);
}


EventManager* EventManager::GetInstance()
{
    if (inst_ == NULL)
        inst_ = new EventManager();
    return inst_;
}

void EventManager::AddListener(std::string eventType, BaseScript* script)
{
    size_t hashedKey = HashString(eventType);
    if (listeners.find(hashedKey) == listeners.end())
    {
        vector<BaseScript*> newListenerType;
        listeners.insert(pair<size_t, vector<BaseScript*>>(hashedKey, newListenerType));
    }
    listeners.find(hashedKey)->second.push_back(script);
}


void EventManager::FireEvents()
{
    while (events.size() > 0)
    {
        BaseEvent * currentEvent = events[0]; 
        string key = currentEvent->returnType();
        size_t hashedKey = HashString(currentEvent->returnType());

        if (listeners.find(hashedKey) != listeners.end())
        {
            for (BaseScript* currentScript : listeners.find(hashedKey)->second)
            {
                currentScript->HandleEvent(currentEvent);
            }
        }
        events.erase(events.begin());
        delete currentEvent;
    }
}
