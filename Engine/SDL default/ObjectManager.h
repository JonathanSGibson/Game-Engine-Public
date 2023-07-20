#pragma once
#include <vector>
#include "SDL.h"
#include "ResourceManager.h"
#include "ScreenManager.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <imgui_sdl.h>
#include <imgui_internal.h>

using namespace std;


static enum ComponentType
{
	TRANSFORM,
	SPRITE,
	TEXT,
	PHYSICS,
	NUM_TYPES,
	SCRIPT //Script is listed at the end as it is not included in the component array
	//This is because you can have multiple scripts but only one of each other type of component
};



class BaseObject;


class ObjectManager
{
private:
	static ObjectManager* instance;
	BaseObject* selected;
	BaseObject* dragging;
	//static BaseObject* sceneRoot;

	std::vector<std::vector<BaseObject*>> scenes;
	std::vector<BaseObject*> sceneRoots;
	int currentScene;
public:
	/// Sets the current scene to 0
	ObjectManager();

	/// Gets the instance of the ObjectManager
	/// 
	/// @returns A pointer to the ObjectManager instance
	/// @notes ObjectManager is a singleton so this should be called instead of creating a new one
	static ObjectManager* GetInstance();

	/// Vector of BaseObjects* in the current scene
	static std::vector<BaseObject*> Objects;

	/// Updates every object
	/// 
	/// Calls the prerender, render and postrender of every object in the current scene
	/// Checks the clicking and dragging of objects in the editor
	/// Outputs the information of the currently selected object
	void UpdateAll();

	/// Virtual PreRender so it can be called on the objects
	virtual void PreRender();

	/// Virtual Render so it can be called on the objects
	virtual void Render();

	/// Virtual PostRender so it can be called on the objects
	virtual void PostRender();

	/// Virtual DrawGui so it can be called on the objects
	virtual void DrawGui();

	/// Virtual OnHold so it can be called on the objects
	virtual void OnHold();

	/// Sets the currently selected object
	/// @param object The object to be set as the currently selected object
	void SetSelectedObject(BaseObject* object) { selected = object; }

	/// Gets the currently selected object
	/// 
	/// @returns BaseObject pointer to the selected object
	BaseObject* GetSelectedObject() { return selected; }

	/// Sets the dragging object
	/// 
	/// @param object BaseObject pointer to the object to drag
	void SetDraggingObject(BaseObject* object) { dragging = object; }

	/// Gets the current scene
	/// 
	/// @returns Returns an int of the current scene
	int GetCurrentScene() { return currentScene; }

	/// Converts the component type to string
	/// 
	/// @param type The type to convert
	/// @returns The type as a string
	string TypeToString(ComponentType type);

	/// Sorts the object in the currently selected scene
	/// 
	/// Sorts the object in the currently selected scene by their render layer (if they have a sprite component)
	void SortObjects();

	/// Quicksort
	/// 
	/// @param input Objects to sort
	/// @param first The first item to be sorted (As some items may not have sprite components)
	/// @param last The last item to be sorted (As some items may not have sprite components)
	/// @returns The sorted vector of objects
	std::vector<BaseObject*> QuickSort(vector<BaseObject*> input, int first, int last);

	/// Gets the scene roots
	/// 
	/// @returns Vector of all scene roots
	/// @note There are multiple scene roots as there is one per scene, this vector can be navigated using the scene number you wish to access
	std::vector<BaseObject*> GetSceneRoots() { return sceneRoots; }

	/// Adds a new scene root
	/// 
	/// @param scene The scene to add the root for
	/// @returns The new scene root
	BaseObject* AddSceneRoot(int scene);



	/// Gets the objects in the current scene
	/// 
	/// @returns Vector of every object in the current scene
	vector<BaseObject*> GetObjects() { return Objects; }

	/// Sets the active scene
	/// 
	/// Updates the current scene int
	/// Update Objects to be the objects in the current scene
	/// @param sceneCount The scene to be set as the active scene
	void SetActiveScene(int sceneCount);

	/// Adds an object to the scene
	/// 
	/// @param object The object to be added
	/// @param sceneCount The scene to add the object to
	/// @note This is called in the BaseObject constructor
	void AddObject(BaseObject* object, int sceneCount);
protected:
};
