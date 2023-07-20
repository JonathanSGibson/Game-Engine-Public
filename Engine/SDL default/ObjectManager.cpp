 #include "ObjectManager.h"
#include "BaseObject.h"
#include "Sprite.h"
#include "EventManager.h"
#include "Profiler.h"
#include "Game.h"
#include "QuitListener.h"

ObjectManager* ObjectManager::instance = NULL;

std::vector<BaseObject*> ObjectManager::Objects;

ObjectManager::ObjectManager()
{
    currentScene = 0;
}


ObjectManager* ObjectManager::GetInstance()
{
    if (instance == NULL)
        instance = new ObjectManager();
    return instance;
}

void ObjectManager::UpdateAll()
{
    ProfileSample* preRenderSample = new ProfileSample("Pre Render");

	//Cycle through every object and call their PreRender function, sort them by render order, then call Render and PreRender
	for (int i = 0; i < Objects.size(); i++)
	{
		Objects[i]->PreRender();
	}
    preRenderSample->EndSample();

    //Sorts objects based on layer
	SortObjects();

    ProfileSample* renderSample = new ProfileSample("Render");
	for (int i = 0; i < Objects.size(); i++)
	{
		Objects[i]->Render();
	}
    renderSample->EndSample();

    ProfileSample* postRenderSample = new ProfileSample("Post Render");
	for (int i = 0; i < Objects.size(); i++)
	{
		Objects[i]->PostRender();
	}
    postRenderSample->EndSample();

    if (Game::GetInstance()->guiOn)
    {
        //gui information
        ProfileSample* objectGUISample = new ProfileSample("Object Inspector Information");
        for (int i = 0; i < Objects.size(); i++)
        {
            if (Objects[i]->GetComponent(SPRITE) != nullptr)
                if (((Sprite*)(Objects[i]->GetComponent(SPRITE)))->CheckClicked())
                    break;
        }

        if (!ImGui::GetIO().MouseDown[0])
            dragging = nullptr;

        ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        if (selected != nullptr)
            selected->DrawGui();
        ImGui::End();

        if (dragging != nullptr)
            dragging->OnHold();

        ImGui::Begin("Hierarchy", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        sceneRoots[currentScene]->DrawHierarchy(ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen);
        ImGui::End();
        objectGUISample->EndSample();
    }
}

void ObjectManager::PreRender()
{

}

void ObjectManager::Render()
{

}

void ObjectManager::PostRender()
{

}

void ObjectManager::DrawGui()
{

}

void ObjectManager::OnHold()
{
}

string ObjectManager::TypeToString(ComponentType type)
{
    string tempStr;
    switch (type)
    {
    case TRANSFORM:
        tempStr = "Transform";
        break;
    case SPRITE:
        tempStr = "Sprite";
        break;
    case TEXT:
        tempStr = "Text";
        break;
    case PHYSICS:
        tempStr = "Physics";
        break;
    }

    return tempStr;
}

void ObjectManager::SortObjects()
{
    vector<BaseObject*> renderable;
    vector<BaseObject*> unRenderable;
    for (BaseObject* object : Objects)
    {
        if (object->GetComponent(SPRITE) != nullptr)
            renderable.push_back(object);
        else
            unRenderable.push_back(object);
    }

    renderable = QuickSort(renderable, 0, renderable.size() - 1);

    for (BaseObject* object : renderable)
    {
        unRenderable.push_back(object);
    }

    Objects = unRenderable;
}

std::vector<BaseObject*> ObjectManager::QuickSort(vector<BaseObject*> input, int first, int last)
{
    if (first < last)
    {
        int pivotValue = ((Sprite*)(input[first]->GetComponent(SPRITE)))->GetRenderLayer();
        int leftPointer = first;
        int rightPointer = last;

        while (leftPointer < rightPointer)
        {
            while (((Sprite*)(input[leftPointer]->GetComponent(SPRITE)))->GetRenderLayer() <= pivotValue && leftPointer <= rightPointer && leftPointer < last)
            {
                leftPointer++;
            }
            while (((Sprite*)(input[rightPointer]->GetComponent(SPRITE)))->GetRenderLayer() >= pivotValue && rightPointer >= leftPointer && rightPointer > first)
            {
                rightPointer--;
            }
            if (leftPointer < rightPointer)
            {
                BaseObject* temp = input[leftPointer];
                input[leftPointer] = input[rightPointer];
                input[rightPointer] = temp;
            }
        }

        int pivot = rightPointer;
        BaseObject* temp1 = input[first];
        input[first] = input[pivot];
        input[pivot] = temp1;

        input = QuickSort(input, first, pivot - 1);
        input = QuickSort(input, pivot + 1, last);
    }
    return input;

}

BaseObject* ObjectManager::AddSceneRoot(int scene)
{
    BaseObject* newSceneRoot = new BaseObject("SceneRoot", 0, 0, scene);
    newSceneRoot->AddComponent(new QuitListener(newSceneRoot));
    sceneRoots.push_back(newSceneRoot);
    return newSceneRoot;
}

void ObjectManager::SetActiveScene(int sceneCount)
{
    //Create scenes up to the selected scene if it doesn't already exist
    while (scenes.size() < sceneCount + 1)
    {
        scenes.push_back(vector<BaseObject*>());
        scenes[scenes.size() - 1].push_back(AddSceneRoot(scenes.size() - 1));
    }

    //Set objects on previous frame inactive
    for (BaseObject* object : Objects)
    {
        Sprite* sprite = (Sprite*)object->GetComponent(SPRITE);
        if (sprite != nullptr)
            sprite->SetActive(false);
    }

    //Set objects to the objects of the new scene
    Objects = scenes[sceneCount];

    //Set objects on the new scene to be active
    for (BaseObject* object : Objects)
    {
        Sprite* sprite = (Sprite*)object->GetComponent(SPRITE);
        if (sprite != nullptr)
            sprite->SetActive(true);
    }
    currentScene = sceneCount;
}

void ObjectManager::AddObject(BaseObject* object, int sceneCount)
{
    while (scenes.size() < sceneCount+1)
    {
        scenes.push_back(vector<BaseObject*>());
        scenes[scenes.size() - 1].push_back(AddSceneRoot(scenes.size() - 1));
    }
    if (sceneCount == currentScene)
        Objects.push_back(object);

    scenes[sceneCount].push_back(object);
}


