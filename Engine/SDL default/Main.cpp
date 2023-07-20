 #include "SDL.h"
#include "Game.h"
#include "GameObject.h"
#include <iostream>
#include "InputManager.h"
#undef main
#include "Time.h"
#include <string>
#include <filesystem>
#include "BaseObject.h"
#include "Sprite.h"
#include "Physics.h"
#include "Constants.h"
#include "PlayerScript.h"
#include "EventManager.h"
#include "EnemyScript.h"
#include "Profiler.h"
#include "Text.h"
#include "QuitButton.h"
#include "NextSceneButton.h"


struct FrameQueue
{
	vector<float> frames;
	int maxFrames = 300;

	void PushFrame(float newFrame)
	{
		frames.push_back(newFrame);
		int difference = frames.size() - maxFrames;
		for (int i = 0; i < difference; i++)
		{
			frames.erase(frames.begin());
		}
	}

	float GetAverage()
	{
		float average = 0;
		for (int i = 0; i < frames.size(); i++)
		{
			average += frames[i];
		}
		return average / frames.size();
	}
};

// 
// IMGUI CODE FOR GETTING SPRITES IN ASSET WINDOW
// 
struct IMGUISprite {
	Sprite* sprite;
	string name;

	IMGUISprite(string name_, Sprite* sprite_) {
		name = name_;
		sprite = sprite_;
	}
};

vector<IMGUISprite*> directoryContent;

void SearchDirectory()
{
	for (IMGUISprite* object : directoryContent)
	{
		delete(object);
	}
	directoryContent.clear();
	string DirectoryPath = "assets";
	for (const auto& entry : filesystem::directory_iterator(DirectoryPath)) {
		if (entry.path().extension() == ".bmp") {
			IMGUISprite* Asset = new IMGUISprite(entry.path().string().c_str(), new Sprite(nullptr, entry.path().string().c_str()));
			directoryContent.push_back(Asset);
		}
		else if (entry.is_directory()) {
			//cout << "dir " << entry.path().string().c_str() << endl;
		}
		//cout << entry.path() << endl;
	}
}




void main()
{
	//start up
	SDL_Init(SDL_INIT_EVERYTHING);

	Game* myGame = new Game();
	System* system = myGame->GetSystem();
	ScreenManager* screen = system->GetScreenManager();
	ObjectManager* objects = system->GetObjectManager(); // Put in system/game
	InputManager* inputs = system->GetInputManager(); // put in system/game


	//imGUI setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiSDL::Initialize(screen->GetRendererReference(), 900, 900);
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(screen->GetWindowReference(), nullptr);


	//scene creation
	{
		//Scene zero
		BaseObject* playButton = new BaseObject("Play Button", 300, 500, 0);
		playButton->AddComponent(new Sprite(playButton, "Assets/button.bmp"));
		playButton->AddComponent(new Text(playButton, "Assets/DejaVuSans.ttf", "Play", 60, Vector2f(75, 40)));
		playButton->AddComponent(new NextSceneButton(playButton));

		BaseObject* quitButton = new BaseObject("Quit Button", screenWidth - 600, 500, 0);
		quitButton->AddComponent(new Sprite(quitButton, "Assets/button.bmp"));
		quitButton->AddComponent(new Text(quitButton, "Assets/DejaVuSans.ttf", "Quit", 60, Vector2f(75, 40)));
		quitButton->AddComponent(new QuitButton(quitButton));

		BaseObject* title = new BaseObject("Title", 650, 200, 0);
		title->AddComponent(new Text(title, "Assets/DejaVuSans.ttf", "TITLE", 100, { 0,0 }, { (Uint8)255, (Uint8)255, (Uint8)255, (Uint8)255 }));

		//Scene one
		BaseObject* player = new BaseObject("Player", 15, 15, 1);
		player->AddComponent(new Sprite(player, "Assets/monstertrans.bmp", 50, true));
		player->AddComponent(new Physics(player, true, true));
		player->AddComponent(new PlayerScript(player, 5, 15, 15, 3, Vector2f(15, 15)));

		BaseObject* floor = new BaseObject("Floor", 0, screenHeight - 15, 1);
		floor->AddComponent(new Sprite(floor, "Assets/Floor.bmp"));
		floor->AddComponent(new Physics(floor, false, false));
		floor->AddTag("Platform");

		BaseObject* wall1 = new BaseObject("Wall 1", -90, 0, 1);
		wall1->AddComponent(new Sprite(wall1, "Assets/Wall.bmp"));
		wall1->AddComponent(new Physics(wall1, false, false));
		wall1->AddTag("Platform");

		BaseObject* wall2 = new BaseObject("Wall 2", screenWidth - 10, 0, 1);
		wall2->AddComponent(new Sprite(wall2, "Assets/Wall.bmp"));
		wall2->AddComponent(new Physics(wall2, false, false));
		wall2->AddTag("Platform");

		BaseObject* enemy1 = new BaseObject("Enemy 1", 400, 15, 1);
		enemy1->AddComponent(new Sprite(enemy1, "Assets/monster.bmp", 1));
		enemy1->AddComponent(new Physics(enemy1, true, true));
		enemy1->AddComponent(new EnemyScript(enemy1, 5, 400, 800, 1));

		BaseObject* key = new BaseObject("Key 1", screenWidth / 2, screenHeight - 200, 1);
		key->AddComponent(new Sprite(key, "Assets/key.bmp", 2, true));
		key->AddComponent(new Physics(key, false, false, OVERLAP));
		key->AddTag("PlayerOverlap");
		key->AddTag("Key");

		BaseObject* door = new BaseObject("Door 1", screenWidth - 200, screenHeight - 125, 1);
		door->AddComponent(new Sprite(door, "Assets/door.bmp", -2, true));
		door->AddComponent(new Physics(door, false, false, OVERLAP));
		door->AddTag("PlayerOverlap");
		door->AddTag("Door");

		//Scene two
		BaseObject* quitButton2 = new BaseObject("Quit Button (Endscreen)", (screenWidth / 2) - 150, 500, 2);
		quitButton2->AddComponent(new Sprite(quitButton2, "Assets/button.bmp"));
		quitButton2->AddComponent(new Text(quitButton2, "Assets/DejaVuSans.ttf", "Quit", 60, Vector2f(75, 40)));
		quitButton2->AddComponent(new QuitButton(quitButton2));

		BaseObject* endTitle = new BaseObject("End title", 550, 200, 2);
		endTitle->AddComponent(new Text(endTitle, "Assets/DejaVuSans.ttf", "you did it.", 100, { 0, 0 }, { (Uint8)255, (Uint8)255, (Uint8)255, (Uint8)255 }));
	}
	MyTime* timer = MyTime::GetInstance();
	Profiler* profiler = Profiler::GetInstance();


	//Variable setup
	FrameQueue frameQueue;

	SearchDirectory();

	screen->SetDisplayColour(100, 0, 100);

	IMGUISprite* contentBeingDragged = nullptr;

	string guiButtonText = "GUI Off";

	while (!Game::GetInstance()->GetQuit())
	{
		//Frame start info
		timer->StartFrame();
		profiler->StartFrame();

		SDL_RenderClear(screen->GetRendererReference());

		ImGui::NewFrame();

		//Input and even handling
		ProfileSample* inputSample = new ProfileSample("Input Polling");
		InputManager::GetInstance()->Update();
		inputSample->EndSample();

		ProfileSample* eventSample = new ProfileSample("Fire Events");
		EventManager::GetInstance()->FireEvents();
		eventSample->EndSample();

		//Updating all objects
		ProfileSample* objectsSample = new ProfileSample("Object Updates");
		objects->UpdateAll();
		objectsSample->EndSample();
		
		//GUI information
		ProfileSample* imGUISample = new ProfileSample("ImGUI");

		ImGui_ImplSDL2_NewFrame(screen->GetWindowReference());

		ImGui::Begin("Gui hider", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
		if (ImGui::Button(guiButtonText.c_str()))
		{
			Game::GetInstance()->guiOn = !Game::GetInstance()->guiOn;
		}
		ImGui::End();

		if (Game::GetInstance()->guiOn)
			guiButtonText = "GUI Off";
		else
			guiButtonText = "GUI On";
			

		if (Game::GetInstance()->guiOn)
		{
			ImGui::Begin("Performance", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			if (frameQueue.frames.size() > 0)
			{
				if (ImGui::CollapsingHeader("Framerate", ImGuiTreeNodeFlags_DefaultOpen))
				{
					float* frameQueueTemp = &frameQueue.frames[0];
					ImGui::PlotLines("Lines", frameQueueTemp, frameQueue.frames.size(), 1, "", 0.0f, 0.1f, ImVec2(0.0f, 120.0f));

					char buffer[40];
					snprintf(buffer, sizeof(buffer), "Average: %F seconds", frameQueue.GetAverage());
					ImGui::Text(buffer);
				}
			}
			profiler->DrawGui();
			ImGui::End();

			ImGui::Begin("Editor", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

			if (ImGui::Button("Refresh"))
				SearchDirectory();


			ImGui::BeginChild("Asset Window", ImVec2(), true);
			for (int i = 0; i < directoryContent.size(); i++)
			{
				ImGui::PushID(directoryContent[i]->name.c_str());

				ImGui::ImageButton((ImTextureID)directoryContent[i]->sprite->GetTexture(), { 100,100 });

				if (ImGui::BeginDragDropSource())
				{
					contentBeingDragged = directoryContent[i];
					ImGui::Image((ImTextureID)directoryContent[i]->sprite->GetTexture(), { 100,100 });
					ImGui::EndDragDropSource();
				}

				ImGui::PopID();
				if ((i + 1) % 10 != 0)
					ImGui::SameLine();
			}

			//Code to drag out assets from the asset window
			if (contentBeingDragged != nullptr && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			{
				int x;
				int y;
				SDL_GetMouseState(&x, &y);
				BaseObject* newObject = new BaseObject(contentBeingDragged->name.c_str(), x, y, ObjectManager::GetInstance()->GetCurrentScene());
				newObject->AddComponent(new Sprite(newObject, contentBeingDragged->name.c_str()));
				contentBeingDragged = nullptr;
			}

			ImGui::EndChild();
			ImGui::End();
		}

		//rendering information
		ImGui::Render();
		ImGuiSDL::Render(ImGui::GetDrawData());
		ImGui::EndFrame();
		imGUISample->EndSample();

		SDL_RenderPresent(screen->GetRendererReference());

		SDL_Delay(16);
		profiler->EndFrame();
		timer->EndFrame();

		frameQueue.PushFrame(timer->getDeltaTime());
	}
	SDL_Quit();
}