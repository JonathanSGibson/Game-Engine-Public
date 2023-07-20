#pragma once
#include <chrono>
#include <ctime>

using namespace std;
//Used both of these for help using chrono https://www.techiedelight.com/measure-elapsed-time-program-chrono-library/ and https://en.cppreference.com/w/cpp/chrono

//All time is stored in milliseconds

class MyTime
{
private:
	static MyTime* timeInst_;

	clock_t gameStart;
	clock_t frameStart;
	clock_t frameEnd;
	//DeltaTime is stored in seconds unlike other variables/function returns
	double deltaTime;
public:
	/// Gets the instance of the MyTime
	/// 
	/// @returns A pointer to the MyTime instance
	/// @notes MyTime is a singleton so this should be called instead of creating a new one
	static MyTime* GetInstance();

	/// Gets the game runtime
	/// 
	/// @returns The time since the game started
	int GetRunTime() { return (clock() - gameStart) / CLOCKS_PER_SEC; }

	/// Sets the time this frame started
	void StartFrame() { frameStart = clock(); }
	///Set the end time of a frame and calculate delta time
	void EndFrame();
	///Gets the delta time (time taken to calculate the previous frame)
	/// 
	/// @returns The time the previous frame took to render
	float getDeltaTime() { return deltaTime; };

	///Deletes self
	~MyTime() { delete this; };

};