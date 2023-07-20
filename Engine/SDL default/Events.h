#pragma once
#include <string>

class GameObject;
struct Vector2f;
using namespace std;





class BaseEvent
{
protected:
	string eventType;
public:
	std::string returnType() { return eventType; };
	~BaseEvent() {  };
	//Creating a list of event names here so I can call them instead of manually typing (to avoid error by typo)
	const string Up = "Up";
	const string Down = "Down";
	const string Left = "Left";
	const string Right = "Right";
	const string Jump = "Jump";
	const string Quit = "Quit";
	const string Escape = "Escape";
	const string LMB = "LMB";
	const string RMB = "RMB";
};


//The events are hardcoded here, ideally this would be scriptable
//Despite being hardcoded this is still easily extendable

class KeyInput: public BaseEvent{
protected:
	bool keyDown;
public:
	KeyInput(bool keyDown_) { keyDown = keyDown_; }
	bool GetKeyDown() { return keyDown; }
};


class UpInput : public KeyInput {
public:
	UpInput(bool keyDown_) :KeyInput(keyDown_) { eventType = Up; };
};

class DownInput : public KeyInput {
public:
	DownInput(bool keyDown_) :KeyInput(keyDown_) { eventType = Down; };
};

class LeftInput : public KeyInput {

public:
	LeftInput(bool keyDown_) :KeyInput(keyDown_) { eventType = Left; };
};

class RightInput : public KeyInput {
public:
	RightInput(bool keyDown_) :KeyInput(keyDown_) { eventType = Right; };
};

class EscapeInput : public KeyInput {
public:
	EscapeInput(bool keyDown_) :KeyInput(keyDown_) { eventType = Escape; };
};

class JumpInput : public KeyInput {
public:
	JumpInput(bool keyDown_) :KeyInput(keyDown_) { eventType = Jump; };
};

class LMBInput : public KeyInput {
protected:
	float x;
	float y;
public:
	LMBInput(bool keyDown_, float x_, float y_) :KeyInput(keyDown_) { x = x_, y = y_, eventType = LMB; };
	Vector2f GetMousePos1();
};




class QuitEvent : public BaseEvent {
public:
	QuitEvent() { eventType = Quit; }
};