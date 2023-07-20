#include "ResourceManager.h"

ResourceManager* ResourceManager::instance = NULL;

ResourceManager::ResourceManager()
{
	TTF_Init();
}

ResourceManager::~ResourceManager()
{
	//Used this for finding out how to iterate through a map: https://stackoverflow.com/questions/110157/how-to-retrieve-all-keys-or-values-from-a-stdmap-and-put-them-into-a-vector


	//Deleting maps
	for (std::map<const string, SDL_Surface*>::iterator i = loadedBitmaps.begin(); i != loadedBitmaps.end(); ++i)
	{
		delete (i->second);
	}
	loadedBitmaps.clear();

	for (std::map<const string, TTF_Font*>::iterator i = loadedFonts.begin(); i != loadedFonts.end(); ++i)
	{
		delete (i->second);
	}
	loadedFonts.clear();

	delete (this);
}

ResourceManager* ResourceManager::GetInstance()
{
	if (instance == NULL)
		instance = new ResourceManager();
	return instance;
}

SDL_Surface* ResourceManager::LoadBMP(string fileDirectory)
{
	if (loadedBitmaps.find(fileDirectory) == loadedBitmaps.end())
	{
		//Load bitmap file
		SDL_Surface* newBitmap = SDL_LoadBMP(fileDirectory.c_str());
		loadedBitmaps.insert(pair<string, SDL_Surface*>(fileDirectory, newBitmap));
		return newBitmap;
	}
	else
	{
		return loadedBitmaps.find(fileDirectory)->second;
	}
}

TTF_Font* ResourceManager::LoadFont(string fileDirectory, int fontSize)
{
	string temp = fileDirectory + to_string(fontSize);
	if (loadedFonts.find(temp) == loadedFonts.end())
	{
		TTF_Font* newFont = TTF_OpenFont((fileDirectory.c_str()), fontSize);
		loadedFonts.insert(pair<string, TTF_Font*>(temp, newFont));
		return newFont;
	}
	else
	{
		return loadedFonts.find(temp)->second;
	}
}

void ResourceManager::UnloadBitmap(string fileDirectory)
{
	delete(loadedBitmaps.find(fileDirectory)->second);
	loadedBitmaps.erase(fileDirectory);
}

void ResourceManager::UnloadFont(string fileDirectory)
{
	delete(loadedFonts.find(fileDirectory)->second);
	loadedFonts.erase(fileDirectory);
}