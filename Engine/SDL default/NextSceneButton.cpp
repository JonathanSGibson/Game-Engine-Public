#include "NextSceneButton.h"

void NextSceneButton::ButtonPressed()
{
	ObjectManager::GetInstance()->SetActiveScene(ObjectManager::GetInstance()->GetCurrentScene() + 1);
}

void NextSceneButton::DrawGui()
{
	if (ImGui::TreeNodeEx("Next Scene Button", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(("Scene to load: " + to_string(ObjectManager::GetInstance()->GetCurrentScene() + 1)).c_str());
		ImGui::TreePop();
	}
}
