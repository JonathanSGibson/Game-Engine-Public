#include "Profiler.h"

Profiler* Profiler::inst_ = NULL;

Profiler::Profiler()
{
	activeFlameGraph = true;
}

Profiler* Profiler::GetInstance()
{
	if (inst_ == NULL)
		inst_ = new Profiler();
	return inst_;
}

void Profiler::StartFrame()
{
	//Double checking the previous frame has been ended properly
	if (currentFrame > 0 && frames[currentFrame-1]->endTime == 0)
		EndFrame();
	ProfileSample* rootSample = new ProfileSample("Main Loop");
	frames.push_back(rootSample);
	currentSample = rootSample;
}

void Profiler::EndFrame()
{
	//This is just checking every subsample has been ended correctly
	while (currentSample != frames[currentFrame])
	{
		currentSample->EndSample();
	}

	frames[currentFrame]->EndSample();

	currentFrame++;
}

void Profiler::DrawGui()
{
	//Only displays flame graph if there is a frame to display
	if (frames.size() > 2)
	{
		if(ImGui::CollapsingHeader("Profiler", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//Checkbox for current frame - if yes shows current frame - if no slider to select frame from backlog
			ImGui::Checkbox("Active Flame Graph", &activeFlameGraph);
			if (activeFlameGraph)
				currentlySelectedFrame = frames.size() - 2;
			else
			{
				ImGui::SliderInt("Selected Frame", &currentlySelectedFrame, 0, frames.size() - 2);
				//display bars and shit
			}

			ImDrawList* drawlist = ImGui::GetCurrentWindow()->DrawList;
			ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
			ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
			frames[currentlySelectedFrame]->DrawSample(frames[currentlySelectedFrame], canvas_p0, canvas_sz, drawlist);
		}
	}
}
