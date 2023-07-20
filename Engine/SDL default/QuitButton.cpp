#include "QuitButton.h"
#include "EventManager.h"

void QuitButton::ButtonPressed()
{
	EventManager::GetInstance()->QueueEvent(new QuitEvent());
}

void QuitButton::DrawGui()
{
	if (ImGui::TreeNodeEx("Quit Button", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Sends a quit event when pressed");
		ImGui::TreePop();
	}
}
