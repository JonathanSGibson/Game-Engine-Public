//This is the old object system which I am keeping in the project for reference in making the same systems in the new system
//The new object system is handled by "BaseObject" and classes that inherit from it, which use a component based system
//The component based system means it is more modular and extendable and means there is less need for hard-coding stuff into the basic object class






//#pragma once
//#include "ObjectManager.h"
//#include "SDL.h"
//#include "Vector.h"
//#include <vector>
//
//class BaseEvent;
//
//class GameObject :
//    public ObjectManager
//{
//protected:
//    char* objectName;
//    SDL_Texture* sprite;
//    SDL_Surface* spriteFile;
//    Vector2f localPos;
//    ImU16 localRotation;
//    Vector2f scale;
//    ResourceManager* resourceManager;
//    ScreenManager* screenManager;
//    SDL_Renderer* m_renderer;
//    GameObject* parent;
//    std::vector<GameObject*> children;
//
//    void AddChild(GameObject* child) { children.push_back(child); };
//    void RemoveChild(GameObject*);
//    inline void RemoveParent() { parent->RemoveChild(this); parent = nullptr; }
//public:
//    GameObject(const char* objectName_, string spriteFileDirectory, int xStart, int yStart, ResourceManager* newResourceManager, ScreenManager* newScreenManager, RenderLayers startLayer = Background, bool useTransparency = false);
//    ~GameObject();
//
//    //Functions for in game updates every frame
//    void PreRender();
//    void Render();
//    void PostRender();
//
//
//    // GUI for editor
//    bool CheckClicked();
//    void DrawGui();
//    void OnHold();
//    char* GetName() { return objectName; }
//    SDL_Texture* GetTexturePointer() { return sprite; }
//
//    //Function for handling events
//    virtual void HandleEvent(BaseEvent*);
//
//    //Functions for setting up our objects
//    void SetPosition(float x, float y) { localPos.x = x; localPos.y = y; };
//    void SetPosition(Vector2f newPos) { localPos = newPos; };
//    void SetParent(GameObject* parent_);
//
//    //Utility functions
//    Vector2f GetGlobalPos();
//    Vector2f GetLocalPos() { return localPos; };
//
//};