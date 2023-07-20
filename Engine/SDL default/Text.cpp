#include "Text.h"
#include "Transform.h"

Text::Text(BaseObject* owner_, string fileDirectory, string textContent, int fontSize, Vector2f offset_, SDL_Colour colour_)
{
	owner = owner_;
	type = TEXT;
	if (owner->GetComponent(TRANSFORM) == nullptr)
		delete this;
	else
	{
		textValue = textContent;
		font = ResourceManager::GetInstance()->LoadFont(fileDirectory, fontSize);
		offset = offset_;
		colour = colour_;
	}
}

Text::~Text()
{
	TTF_CloseFont(font);
}

void Text::Update()
{
	int texW = 0;
	int texH = 0;

	SDL_Surface* surface = nullptr;
	SDL_Texture* texture = nullptr;

	surface = TTF_RenderText_Solid(font, textValue.c_str(), colour);

	//Check the surface is created correctly then make the texture, with error messages if any part fails
	if (!surface)
	{
		printf("Surface for font not loaded \n");
		printf("%s\n", SDL_GetError());
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(ScreenManager::GetInstance()->GetRendererReference(), surface);
		if (!texture)
		{
			printf("Surface for font not loaded \n");
			printf("%s\n", SDL_GetError());
		}
		else
		{
			SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
			SDL_Rect textRect = { (int)(((Transform*)owner->GetComponent(TRANSFORM))->GetGlobalPos().x + offset.x), (int)(((Transform*)owner->GetComponent(TRANSFORM))->GetGlobalPos().y + offset.y),
						texW, texH };
			SDL_RenderCopy(ScreenManager::GetInstance()->GetRendererReference(), texture, NULL, &textRect);
		}
	}

	//Destroy the texture and surface after rendering
	if (texture)
		SDL_DestroyTexture(texture);
	if (surface)
		SDL_FreeSurface(surface);
}

void Text::DrawGui()
{
	if (ImGui::TreeNodeEx("Text", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(("Text: " + textValue).c_str());
		ImGui::TreePop();
	}
}
