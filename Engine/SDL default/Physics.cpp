#include "Physics.h"
#include "Sprite.h"
#include "Transform.h"
#include "Vector.h"
#include "Time.h"

Physics::Physics(BaseObject* owner_, bool dynamic_, bool gravity_, CollisionType collisionType_)
{
	owner = owner_;
	if (owner->GetComponent(TRANSFORM) == nullptr || owner->GetComponent(SPRITE) == nullptr) //Check there is a sprite and transform component (required to calculate the collision box)
	{
		//Add error handling
	}
	UpdateCorners(); // Initially set the corners
	dynamic = dynamic_;
	hasGravity = gravity_;
	colliderType = collisionType_;
	type = PHYSICS;
}

void Physics::UpdateCorners()
{
	//Get the position of the parent object
	Transform* ownerTransform = (Transform*)(owner->GetComponent(TRANSFORM));
	Vector2f ownerGlobalPos = ownerTransform->GetGlobalPos();

	//Get the width and height of the sprite component
	Sprite* sprite = (Sprite*)(owner->GetComponent(SPRITE));
	Vector2f WidthHeight = sprite->GetWidthAndHeight();

	//Calculate the positions of each corner
	//This is assuming position is measured from the top left corner
	topLeft = ownerGlobalPos;
	topRight = { ownerGlobalPos.x + WidthHeight.x, ownerGlobalPos.y};
	bottomRight = { ownerGlobalPos.x + WidthHeight.x, ownerGlobalPos.y + WidthHeight.y };
	bottomLeft = { ownerGlobalPos.x, ownerGlobalPos.y + WidthHeight.y };


	top = topLeft.y;
	left = topLeft.x;
	bottom = bottomRight.y;
	right = topRight.x;

	centre = Vector2f(left + (0.5f * WidthHeight.x), top + (0.5f * WidthHeight.y));
}

/// <summary>
/// Top left, top right, bottom right, bottom left
/// </summary>
vector<Vector2f> Physics::GetCorners()
{
	vector<Vector2f> corners = { topLeft, topRight, bottomRight, bottomLeft };
	return corners;
}

/// <summary>
/// Top, right, bottom, left
/// </summary>
vector<float> Physics::GetSides()
{
	vector<float> sides = { top, right, bottom, left };
	return sides;
}



vector<BaseObject*> Physics::GetOverlaps(string tag)
{	
	//Get all objects
	//Check they have a physics component
	//if yes
	//	Update their corners
	// 	Check if they are colliding
	// 		Check tags (if relevant)
	//			Add to list
	
	vector<BaseObject*> overlappingObjects;

	UpdateCorners();

	for (BaseObject* currentObject : ObjectManager::GetInstance()->GetObjects())
	{
		if (currentObject != owner)
		{
			//Update the collision boxes of both sides of potential overlap

			Physics* currentPhysics = (Physics*)currentObject->GetComponent(PHYSICS);
			Sprite* currentSprite = ((Sprite*)currentObject->GetComponent(SPRITE));

			if (currentPhysics != nullptr && currentSprite != nullptr && currentPhysics->GetCollisionType() == OVERLAP && currentSprite->GetActive())
			{
				currentPhysics->UpdateCorners();

				//Get the collision boxes and check (using proof by contradiction) if there is an overlap
				vector<float> currentSides = currentPhysics->GetSides();
				if (!(left > currentSides[RIGHT] || right < currentSides[LEFT]
					|| top > currentSides[BOTTOM] || bottom < currentSides[TOP]))
				{
					//If necessary check the tags
					if (tag == "" || currentObject->HasTag(tag))
						overlappingObjects.push_back(currentObject);
				}
			}
		}
	}
	return overlappingObjects;
}

vector<BaseObject*> Physics::GetCollisions(string tag)
{
	//Get all objects
	//Check they have a physics component
	//if yes
	//	Update their corners
	// 	Check if they are colliding
	// 		Check tags (if relevant)
	//			Add to list
	vector<BaseObject*> collidingObjects;
	UpdateCorners();
	for (BaseObject* currentObject : ObjectManager::GetInstance()->GetObjects())
	{
		//Update the collision boxes of both sides of potential collision
		if (currentObject != owner)
		{
			Physics* currentPhysics = (Physics*)currentObject->GetComponent(PHYSICS);
			Sprite* currentSprite = ((Sprite*)currentObject->GetComponent(SPRITE));

			if (currentPhysics != nullptr && currentSprite != nullptr&& currentPhysics->GetCollisionType() == COLLISION && currentSprite->GetActive())
			{
				currentPhysics->UpdateCorners();

				//Get the collision boxes and check (using proof by contradiction) if there is a collision
				vector<float> currentSides = currentPhysics->GetSides();
				if (!(left >= currentSides[RIGHT] || right <= currentSides[LEFT]
					|| top >= currentSides[BOTTOM] || bottom <= currentSides[TOP]))
				{
					//If necessary check the tags
					if (tag == "" || currentObject->HasTag(tag))
						collidingObjects.push_back(currentObject);
				}
			}
		}
	}
	return collidingObjects;
}

void Physics::DrawGui()
{
	if (ImGui::TreeNodeEx("Physics", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (colliderType == COLLISION)
			ImGui::Text("Collision type: collision");
		else if (colliderType == OVERLAP)
			ImGui::Text("Collision type: overlap");
		ImGui::Checkbox("Dynamic", &dynamic);
		ImGui::Checkbox("Has Gravity", &hasGravity);
		ImGui::Text("Forces");
		ImGui::Text(("	x:" + to_string(forces.x)).c_str());
		ImGui::Text(("	y:" + to_string(forces.y)).c_str());
		ImGui::TreePop();
	}
}

void Physics::Update()
{
	if (dynamic)
	{
		if (hasGravity)
			forces.y = forces.y + gravity * MyTime::GetInstance()->getDeltaTime(); // Adds gravity incrimentally with delta time


		Transform* transform = (Transform*)owner->GetComponent(TRANSFORM);

		//Moves the object in x before checking for horizontal collisions, then moves in y and checks for vertical collisions
		//This is to make the collision handling itself easier
		transform->SetXPosition(transform->GetLocalPos().x + forces.x);
		for (BaseObject* collisionObject : GetCollisions("Platform"))
		{
			Physics* physCollisionObject = (Physics*)collisionObject->GetComponent(PHYSICS);
			if (centre.x > physCollisionObject->GetCentre().x)
			{
				//If there is a collision on the left of the object set the left of the object to the right of the object it collides with
				//This prevents clipping
				SetLeft(physCollisionObject->GetSides()[RIGHT]);
			}
			else
			{
				SetRight(physCollisionObject->GetSides()[LEFT]);
			}
			forces.x = 0;
	}


		transform->SetYPosition(transform->GetLocalPos().y + forces.y);
		for (BaseObject* collisionObject : GetCollisions("Platform"))
		{
			Physics* physCollisionObject = (Physics*)collisionObject->GetComponent(PHYSICS);
			if (centre.y > physCollisionObject->GetCentre().y)
			{
				//Same as before but with vertical collisions
				SetTop(physCollisionObject->GetSides()[BOTTOM]);
			}
			else
			{
				SetBottom(physCollisionObject->GetSides()[TOP]);
			}
			forces.y = 0;
		}
	}
}

void Physics::AddForce(Vector2f newForce)
{
	forces += newForce;
}

void Physics::SetTop(float value)
{
	Transform* transform = (Transform*)(owner->GetComponent(TRANSFORM));

	float change = value - bufferDistance - top;

	Vector2f currentPos = transform->GetLocalPos();
	transform->SetPosition(Vector2f(currentPos.x, currentPos.y + change));
}

void Physics::SetRight(float value)
{
	Transform* transform = (Transform*)(owner->GetComponent(TRANSFORM));
	Sprite* sprite = (Sprite*)(owner->GetComponent(SPRITE));

	float change = value - bufferDistance - right;

	Vector2f currentPos = transform->GetLocalPos();

	transform->SetPosition(Vector2f(currentPos.x + change, currentPos.y));

}

void Physics::SetBottom(float value)
{
	Transform* transform = (Transform*)(owner->GetComponent(TRANSFORM));
	Sprite* sprite = (Sprite*)(owner->GetComponent(SPRITE));

	float change = value - bufferDistance - bottom;

	Vector2f currentPos = transform->GetLocalPos();

	transform->SetPosition(Vector2f(currentPos.x, currentPos.y + change));
}

void Physics::SetLeft(float value)
{
	Transform* transform = (Transform*)(owner->GetComponent(TRANSFORM));

	float change = value + bufferDistance - left;

	Vector2f currentPos = transform->GetLocalPos();
	transform->SetPosition(Vector2f(currentPos.x + change, currentPos.y));

}
