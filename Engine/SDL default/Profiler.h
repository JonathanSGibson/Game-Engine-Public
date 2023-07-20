#pragma once
#include <string>
#include <vector>
#include "Time.h"
#include <ctime>
#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <imgui_sdl.h>
#include <imgui_internal.h>
#include <string>


using namespace std;

struct ProfileSample;

class Profiler
{
private:
	static Profiler* inst_;
	ProfileSample* currentSample;
	vector<ProfileSample*> frames; //Vector of the root sample of each frame

	int currentFrame = 0;
	//For gui
	int currentlySelectedFrame;
	bool activeFlameGraph;

public:
	/// Constructs the profiler
	Profiler();

	/// Gets the instance of the Profiler
	/// 
	/// @returns A pointer to the Profiler instance
	/// @notes Profiler is a singleton so this should be called instead of creating a new one
	static Profiler* GetInstance();

	/// Gets the current sample
	/// 
	/// @returns The sample that data is currently being recorded to
	ProfileSample* GetCurrentSample() { return currentSample; }

	/// Sets the current sample
	/// 
	/// Sets the sample that data is  currently being recorded to
	/// @param newSample The sample to be set as the current sample
	void SetCurrentSample(ProfileSample* newSample) { currentSample = newSample; }

	/// Starts recording the current frame
	/// 
	/// Checks the previous frame has been ended properly and if not ends it
	/// Creates a new frame in the profiler
	/// @note Call this at the start of your main loop before any game stuff happens
	void StartFrame();

	/// Stops recording the current frame
	/// 
	/// Ends all the currently active samples
	/// Increments the current frame
	/// @note Call this at the end of your main loop after all of the game stuff happens
	void EndFrame();
	
	/// Outputs information to the ImGui window
	/// 
	/// Outputs a flame graph to the ImGui window
	/// Flame graph can be toggled between showing the current frame or a frame selected via a slider
	void DrawGui();
};


struct ProfileSample
{
	///The height of the flame graph bars
	const float FlameGraphHeight = 35.0f;

	///The name of the sample
	string sampleName;

	/// The time when the sample is created
	clock_t startTime;

	/// The time when the sample is ended
	clock_t endTime;

	/// All the samples within this sample
	vector<ProfileSample*> subSamples;

	/// The sample this is a subsample of
	ProfileSample* parent;

	/// The duration between startTime and endTime
	int  timeDifference;

	/// Constructs and starts the profile
	/// 
	/// Sets startTime to the current time
	/// Sets the parent to the currently active sample
	/// Set this to be the currently active sample
	/// Adds this to the subsamples of its parent if required
	/// @param name The name of the sample
	ProfileSample(string name)
	{
		sampleName = name;
		startTime = clock();
		parent = Profiler::GetInstance()->GetCurrentSample();
		Profiler::GetInstance()->SetCurrentSample(this);
		if (parent != nullptr)
			parent->subSamples.push_back(this);
	}

	/// Ends the sample
	/// 
	/// Sets the end time and time difference, and sets the current sample to be this samples parent
	void EndSample()
	{
		endTime = clock();
		timeDifference = 1000 * (endTime - startTime) / CLOCKS_PER_SEC; //in ms
		Profiler::GetInstance()->SetCurrentSample(parent);
	}

	/// Draws this sample's component of the flame graph to the ImGui window
	/// 
	/// @param root The root sample of the frame
	/// @param canvasTopLeft The top left corner of the area that can be drawn within
	/// @param widthHeight The width and height of the draw space
	/// @param drawList The ImGui draw list to add the rectangles to
	/// @param depth How many subsamples deep this sample is
	void DrawSample(ProfileSample* root, ImVec2 canvasTopLeft, ImVec2 widthHeight, ImDrawList* drawList, int depth = 0)
	{
		float startX = startTime - root->startTime;
		float endX = (startX + timeDifference+1)/root->timeDifference;
		startX /= root->timeDifference;

		ImVec2 rectangleTopLeft;
		rectangleTopLeft.x = canvasTopLeft.x + (startX * widthHeight.x);
		rectangleTopLeft.y = canvasTopLeft.y + (depth * FlameGraphHeight);

		ImVec2 rectangleBottomRight;
		rectangleBottomRight.x = canvasTopLeft.x + (endX * widthHeight.x);
		rectangleBottomRight.y = rectangleTopLeft.y + FlameGraphHeight;
		
		drawList->AddRectFilled(rectangleTopLeft, rectangleBottomRight, ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x77FFFFFF);
		drawList->AddRect(rectangleTopLeft, rectangleBottomRight, ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x7FFFFFFF);

		ImVec2 mouseLocation = { ImGui::GetIO().MousePos.x,ImGui::GetIO().MousePos.y };
		if (!(rectangleTopLeft.x > mouseLocation.x || rectangleBottomRight.x < mouseLocation.x
			|| rectangleTopLeft.y > mouseLocation.y || rectangleBottomRight.y < mouseLocation.y))
			ImGui::SetTooltip((sampleName + '\n' + to_string(timeDifference) + " (ms)").c_str());



		for (ProfileSample* profile : subSamples)
		{
			profile->DrawSample(root, canvasTopLeft, widthHeight, drawList, depth + 1);
		}
	}
};