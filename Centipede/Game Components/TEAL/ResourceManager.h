// ResouceManager.cpp
// Andre Berthiaume, June 2012
//
// Overall resource manager

#ifndef _ResouceManager
#define _ResouceManager

#include "CollisionTools.h"
#include "SFML\Audio.hpp"

// forward Declarations
class Scene;

/// \brief Preloads and distributes external resources used in the game.
/// 
/// All external resources such as images, textures, sounds, musics must be preloaded in the game. The resources manager is teh central repository of
/// there resources for the duration of the game. All resource types have an associated 'add' method to load them in the manager. When needed (typically
/// in the constructor of a GameObject), the appropriate 'get' method is used to retreive a reference to the resource.
///  
///  This ensures that only one copy of any given resource is loaded in the game (a time consuming operation) and allows more lightweight object 
///  to be created and merely refer to these preloaded instances.
///  \ingroup RESOURCES
class ResourceManager
{
private:
	static std::string RESOURCE_PATH;
	static std::map< std::string, sf::Image> CollectionImage;
	static std::map< std::string, sf::Texture> CollectionTexture;
	static std::map< std::string, CollisionTools::TextureBitmap> CollectionBitmap;
	static std::map< std::string, sf::SoundBuffer> CollectionSoundBuffer;
	static std::map< std::string, sf::Music*> CollectionMusic;			// Note the Music*. Music are non-copy: http://en.sfml-dev.org/forums/index.php?topic=7731.0
	static std::map< std::string, sf::Font> CollectionFont;

	static ResourceManager* resmgrInstance;					// Singleton setup
	ResourceManager(){};									// Private constructor
	ResourceManager(const ResourceManager&);				// Prevent copy-construction
	ResourceManager& operator=(const ResourceManager&);		// Prevent assignment

	static ResourceManager& Instance()						// Access reference (all public methods will be static)
	{
		if (! resmgrInstance) 
			resmgrInstance = new ResourceManager;
		return *resmgrInstance;
	};

	/// \brief Adds the content of path to resourcemap. Processes possible errors and status
	/// conditions on console window.
	///
	/// \tparam T Generic type parameter of the resource.
	/// \param resourcemap The resourcemap.
	/// \param path		   Full pathname of the file.	
	template <class T>
	static void AddItem( std::map< std::string, T> &resourcemap, std::string &path, std::string resKey)
	{
		std::map< std::string, T>::iterator it = resourcemap.find(resKey);
		if ( it != resourcemap.end() )
			ConsoleMsg::ShowError("Using resource key '" + resKey + "' multiple times for the same resource type.");
		else
			if ( !( resourcemap[resKey].loadFromFile(RESOURCE_PATH + path)  ) )
				ConsoleMsg::ShowError("Failed to load resource key " + resKey + "' (" + path + ")");
			else
				ConsoleMsg::ShowStatus("Resource key '" + resKey + "' (" + path + ") successfully loaded");
	}

	// Retrieves a resource item path from resourcemap
	// Shows an error on console if path not found in resourcemap

	/// \brief Retrieves a resource item path from resourcemap Shows an error on console if path not
	/// found in resourcemap.
	///
	/// \tparam T Generic type parameter of the resource.
	/// \param resourcemap The resourcemap.
	/// \param path		   Full pathname of the associated file is used as a resource key.
	///
	/// \return The item&lt; t&gt;	
	template <class T>
	static T& GetItem( std::map< std::string, T> &resourcemap, std::string resKey)
	{
		std::map< std::string, T>::iterator it = resourcemap.find(resKey);
		if ( it == resourcemap.end() )
			ConsoleMsg::ShowError("Resource '" + resKey + "' not found in collection");
		
		return (it->second); 
	}

public:

	static std::string GetBasePath();
	static void SetBasePath(std::string path);

	/// \brief Loads all resources for the game. 
	/// 	   
	/// \ingroup RESOURCES 
	/// This methods is actually implemented in the file LoadAllResources.cpp by the user. All
	/// external resources used during the game must be previously loading in the game using an associated 'add' line here. Additionally, the
	/// starting scene is also specified using the SetStartScene function.
	/// 
	/// Example
	///		\code
	///		void ResourceManager::LoadAllResources()
	///		{
	///			SetStartScene(  new SpaceBallSplashScene() );
	///
	///			AddTexture("image1.jpg");
	///			AddTexture("Ship.png");
	///			AddTexture("Shot.png");
	///			AddTexture("Bug.png");;
	///			AddTexture("Goo.png");
	///		}
	///	    \endcode 
	///	 \note
	///	*IMPORTANT*: the path name for a resource is always relative the folder: '\<Project Folder\> \\ resources'
	///  \sa 
	///  - ResourceManager for the various resouces that can be added.
	///  - SetStartScene to correctly set the initial scene.
	static void LoadAllResources();

	/// \brief Adds an <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Image.php">sf::image</a>
	/// to the resouce manager. The path name serves as the resource key for later retrieval.
	///
	/// \param path Full pathname of the file.	
	/// \note
	///	*IMPORTANT*: the pathname for a resource is always relative the folder: '\<Project Folder\> \\ resources'
	static void AddImage(std::string resKey, std::string path);

	/// \brief Gets an <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Image.php">sf::image</a> 
	/// from the resource manager. The path name of the associate file serves as the resource key.
	///
	/// \param path path name of the associated file.
	/// \note
	///	*IMPORTANT*: the pathname for a resource is always relative the folder: '\<Project Folder\> \\ resources'
	///
	/// \return A reference to the <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Image.php">sf::image</a>.
	static sf::Image& GetImage( std::string resKey );

	static void AddSound(std::string resKey, std::string path);
	static sf::SoundBuffer& GetSound(std::string resKey);

	static void AddMusic(std::string resKey, std::string path);
	static sf::Music* GetMusic(std::string resKey);


	/// \brief Adds an <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Texture.php">sf::Texture</a> to the resource manager.  
	/// The path name serves as the resource key for later retrieval. 
	/// \note For every <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Texture.php">sf::Texture</a> loaded, 
	/// an associated CollisionTools::TextureBitmap resource will be created using the same key unless CreateBitmap is set to false. This CollisionTools::TextureBitmap resource will be required if
	/// processing precise collision checking on sprites.
	///
	/// \param path pathname of the texture file.	
	/// \param CreateBitmap determines if an associated bitmap will be created. Default = true;
	/// \note
	///	*IMPORTANT*: the path name for a resource is always relative the folder: '\<Project Folder\> \\ resources'
	static void AddTexture(std::string resKey, std::string path, bool CreateBitmap = true);

	/// \brief Gets a <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Texture.php">sf::Texture</a> from the resource manager. 
	///
	/// \param path pathname of the file.
	/// 
	///  \note
	///	*IMPORTANT*: the pathname for a resource is always relative the folder: '\<Project Folder\> \\ resources'
	///
	/// \return A reference to the <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Texture.php">sf::Texture</a>.
	static sf::Texture& GetTexture( std::string resKey);

	static void AddFont( std::string resKey, std::string path );
	static sf::Font& GetFont( std::string resKey );

	/// \brief Gets the CollisionTools::TextureBitmap assocaited with the texture using key 'path'. This CollisionTools::TextureBitmap resource
	/// will be required if using precise collision checking on sprites.
	///
	/// \param path pathname of the associated with the texture file.
	///
	/// \note
	///	*IMPORTANT*: the pathname for a resource is always relative the folder: '\<Project Folder\> \\ resources'
	/// \return a reference to the CollisionTools::TextureBitmap.
	static CollisionTools::TextureBitmap& GetTextureBitmap( std::string resKey);

	/// \brief Allows to set both the <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Sprite.php">sf::Sprite</a>'s 
	/// <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Texture.php">sf::Texture</a> and load the CollisionTools::TextureBitmap corresponding to the
	/// resource key 'path'.
	///
	/// \param path pathname of the file.
	/// \param s    The <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Sprite.php">sf::Sprite</a> to receive the <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Texture.php">sf::Texture</a>.
	/// \param b    The CollisionTools::TextureBitmap &amp; to to receive the associated bitmap.	
	/// 
	///  \note
	///	*IMPORTANT*: the pathname for a resource is always relative the folder: '\<Project Folder\> \\ resources'
	static void SetSpriteTextureAndBitmap( std::string resKey, sf::Sprite &s, CollisionTools::TextureBitmap &b);

	/// \brief Sets the initial scene for the game.
	///  		 
	/// \param sc The scene to set as the first scene.
	/// 
	/// Used in the LoadAllResources(), this function sets the initial scene for the game. 
	/// 
	/// \b Example:
	///		\code
	///		void ResourceManager::LoadAllResources()
	///		{
	///			SetStartScene(  new SpaceBallSplashScene() );
	///
	///			<... various resources added ... >
	///		}
	///		\endcode
	///
	///  \ingroup RESOURCES
	static void SetStartScene( Scene* sc);
};

#endif _ResouceManager