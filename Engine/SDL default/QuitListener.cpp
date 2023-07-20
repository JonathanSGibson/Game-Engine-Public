#include "QuitListener.h"
#include "EventManager.h"
#include "Game.h"

QuitListener::QuitListener(BaseObject* owner_)
{
	owner = owner_;
	EventManager::GetInstance()->AddListener("Quit", this);
}

void QuitListener::HandleEvent(BaseEvent* currentEvent)
{
	if (currentEvent->returnType() == "Quit")
	{
		Game::GetInstance()->Quit();
	}
}

void QuitListener::DrawGui()
{
	if (ImGui::TreeNodeEx("Quit Listener", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Listens for a quit events and quits");
		ImGui::TreePop();
	}
}
