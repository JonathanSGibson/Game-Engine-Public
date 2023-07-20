#include "Transform.h"

void Transform::DrawGui()
{
	if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		static bool showStepButtons = true;
		const float stepCount = 1.0f;

		ImGui::Text("X");
		ImGui::SameLine(); ImGui::InputFloat("", &localPos.x, 0.1f, 1.0f);
		ImGui::Text("Y");
		ImGui::SameLine(); ImGui::InputFloat(" ", &localPos.y, 0.1f, 0.0f);
		ImGui::TreePop();
	}

}

Transform::Transform(Vector2f vec, BaseObject* owner_)
{
	localPos = vec; 
	owner = owner_;
	type = TRANSFORM;
}

Transform::Transform(int x, int y, BaseObject* owner_)
{
	localPos = Vector2f(x,y);
	owner = owner_;
	type = TRANSFORM;
}

Vector2f Transform::GetGlobalPos()
{
	if (owner->GetParent() != nullptr)
		return ((Transform*)(owner->GetParent()->GetComponent(TRANSFORM)))->GetGlobalPos() + localPos;
	else
		return localPos;
}
