#pragma once
#include "ObjectManager.h"
#include "SDL.h"
#include "Vector.h"
#include <map>
#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <imgui_sdl.h>
#include <imgui_internal.h>

class BaseComponent;

class BaseObject :
	public ObjectManager
{
protected:
	/// The name of the object
	const char* objectName;

	/// Which scene the object is on
	int scene;

	/// Vector of all the attached scripts
	vector<BaseComponent*> scripts;

	/// Array of base components, the size of which is determined by the ComponentType enum in ObjectManager.h
	/// @note The ComponentType enum in ObjectManager.h should be used to navigate this
	BaseComponent* components[NUM_TYPES];

	/// Adds a script to the object
	/// 
	/// Pushes back a script to the scripts vector
	/// @param script The script to be pushed back
	/// @note This function is protected as it is called by AddComponent(BaseComponent*) and does not need to be called directly
	void AddScript(BaseComponent* script) { scripts.push_back(script); }

	/// The object's parent in the hierarchy
	BaseObject* parent;

	/// Vector of the object's children in the hierarchy
	vector<BaseObject*> children;

	/// Adds a child from the parent
	/// 
	/// Pushes back a BaseComponent pointer to the children vector
	/// @param child The BaseComponent pointer to be pushed to the children vector
	/// @note This function is protected as it is called by SetParent(BaseObject*)
	void AddChild(BaseObject* child) { children.push_back(child); }

	/// Removes a child from the object
	/// 
	/// Removes a child from the children vector
	/// @param child The child to be removed
	/// @note This function is protected as it is called by SetParent(BaseObject*)
	void RemoveChild(BaseObject* child);

	/// Removes the parent from the object
	/// 
	/// Removes the parent from the children vector
	/// @note This function is protected as it is called by SetParent(BaseObject*)
	void RemoveParent();

	/// The tags of the object
	/// 
	/// A vector of strings for the tags of the object
	/// @see An example of this being used is as the parameter of GetOverlaps(string tag) in the Physics class
	vector<string> tags;

public:
	/// Constructor for the base component
	/// 
	/// Sets up various variables required for the object
	/// Adds the transform component
	/// @param name_ The name of the object
	/// @param x The X position of the object to be given to the Transform component, defaulted to 0
	/// @param y The Y position of the object to be given to the Transform component, defaulted to 0
	/// @param sceneCount The scene for the object to be created on, defaulted to 0
	BaseObject(const char* name_, int x = 0, int y = 0, int sceneCount = 0);


	/// Add a tag to the object
	/// 
	/// Pushes back a string to the tag vector as long as the vector does not already contain it
	/// @param tag The tag to be added
	void AddTag(string tag);

	/// Removes a tag
	/// 
	/// Removes the given string from the tags vector if it is present
	/// @param tag The tag to be removed
	void RemoveTag(string tag);

	/// Checks if the object has a specific tag
	/// 
	/// Checks if the given tag is present in the tags vector
	/// @param tag The tag to check
	/// @returns A bool of whether the tag is present or not
	bool HasTag(string tag);



	/// Outputs various elements to the ImGui window
	/// 
	/// Outputs the following information to the ImGui window:
	///		- The name of the object
	///		- Each component and their information (As determined by their DrawGui() functions)
	///		- Each script and their information (As determined by their DrawGui() functions)
	///		- Each tag
	void DrawGui();

	/// Outputs the hierarchy to the ImGui window
	/// 
	/// Recursively displays every child of the object, with each level being an ImGui tree node
	/// @param flags The flags for each of the tree nodes
	/// @note If called from the root node of a scene it will display the entire hierarchy of the scene
	void DrawHierarchy(ImGuiTreeNodeFlags flags);

	/// Code to run every frame before rendering
	/// 
	/// Calls the update of every script and the physics component (if present)
	void PreRender();

	/// Code to render the object
	/// 
	/// Calls the update of the sprite and text components (if present) in order
	void Render();

	/// Code to run every frame after rendering
	/// 
	/// Currently empty but set up for the sake of extendability
	void PostRender();

	/// Set the parent
	/// 
	/// Checks the new parentage would not be recursive and checks that the new parent and this object are on the same scene
	/// If parenting is safe removes the previous parent, removes this from the previous parents children, sets this objects parent and adds this to the new parents children
	/// <param name=""></param>
	/// @note Utilises RemoveParent(), RemoveChild(BaseObject* child) and AddChild(BaseObject* child)
	void SetParent(BaseObject*);

	/// Gets the objects parent
	/// 
	/// @returns BaseObject pointer to the objects parent
	BaseObject* GetParent() { return parent; }

	/// Adds a component to the object
	/// 
	/// Checks if the component is a script and if so calls AddScript(BaseComponent* script), if not checks if a component of the same type already exists if not adds this as the component
	/// @param newComponent The component to be added
	void AddComponent(BaseComponent* newComponent);

	/// Gets a component
	/// 
	/// Gets a component of a specified type, if it exists
	/// @param type The type of the component to be returned
	/// @returns Either a pointer to the specified component or nullptr if it has not been added
	BaseComponent* GetComponent(ComponentType type);

	/// Function to drag object in the editor
	/// 
	/// Alters the transform of the object based on mouse movements whilst the object is being click and dragged in the editor
	void OnHold();

	/// Gets all the scripts
	/// 
	/// @returns Returns the scripts vector
	vector<BaseComponent*> GetScripts() { return scripts; }

	/// Gets the objects name
	/// 
	/// @returns The name of the object
	std::string GetName() { return objectName; }

	/// Gets the scene the object is on
	/// 
	/// @returns Returns the int value of the scene the object is on
	int GetScene() { return scene; }
};