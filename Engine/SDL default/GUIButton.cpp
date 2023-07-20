#include "GUIButton.h"
#include "EventManager.h"
#include "Sprite.h"
#include "ObjectManager.h"
#include "Transform.h"

GUIButton::GUIButton(BaseObject* owner_)
{
	owner = owner_;
	if (owner->GetComponent(SPRITE) == nullptr)
		delete this;
	else
	{
		EventManager::GetInstance()->AddListener("LMB", this);
		UpdateColliderBounds();
	}
}

bool GUIButton::CheckButtonPressed(Vector2f mousePos)
{
	if (!(left > mousePos.x || right < mousePos.x ||
		top > mousePos.y || bottom < mousePos.y))
		return true;
	return false;
}

void GUIButton::UpdateColliderBounds()
{
	Transform* transform = (Transform*)owner->GetComponent(TRANSFORM);
	Sprite* sprite = (Sprite*)owner->GetComponent(SPRITE);

	Vector2f topLeft = transform->GetGlobalPos();
	Vector2f widthHeight = sprite->GetWidthAndHeight();

	left = topLeft.x;
	top = topLeft.y;
	bottom = topLeft.y + widthHeight.y;
	right = topLeft.x + widthHeight.x;
}

void GUIButton::HandleEvent(BaseEvent* event_)
{
	//Checks the button is on the active scene, check the input type is left mouse button click and check that it's within the collider bounds
	if (owner->GetScene() == ObjectManager::GetInstance()->GetCurrentScene()) {
		if (event_->returnType() == "LMB") {
			LMBInput* LMBevent = (LMBInput*)event_;
			UpdateColliderBounds();
			if (CheckButtonPressed(LMBevent->GetMousePos1())) {
				ButtonPressed();
			}
		}
	}
}
