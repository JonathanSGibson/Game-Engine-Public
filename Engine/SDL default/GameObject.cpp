//This is the old object system which I am keeping in the project for reference in making the same systems in the new system
//The new object system is handled by "BaseObject" and classes that inherit from it, which use a component based system
//The component based system means it is more modular and extendable and means there is less need for hard-coding stuff into the basic object class







//#include "GameObject.h"
///// <param name="xStart"> - Local start x</param>
///// <param name="yStart"> - Local start y</param>
///// <param name="newResourceManager"> - Pointer to the ResourceManager used by the object</param>
///// <param name="newScreenManager">- Pointer to the ScreenManager used by the object</param>
///// <param name="startLayer"> - Rendering layer of the object</param>
//GameObject::GameObject(const char* objectName_, string spriteFileDirectory, int xStart, int yStart, ResourceManager* newResourceManager, ScreenManager* newScreenManager, RenderLayers startLayer, bool useTransparency)
//{
//	//objectName = (char*)objectName_;
//	//renderOrder = startLayer;
//	//resourceManager = newResourceManager;
//	//screenManager = newScreenManager;
//	//localPos.x = xStart;
//	//localPos.y = yStart;
//	//m_renderer = screenManager->GetRendererReference();
//	//spriteFile = resourceManager->LoadBMP(spriteFileDirectory);
//	//parent = nullptr;
//
//	//scale.x = 1.0f;
//	//scale.y = 1.0f;
//	//localRotation = 0;
//
//
//	//if (!spriteFile)
//	//{
//	//	sprite = nullptr;
//	//	//bitmap not loaded? output the error
//	//	printf("SURFACE for bitmap '%s' not loaded! \n", spriteFileDirectory.c_str());
//	//	printf("%s\n", SDL_GetError());
//	//}
//	//else
//	//{
//	//	//if we are to use the transparency, going to assume
//	//	//the colour key is magenta (R=255, g=0, b=255)
//	//	if (useTransparency)
//	//	{
//	//		Uint32 colourKey = SDL_MapRGB(spriteFile->format, 255, 0, 255);
//	//		SDL_SetColorKey(spriteFile, SDL_TRUE, colourKey);
//	//	}
//
//	//	//create the texture
//	//	sprite = SDL_CreateTextureFromSurface(m_renderer, spriteFile);
//	//	if (!sprite)
//	//	{
//	//		//texture not loaded? Output the error
//	//		printf("TEXTURE for bitmap '%s' not loaded! \n", spriteFileDirectory.c_str());
//	//		printf(" % s\n", SDL_GetError());
//	//	}
//	//}
//}
//
//void GameObject::Render()
//{
//	//Need to add error checking
//	if (sprite)
//	{
//		Vector2f temp = GetGlobalPos();
//		SDL_Rect newRect = { temp.x, temp.y, spriteFile->w, spriteFile->h };
//		SDL_RenderCopy(m_renderer, sprite, NULL, &newRect);
//	}
//}
//
//GameObject::~GameObject()
//{
//	if (sprite)
//		SDL_DestroyTexture(sprite);
//	/*if (spriteFile)
//		SDL_FreeSurface(spriteFile);*/
//}
//
//void GameObject::PreRender()
//{
//	//Do movement and collision stuff
//} 
//
//void GameObject::PostRender()
//{
//
//}
//
//bool GameObject::CheckClicked()
//{
//	SDL_Point mouseLocation;
//	mouseLocation.x = ImGui::GetIO().MousePos.x;
//	mouseLocation.y = ImGui::GetIO().MousePos.y;
//
//	Vector2f tempPos = GetGlobalPos();
//
//	SDL_Rect spriteRect = { tempPos.x, tempPos.y, spriteFile->w, spriteFile->h };
//
//	if (SDL_PointInRect(&mouseLocation, &spriteRect) && ImGui::GetIO().MouseClicked[0])
//	{
//		ObjectManager::GetInstance()->SetSelectedObject(this);
//		ObjectManager::GetInstance()->SetDraggingObject(this);
//		return true;
//	}
//
//	return false;
//}
//
//void GameObject::OnHold()
//{
//	if (parent != nullptr)
//	{
//		Vector2f globalPos = GetGlobalPos();
//		globalPos.x += ImGui::GetIO().MouseDelta.x;
//		globalPos.y += ImGui::GetIO().MouseDelta.y;
//
//		localPos = globalPos - parent->GetGlobalPos();
//	}
//	else
//	{
//		localPos.x += ImGui::GetIO().MouseDelta.x;
//		localPos.y += ImGui::GetIO().MouseDelta.y;
//	}
//
//
//	//add mouse movement to position
//}
//
//void GameObject::DrawGui()
//{
//	////display the gui information
//	//ImGui::Begin("Do do do do do Inspector Gui");
//	//ImGui::InputText("Name", objectName, IM_ARRAYSIZE(objectName));
//
//	//if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_None))
//	//{
//	//	static bool inputs_step = true;
//	//	const float test = 1.f;
//	//	const ImU16 minRotation = 0;
//	//	const ImU16 maxRotation = 365;
//
//	//	ImGui::Text("Position (Local)");
//	//	ImGui::Text("X");
//	//	ImGui::SameLine();
//	//	ImGui::InputFloat("", &localPos.x, 0.1f, 1.0f);
//	//	ImGui::Text("Y");
//	//	ImGui::SameLine();
//	//	ImGui::InputFloat(" ", &localPos.y, 0.1f, 1.0f);
//
//	//	ImGui::Text("\nRotation (Please note - currently not implemented)");
//	//	ImGui::Text(" ");
//	//	ImGui::SameLine();
//	//	ImGui::SliderScalar("    ", ImGuiDataType_U16, &localRotation, &minRotation, &maxRotation, "%d");
//
//
//	//	ImGui::Text("\nScale (Please note - currently not implemented)");
//	//	ImGui::Text("X");
//	//	ImGui::SameLine();
//	//	ImGui::InputFloat("  ", &scale.x, 0.1f, 1.0f);
//	//	ImGui::Text("Y");
//	//	ImGui::SameLine();
//	//	ImGui::InputFloat("   ", &scale.y, 0.1f, 1.0f);
//	//	}
//
//	//ImGui::Text("Render Layer");
//	////This is bad and hardcoded, not sure how to fix currently, will look into
//	//const char* layers[LayerCount] = { "Background", "Lasers", "Obstacles", "Enemies", "Player", "UI" };
//	//const char* combo_preview_value = layers[renderOrder];
//	//if (ImGui::BeginCombo("               ", combo_preview_value))
//	//{
//	//	for (int n = 0; n < IM_ARRAYSIZE(layers); n++)
//	//	{
//	//		const bool is_selected = (renderOrder == n);
//	//		if (ImGui::Selectable(layers[n], is_selected))
//	//			renderOrder = (RenderLayers)n;
//
//	//		// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
//	//		if (is_selected)
//	//			ImGui::SetItemDefaultFocus();
//	//	}
//	//	ImGui::EndCombo();
//	//}
//	//
//	////render layer - drop down
//	//ImGui::End();
//}
//
//void GameObject::HandleEvent(BaseEvent*)
//{
//}
//
//void GameObject::SetParent(GameObject* parent_)
//{
//
//	bool NonRecursive = true;
//	GameObject* currentObject = parent_;
//
//	while (currentObject != nullptr)
//	{
//		if (currentObject == this)
//		{
//			NonRecursive = false;
//			break;
//		}
//
//		currentObject = currentObject->parent;
//	}
//
//	if (NonRecursive)
//	{
//		if (parent != nullptr)
//			RemoveParent();
//
//		parent = parent_;
//		parent->AddChild(this);
//		SetPosition(GetLocalPos() - parent->GetGlobalPos());
//	}
//	else
//	{
//		//output an error
//	}
//}
//
//Vector2f GameObject::GetGlobalPos()
//{
//	if (parent != nullptr)
//		return parent->GetGlobalPos() + localPos;
//	else
//		return localPos;
//}
//
//void GameObject::RemoveChild(GameObject* child)
//{
//	for (int i = 0; i < children.size(); i++) {
//		GameObject* currentChild = children[i];
//		if (currentChild == child) {
//			children.erase(children.begin() + i);
//		}
//	}
//}