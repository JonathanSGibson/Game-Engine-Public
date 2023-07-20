#include "EnemyScript.h"
#include "Transform.h"
#include "Physics.h"
#include "PlayerScript.h"
#include <typeinfo>
#include <iostream>
EnemyScript::EnemyScript(BaseObject* owner_, float moveSpeed_, float movementBorderLeft_, float movementBorderRight_, int damage_)
{
	owner = owner_;
	if (owner->GetComponent(PHYSICS) == nullptr || movementBorderLeft_ > movementBorderRight_)
		delete this; //Need to add proper error handling, but this should stop crashes
	else
	{
		moveSpeed = moveSpeed_;
		movementBorderLeft = movementBorderLeft_;
		movementBorderRight = movementBorderRight_;
		damage = damage_;
		owner->AddTag("Enemy");
	}
}

void EnemyScript::Update()
{
	Vector2f pos = ((Transform*)owner->GetComponent(TRANSFORM))->GetGlobalPos();
	Physics* physics = ((Physics*)owner->GetComponent(PHYSICS));
	if (pos.x > movementBorderRight)
		movingRight = false;
	else if (pos.x < movementBorderLeft)
		movingRight = true;

	if (movingRight)
		physics->SetXForce(moveSpeed);
	else
		physics->SetXForce(-moveSpeed);



	for (BaseObject* object : physics->GetCollisions())
	{
		if (object->HasTag("Player"))
		{
			for (BaseComponent* script : object->GetScripts())
			{
				//Used this to work out how to check the dynamic casting https://en.cppreference.com/w/cpp/language/dynamic_cast
				if (dynamic_cast<PlayerScript*>(script) != nullptr)
				{
					((PlayerScript*)script)->DealDamage(damage);
					break;
				}
			}
		}
	}
}

void EnemyScript::DrawGui()
{
	if (ImGui::TreeNodeEx("Player Script", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(("Moving Right: " + to_string(movingRight)).c_str());
		ImGui::Text("Move Speed ");
		ImGui::SameLine(); ImGui::InputFloat("", &moveSpeed, 0.1f, 1.0f);
		ImGui::Text("Movement Border Right ");
		ImGui::SameLine(); ImGui::InputFloat(" ", &movementBorderRight, 0.1f, 1.0f);
		ImGui::Text("Movement Border Left ");
		ImGui::SameLine(); ImGui::InputFloat("  ", &movementBorderLeft, 0.1f, 1.0f);
		ImGui::Text("Damage ");
		ImGui::SameLine(); ImGui::InputInt("          ", &damage);

		ImGui::TreePop();
	}
}
