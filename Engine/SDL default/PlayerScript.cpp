#include "PlayerScript.h"
#include "Transform.h"
#include "Physics.h"
#include "EventManager.h"
#include <iostream>
#include "Time.h"
#include "Sprite.h"

using namespace std;

PlayerScript::PlayerScript(BaseObject* owner_, int maxHealth_, float moveSpeed_, float jumpForce_, float invulnerableDuration_, Vector2f respawnPoint_)
{
	owner = owner_;
	if (owner->GetComponent(PHYSICS) == nullptr)
		delete this; //Need to add proper error handling, but this should stop crashes
	health = maxHealth_;
	maxHealth = maxHealth_;
	respawnPoint = respawnPoint_;
	moveSpeed = moveSpeed_;
	jumpForce = jumpForce_;
	invulnerableDuration = invulnerableDuration_ * 1000; //Multiplying by 1000 to convert to miliseconds
	canJump = true;

	EventManager::GetInstance()->AddListener("Left", this);
	EventManager::GetInstance()->AddListener("Right", this);
	EventManager::GetInstance()->AddListener("Jump", this);
	owner->AddTag("Player");
}

void PlayerScript::DealDamage(int damage)
{
	if (!invulnerable)
	{
		//Add in iframe setting in here
		health -= damage;
		if (health <= 0)
			Respawn();
		else if (damage > 0)
		{
			invulnerable = true;
			invulnerableStart = clock();
		}
	}
}

void PlayerScript::Respawn()
{
	health = maxHealth;
	((Physics*)owner->GetComponent(TRANSFORM))->SetForce(Vector2f(0, 0));
	moveRight = false;
	moveLeft = false;
	((Transform*)owner->GetComponent(TRANSFORM))->SetPosition(respawnPoint);
	//Need to do some logic/maths to work out how to set the position correctly ^
	// If it is a single screen I can just set it based on global position as above
	// If I want a scrolling screen then I will have to work out a way to calculate this based on the scene root moving with the camera (So not based on screen space but world space) - not sure how that will work
	//If I accidentally left these comments in, hi Nick o/

	//No visual feedback from death, but resets the player to their respawn point and resets their health
}


//Because my physics system does not have any form of friction you have to manually stop horizontal movement (Vertical movement will eventually get counteracted by gravity)
//If a friction system was implemented you could remove the manual stopping of horizontal movement, however in my opinion having snappy stop/start movement like this is better in platformers anyway
void PlayerScript::Jump()
{
	((Physics*)owner->GetComponent(PHYSICS))->AddForce(Vector2f(0, -jumpForce));
	canJump = false;
}


void PlayerScript::Checkpoint(Vector2f checkpointPosition)
{
	respawnPoint = checkpointPosition;
}

void PlayerScript::HandleEvent(BaseEvent* currentEvent)
{
	if (canJump && currentEvent->returnType() == currentEvent->Jump && ((KeyInput*)currentEvent)->GetKeyDown())
		Jump();
	else if (currentEvent->returnType() == currentEvent->Left)
		moveLeft = ((KeyInput*)currentEvent)->GetKeyDown();
	else if (currentEvent->returnType() == currentEvent->Right)
		moveRight = ((KeyInput*)currentEvent)->GetKeyDown();
}

void PlayerScript::DrawGui()
{
	if (ImGui::TreeNodeEx("Player Script", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(("Health: " + to_string(health)).c_str());
		ImGui::Text("Max Health: ");
		ImGui::SameLine(); ImGui::InputInt("", &maxHealth);
		ImGui::Text("Move Speed: ");
		ImGui::SameLine(); ImGui::InputFloat("", &moveSpeed, 0.1f, 1.0f);
		ImGui::Text("Jump Force: ");
		ImGui::SameLine(); ImGui::InputFloat("", &jumpForce, 0.1f, 1.0f);
		ImGui::Text("Invulnerability Dutation (ms): ");
		ImGui::SameLine(); ImGui::InputFloat("", &invulnerableDuration, 0.1f, 1.0f);
		ImGui::Text(("Can Jump: " + to_string(canJump)).c_str());
		ImGui::Text(("Invulnerable: " + to_string(invulnerable)).c_str());

		ImGui::Text("Respawn point x ");
		ImGui::SameLine(); ImGui::InputFloat("", &respawnPoint.x, 0.1f, 1.0f);
		ImGui::Text("Respawn point y ");
		ImGui::SameLine(); ImGui::InputFloat(" ", &respawnPoint.y, 0.1f, 0.0f);
		ImGui::TreePop();
	}
}

void PlayerScript::Update()
{
	//Code for health bar
	ImGui::Begin("Health", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	ImGui::Text("Health");
	float startX = 0;
	float endX = (float)health/(float)maxHealth;

	ImVec2 rectangleTopLeft;
	rectangleTopLeft.x = ImGui::GetCursorScreenPos().x;
	rectangleTopLeft.y = ImGui::GetCursorScreenPos().y + 5;

	ImVec2 rectangleBottomRight;
	rectangleBottomRight.x = ImGui::GetCursorScreenPos().x + (endX * ImGui::GetContentRegionAvail().x);
	rectangleBottomRight.y = rectangleTopLeft.y + 5;

	ImVec2 rectangleBottomRightMax;
	rectangleBottomRightMax.x = ImGui::GetCursorScreenPos().x + (ImGui::GetContentRegionAvail().x);
	rectangleBottomRightMax.y = rectangleTopLeft.y + 5;

	ImDrawList* drawList = ImGui::GetCurrentWindow()->DrawList;
	drawList->AddRectFilled(rectangleTopLeft, rectangleBottomRightMax, ImGui::GetColorU32(ImGuiCol_Button));
	drawList->AddRectFilled(rectangleTopLeft, rectangleBottomRight, ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x77FFFFFF);
	drawList->AddRect(rectangleTopLeft, rectangleBottomRight, ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x7FFFFFFF);

	ImGui::End();


	if (invulnerable && (1000 * (clock() - invulnerableStart)/CLOCKS_PER_SEC) >= invulnerableDuration)
		invulnerable = false;

	//Check movement
	if (moveRight && !moveLeft)
		((Physics*)owner->GetComponent(PHYSICS))->SetXForce(moveSpeed);
	else if (moveLeft && !moveRight)
		((Physics*)owner->GetComponent(PHYSICS))->SetXForce(-moveSpeed);
	else
		((Physics*)owner->GetComponent(PHYSICS))->SetXForce(0.0f);

	//Check jumping
	canJump = (((Physics*)owner->GetComponent(PHYSICS))->GetForces().y == 0.0f);

	CheckForOverlaps();
}

void PlayerScript::CheckForOverlaps()
{
	for (BaseObject* overlap : ((Physics*)owner->GetComponent(PHYSICS))->GetOverlaps("PlayerOverlap"))
	{
		if (overlap->HasTag("Key"))
		{
			hasKey = true;
			((Sprite*)overlap->GetComponent(SPRITE))->SetActive(false);
		}
		else if (overlap->HasTag("Door") && hasKey)
			EndLevel();
	}
}

void PlayerScript::EndLevel()
{
	ObjectManager::GetInstance()->SetActiveScene(ObjectManager::GetInstance()->GetCurrentScene()+1);
}














//Function to check for overlaps
//	Get overlaps from the physics component
//	Check what type of overlap it is
//		If it's a key set the key
//		Else if it's the end of level end the level
//		Could also make a pickup function to call on the object(?)