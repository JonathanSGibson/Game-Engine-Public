#include "Time.h"

MyTime* MyTime::timeInst_ = NULL;

MyTime* MyTime::GetInstance()
{
	if (timeInst_ == NULL)
		timeInst_ = new MyTime();
	return timeInst_;
}

void MyTime::EndFrame()
{
	frameEnd = clock();
	deltaTime = 1000*(frameEnd - frameStart)/CLOCKS_PER_SEC;
	deltaTime /= 1000; // Convert from milliseconds to seconds
	if (deltaTime > 1)
		deltaTime = 1;
}
