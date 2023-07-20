#pragma once
#include "BaseComponent.h"

class Sprite : public BaseComponent
{
protected:
	/// The layer the sprite is renderered on
	/// The higher the value the later (and thus "closer" to the screen) the sprite is rendered
	int renderLayer;

	/// The SDL texture of the sprite
	SDL_Texture* texture;

	/// The SDL surface of the sprite
	SDL_Surface* surface;

	/// Whether the sprite is "active" or not
	bool active;
public:
	/// Constructor for the sprite component
	/// 
	/// Sets various required variables of the component based on the parameters
	/// @param owner_ The object that owns this component
	/// @param fileDirectory The file directory of the image to be loaded as the sprite
	/// @param renderLayer_ The layer the sprite will be rendered on, defaulted to 0
	/// @param useTransparency Whether or not the sprite will remove transparent (keyed as 255, 0, 255) backgrounds, defaulted to false
	/// @note Also outputs errors to the console if the texture or surface cannot be loaded correctly
	Sprite(BaseObject* owner_, const char* fileDirectory, int renderLayer_ = 0, bool useTransparency = false);

	/// Adds various information to the ImGui window
	/// 
	/// Outputs the renderLayer as an editable integer
	void DrawGui();

	/// Runs all the code which needs to be run every frame
	/// 
	/// Calls the Draw() function
	/// @note Whilst this function only calls one function that is to make it easier to call as all BaseComponents have an Update() function which can be called by the object manager
	void Update();

	/// Runs the code to render the sprite
	/// 
	/// Checks if the sprite is active and if so renders it at the position determined by the owners Transform component
	/// This is mostly handled by SDL
	void Draw();

	/// Gets the render layer
	/// 
	/// @returns returns an integer of the renderLayer of the sprite
	int GetRenderLayer() { return renderLayer; }

	/// Sets the render layer
	/// 
	/// @param newLayer The layer to be set as the sprite's render layer
	void SetRenderLayer(int newLayer) { renderLayer = newLayer; }

	/// Gets the sprites texture
	/// 
	/// @returns The SDL_Texture pointer of the sprite
	SDL_Texture* GetTexture() { return texture; }

	/// Check if the sprite has been clicked on
	/// 
	/// Used in the editor to click and drag game objects
	/// @returns Bool of whether it has been clicked or not
	bool CheckClicked();

	/// Sets whether the sprite is active or not
	/// 
	/// @param newActive The bool which determines whether to set it as active or inactive
	void SetActive(bool newActive) { active = newActive; }

	/// Get whether the sprite is active or not
	/// 
	/// @returns Bool of whether the sprite is active or not
	bool GetActive() { return active; }

	/// Gets the width and height of the sprite
	/// 
	/// @returns Vector2f of the width and height of the sprite
	Vector2f GetWidthAndHeight();
};

