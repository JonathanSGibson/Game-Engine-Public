#pragma once
#include "Events.h"
#include <vector>
#include <map>
#include <unordered_map>
#include "BaseObject.h"
#include "BaseScript.h"

using namespace std;

//Used this for help with formatting a singleton in c++ https://www.oreilly.com/library/view/c-cookbook/0596007612/ch08s10.html
class EventManager
{
private:
	vector<BaseEvent*> events;

	static EventManager* inst_; //The instance of the singleton 
	static size_t HashString(string stringToHash);

	//Map of all listeners, key is determined by the event type as done in HashString
	map <size_t, vector<BaseScript*>> listeners;
public:
	/// Gets the EventManager
	/// 
	/// @returns A pointer to the EventManager
	/// @notes EventManager is a singleton so this should be run instead of making a new one
	static EventManager* GetInstance();

	/// Adds a listener to the EventManager
	/// 
	/// Adds a BaseScript pointer to the map of listeners
	/// @param eventType The types of event the listener listens for
	/// @param script The script to be added as a listener
	void AddListener(string eventType, BaseScript* script);
	
	/// Adds an event to the queue
	/// 
	/// Pushes back an event to the events vector
	/// @param event The event to be queued
	void QueueEvent(BaseEvent* event) { events.push_back(event); };

	/// Fires all events in the queue
	/// 
	/// Fires every event in the events vector to every listener which is listening for events of that type
	void FireEvents();

	///Deletes the event manager
	~EventManager() { delete this; };
};

