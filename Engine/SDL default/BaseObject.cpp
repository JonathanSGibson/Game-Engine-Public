#include "BaseObject.h"
#include <iostream>
#include "Transform.h"
#include "BaseComponent.h"


BaseObject::BaseObject(const char* name_, int x, int y, int sceneCount)
{
	scene = sceneCount;
	objectName = name_;
	if (name_ != "SceneRoot")
	{
		ObjectManager::GetInstance()->AddObject(this, sceneCount);
		SetParent(ObjectManager::GetInstance()->GetSceneRoots()[sceneCount]);
	}
	AddComponent(new Transform(x, y, this));


}

void BaseObject::SetParent(BaseObject* parent_)
{
	if (parent_->scene == scene)
	{
		bool NonRecursive = true;
		BaseObject* currentObject = parent_;

		//checks that the parentage isn't recursive
		while (currentObject != ObjectManager::GetInstance()->GetSceneRoots()[scene])
		{
			if (currentObject == this)
			{
				NonRecursive = false;
				break;
			}

			currentObject = currentObject->parent;
		}

		//If non-recursive set the parentage, otherwise output an error
		if (NonRecursive)
		{
			if (parent != ObjectManager::GetInstance()->GetSceneRoots()[scene])
				RemoveParent();

			parent = parent_;
			parent->AddChild(this);
		}
		else
		{
			cout << "Uh oh, you did a whoopsie, your parentage is R E C U R S I V E" << endl;
		}
	}
}

void BaseObject::RemoveParent()
{
	if (parent != nullptr)
		parent->RemoveChild(this);
	parent = ObjectManager::GetInstance()->GetSceneRoots()[scene];
}

void BaseObject::AddComponent(BaseComponent* newComponent)
{
	
	if (newComponent->GetType() == SCRIPT)
		AddScript(newComponent);
	else
	{
		if (components[newComponent->GetType()] == nullptr)
			components[newComponent->GetType()] = newComponent;
	}
}

BaseComponent* BaseObject::GetComponent(ComponentType type)
{
	if (type < NUM_TYPES)
		if (components[type] != nullptr)
			return components[type];

	return nullptr;
}

void BaseObject::OnHold()
{
	//Adds the mouse movement to the position of the transform as it is dragged in editor
	Transform* transform = (Transform*)components[TRANSFORM];
	if (parent == nullptr) {
		transform->SetPosition(Vector2f(
			transform->GetLocalPos().x + ImGui::GetIO().MouseDelta.x,
			transform->GetLocalPos().y + ImGui::GetIO().MouseDelta.y));
	}
	else {
		Vector2f globalPos = transform->GetGlobalPos();
		globalPos.x += ImGui::GetIO().MouseDelta.x;
		globalPos.y += ImGui::GetIO().MouseDelta.y;

		transform->SetPosition(globalPos - ((Transform*)(parent->GetComponent(TRANSFORM)))->GetGlobalPos());
	}
}



void BaseObject::RemoveChild(BaseObject* child)
{
	for (int i = 0; i < children.size(); i++) {
		BaseObject* currentChild = children[i];
		if (currentChild == child)
			children.erase(children.begin() + i);
	}
}

void BaseObject::AddTag(string tag)
{
	bool alreadyExists = false;
	for (string tag_ : tags)
	{
		if (tag == tag_)
		{
			alreadyExists = true;
			break;
		}
	}

	if (!alreadyExists)
		tags.push_back(tag);
}


void BaseObject::RemoveTag(string tag)
{
	for (int i = 0; i < tags.size(); i++)
	{
		if (tags[i] == tag) {
			tags.erase(tags.begin() + i);
			break;
		}
	}

}

bool BaseObject::HasTag(string tag)
{
	for (string tag_ : tags)
	{
		if (tag_ == tag)
			return true;
	}
	return false;
}

void BaseObject::DrawGui()
{
	ImGui::Text(("Name: " + (string)objectName).c_str());

	//Cycle through every component, script and tag outputting them to the ImGui window
	if (ImGui::CollapsingHeader("Components", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (BaseComponent* component : components) {
			if (component != nullptr)
				component->DrawGui();
		}
	}
	if (scripts.size() > 0)
	{
		if (ImGui::CollapsingHeader("Scripts", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (BaseComponent* component : scripts) {
				if (component != nullptr)
					component->DrawGui();
			}
		}
	}
	if (tags.size() > 0)
	{
		if (ImGui::CollapsingHeader("Tags", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (string tag : tags)
			{
				ImGui::Text(tag.c_str());
			}
		}
	}
}

void BaseObject::DrawHierarchy(ImGuiTreeNodeFlags flags)
{
	//Code for dragging and dropping to change the hierarchy order
	bool open = ImGui::TreeNodeEx(objectName, flags, objectName);
	if (ImGui::IsItemClicked())
		ObjectManager::GetInstance()->SetSelectedObject(this);

	if (ObjectManager::GetInstance()->GetSelectedObject() == this && ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("_TREENODE", this, sizeof(BaseObject*));
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_TREENODE"))
		{
			if (ObjectManager::GetInstance()->GetSelectedObject() != nullptr)
				ObjectManager::GetInstance()->GetSelectedObject()->SetParent(this);
		}
		ImGui::EndDragDropTarget();
	}

	//Recursively outputs parentage to the hierarchy
	if (open)
	{
		for (BaseObject* child : children)
		{
			child->DrawHierarchy(flags);
		}
		ImGui::TreePop();
	}
}

void BaseObject::PreRender()
{
	for (BaseComponent* script : scripts)
	{
		script->Update();
	}

	if (components[PHYSICS] != nullptr)
		components[PHYSICS]->Update();
}

void BaseObject::Render()
{
	if (components[SPRITE] != nullptr)
		components[SPRITE]->Update();

	if (components[TEXT] != nullptr)
		components[TEXT]->Update();
}

void BaseObject::PostRender()
{

}

