#pragma once

#include "SDL.h"
class ScreenManager
{
private:
	static ScreenManager* instance;
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
public:
	/// Initialises the SDL sttuff to render the window
	/// 
	/// @notes Outputs errors to the console if anything does not initialise correctly
	/// @notes The width, height and start position are set in Constants.h
	ScreenManager();

	///Deconstructs the SDL renderer and window
	~ScreenManager();

	/// Sets the background game colour
	/// 
	///	@param r The red value of the colour from 0 to 255
	///	@param g The green value of the colour from 0 to 255
	///	@param b The blie value of the colour from 0 to 255
	void SetDisplayColour(int r, int g, int b);

	/// Clears the renderer
	/// 
	/// @note Call at the start of a frame to clear the draw calls from the previous 
	void ClearDisplay() { SDL_RenderClear(m_renderer); }

	/// Gets a reference to the SDL window
	/// 
	/// @returns Pointer to the SDL_Window
	SDL_Window* GetWindowReference() { return m_window; }

	/// Gets a reference to the SDL renderer
	/// 
	/// @returns Pointer to the SDL_Renderer
	SDL_Renderer* GetRendererReference() { return m_renderer; }

	/// Gets the instance of the ScreenManager
	/// 
	/// @returns A pointer to the ScreenManager instance
	/// @notes ScreenManager is a singleton so this should be called instead of creating a new one
	static ScreenManager* GetInstance();
};

