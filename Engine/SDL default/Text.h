#pragma once
#include "BaseComponent.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "Vector.h"
class Text :
    public BaseComponent
{
private:
    TTF_Font* font;
    string textValue;
    SDL_Colour colour;
    Vector2f offset;
public:
    /// Constructor for the text component
    /// 
    /// Sets various required variables of the component based on the parameters
    /// @param owner_ The owner of the component
    /// @param fileDirectory The file directory of the font to be loaded
    /// @param textContent The actual contents of the text
    /// @param fontSize The size for the font to be loaded at
    /// @param offset_ The offset from the transform of the owner (So you can make it draw from somewhere other than the top left of the object), defaulted to {0,0}
    /// @param colour_ The colour of the text, defaulted to black
    Text(BaseObject* owner_, string fileDirectory, string textContent, int fontSize, Vector2f offset_ = {0, 0}, SDL_Colour colour_ = { (Uint8)0,(Uint8)0,(Uint8)0,(Uint8)255 });

    ///Closes the font of the text
    ~Text();

    /// Renders the text
    /// 
    /// Renders the text using SDL
    /// @note Outputs an error if the surface or texture cannot be created properly
    void Update();

    /// Outputs the GUI information
    /// 
    /// Outputs the text value to the ImGui window
    void DrawGui();
};

