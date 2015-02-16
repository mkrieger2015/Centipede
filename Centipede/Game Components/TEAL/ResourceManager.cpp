// ResouceManager.cpp
// Andre Berthiaume, June 2012
//
// Overall resource manager

#include "ResourceManager.h"
#include "SceneManager.h"
#include "ConsoleMsg.h"
#include "TEALShow.h"

using namespace std;

std::string ResourceManager::RESOURCE_PATH = "resources/";

std::string ResourceManager::GetBasePath(){ return RESOURCE_PATH; }

void ResourceManager::SetBasePath(std::string path)
{ 
	if ( path[ path.length() - 1] != '/' ) path = path + "/";
	RESOURCE_PATH = path; 
}


ResourceManager* ResourceManager::resmgrInstance = NULL;
std::map< std::string, sf::Image> ResourceManager::CollectionImage;
std::map< std::string, sf::Texture> ResourceManager::CollectionTexture;
std::map< std::string, CollisionTools::TextureBitmap> ResourceManager::CollectionBitmap;
std::map< std::string, sf::SoundBuffer> ResourceManager::CollectionSoundBuffer;
std::map< std::string, sf::Music* > ResourceManager::CollectionMusic;
std::map< std::string, sf::Font> ResourceManager::CollectionFont;

void ResourceManager::AddImage(std::string resKey, string path)
{
	AddItem<sf::Image>( CollectionImage, path, resKey);
}

sf::Image& ResourceManager::GetImage( std::string resKey )
{
	return GetItem<sf::Image>(CollectionImage, resKey);
}

void ResourceManager::AddSound(std::string resKey, std::string path)
{
	AddItem<sf::SoundBuffer>(CollectionSoundBuffer, path, resKey);
}

sf::SoundBuffer& ResourceManager::GetSound(std::string resKey)
{
	return GetItem<sf::SoundBuffer>(CollectionSoundBuffer, resKey);
}

void ResourceManager::AddFont( std::string resKey, std::string path )
{
	AddItem<sf::Font>(CollectionFont, path, resKey);
}

sf::Font& ResourceManager::GetFont( std::string resKey )
{
	return GetItem<sf::Font>(CollectionFont, resKey);
}

void ResourceManager::AddMusic(std::string resKey, std::string path)
{
	// NOTE: We can't use the AddItem method because sf::Music does NOT have loadFromFile...
	std::map< std::string, sf::Music*>::iterator it = CollectionMusic.find(resKey);
	//*
	if ( it != CollectionMusic.end() )
		ConsoleMsg::ShowError("Using resource key '" + resKey + "' multiple times for the same resource type.");
	else
	{
		sf::Music *musicPtr = new sf::Music;
		if ( !( musicPtr->openFromFile( RESOURCE_PATH + path)  ) )
			ConsoleMsg::ShowError("Failed to load resource key " + resKey + "' (" + path + ")");
		else
		{
			CollectionMusic[resKey] = musicPtr;
			ConsoleMsg::ShowStatus("Resource key '" + resKey + "' (" + path + ") successfully loaded");
		}
	}
	//*/
}

sf::Music* ResourceManager::GetMusic(std::string resKey)
{
	return GetItem<sf::Music*>(CollectionMusic, resKey);
}

void ResourceManager::AddTexture(std::string resKey, string path, bool CreateBitmap)
{
	AddItem<sf::Texture>( CollectionTexture, path, resKey);

	// automatically add the bitmap.
	if ( CreateBitmap )
		CollectionBitmap[resKey] = CollisionTools::ContructTextureBitmap( CollectionTexture[resKey].copyToImage() );

}

sf::Texture& ResourceManager::GetTexture( std::string resKey )
{
	return GetItem<sf::Texture>(CollectionTexture, resKey);
}

CollisionTools::TextureBitmap& ResourceManager::GetTextureBitmap( std::string resKey)
{
	return GetItem<CollisionTools::TextureBitmap>(CollectionBitmap, resKey);
}

void ResourceManager::SetSpriteTextureAndBitmap( std::string resKey, sf::Sprite &s, CollisionTools::TextureBitmap &b)
{
	s.setTexture( GetTexture(resKey) );
	b = GetTextureBitmap( resKey );
}


void ResourceManager::SetStartScene( Scene* sc){ SceneManager::SetStartScene(sc); }