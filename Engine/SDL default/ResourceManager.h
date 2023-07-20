#pragma once
#include "SDL.h"
#include <map>
#include <string>
#include <SDL_ttf.h>
#include "SDL_audio.h"

// Used this for looking at SDL audio https://docs.huihoo.com/sdl/sdl-1.0-intro-en/usingsound.html

using namespace std;
class ResourceManager
{
private:
	static ResourceManager* instance;

	//map with all loaded files
	map<const string, SDL_Surface*> loadedBitmaps; //all images
	map<const string, TTF_Font*> loadedFonts; //all fonts
public:
	/// Initiates the the TTF library of SDL
	ResourceManager();

	///Cycles through each map of resources and deletes its contents
	~ResourceManager();

	/// Gets the instance of the ResourceManager
	/// 
	/// @returns A pointer to the ResourceManager instance
	/// @notes ResourceManager is a singleton so this should be called instead of creating a new one
	static ResourceManager* GetInstance();


	/// Loads a bitmap into the game
	/// 
	/// @param fileDirectory The directory of the image to load
	/// @returns An SLD_Surface pointer of the image which is usable in the enginge
	/// @note Used in the constructor of Sprite
	SDL_Surface* LoadBMP(string fileDirectory);

	/// Loads a font into the game
	/// 
	/// @param fileDirectory The directory of the font to load
	/// @returns A TTF_Font pointer of the image which is usable in the enginge
	/// @note Used in the constructor of Text
	TTF_Font* LoadFont(string fileDirectory, int fontSize = 15);

	/// Unloads a bitmap
	/// 
	/// Erases a bitmap from the map of bitmaps
	/// @param fileDirectory The directory of the bitmap to unload
	void UnloadBitmap(string fileDirectory);

	/// Unloads a font
	/// 
	/// Erases a font from the map of fonts
	/// @param fileDirectory The directory of the font to unload
	void UnloadFont(string fileDirectory);

};

