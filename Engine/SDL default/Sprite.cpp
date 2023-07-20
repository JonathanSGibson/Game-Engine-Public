#include "Sprite.h"
#include "Transform.h"
#include "Vector.h"

Sprite::Sprite(BaseObject* owner_, const char* fileDirectory, int renderLayer_, bool useTransparency)
{
	active = true;
	owner = owner_;
	type = SPRITE;
	surface = ResourceManager::GetInstance()->LoadBMP(fileDirectory);
	renderLayer = renderLayer_;

	if (!surface)
	{
		texture = nullptr;
		//bitmap not loaded? output the error
		printf("SURFACE for bitmap '%s' not loaded! \n", fileDirectory);
		printf("%s\n", SDL_GetError());
	}
	else
	{
		//if you are to use the transparency, going to assume
		//the colour key is magenta (R=255, g=0, b=255)
		if (useTransparency)
		{
			Uint32 colourKey = SDL_MapRGB(surface->format, 255, 0, 255);
			SDL_SetColorKey(surface, SDL_TRUE, colourKey);
		}

		//create the texture
		texture = SDL_CreateTextureFromSurface(ScreenManager::GetInstance()->GetRendererReference() , surface);
		if (!texture)
		{
			//texture not loaded? Output the error
			printf("TEXTURE for bitmap '%s' not loaded! \n", fileDirectory);
			printf(" % s\n", SDL_GetError());
		}
	}
}

void Sprite::DrawGui()
{
	if (ImGui::TreeNodeEx("Sprite", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Render Layer");
		ImGui::SameLine(); ImGui::InputInt("", &renderLayer, 1);

		ImGui::TreePop();
	}
}

void Sprite::Update()
{
	Draw();
}

void Sprite::Draw()
{
	if (active)
	{
		Transform* transform = (Transform*)(owner->GetComponent(TRANSFORM));
		if (transform != nullptr)
		{
			Vector2f temp = transform->GetGlobalPos();
			SDL_Rect newRect = { temp.x, temp.y, surface->w, surface->h };
			SDL_RenderCopy(ScreenManager::GetInstance()->GetRendererReference(), texture, NULL, &newRect);
		}
	}
}

bool Sprite::CheckClicked()
{
	//Gets the mouse positions, makes a rectangle from the sprite, then check if the mouse is within the rectangle
	SDL_Point mouseLocation;

	mouseLocation.x = ImGui::GetIO().MousePos.x;
	mouseLocation.y = ImGui::GetIO().MousePos.y;

	Vector2f tempPos = ((Transform*)(owner->GetComponent(TRANSFORM)))->GetGlobalPos();

	SDL_Rect spriteRect = { tempPos.x, tempPos.y, surface->w, surface->h };

	if (SDL_PointInRect(&mouseLocation, &spriteRect) && ImGui::GetIO().MouseClicked[0])
		{
				ObjectManager::GetInstance()->SetSelectedObject(owner);
				ObjectManager::GetInstance()->SetDraggingObject(owner);
				return true;
		}
		
		return false;
}

Vector2f Sprite::GetWidthAndHeight()
{
	Vector2f WidthHeight = { (float)surface->w, (float)surface->h };
	return WidthHeight;
}
