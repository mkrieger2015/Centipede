// SceneManager.h
// Andre Berthiaume, June 2012
//
// Controls which scene is active and deals with storage/retrieval of past scenes

#ifndef _SceneManager
#define _SceneManager

// Forward declarations
class Scene;

class SceneManager
{
private:
	static SceneManager* mgrInstance;
	SceneManager(){};
	SceneManager( const SceneManager&);
	SceneManager operator=(const SceneManager&);

	static SceneManager& Instance()
	{
		if ( ! mgrInstance)
			mgrInstance = new SceneManager;

		return *mgrInstance;
	}

	static Scene* currentScene;
	static Scene* nextScene;

public:
	static void SetStartScene(Scene* sc);

	static void ProcessOneFrame();

	static Scene* GetCurrentScene();
	static void ChangeScene( Scene* sc);
};

#endif _SceneManager