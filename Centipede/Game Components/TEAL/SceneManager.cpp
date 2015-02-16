// SceneManager.h
// Andre Berthiaume, June 2012
//
// Controls which scene is active and (in the future) deals with storage/retrieval of past scenes

#include "SceneManager.h"
#include "Scene.h"

using namespace std;

SceneManager* SceneManager::mgrInstance = NULL;
Scene* SceneManager::currentScene = NULL;
Scene* SceneManager::nextScene = NULL;

void SceneManager::SetStartScene(Scene* sc)
{ 
	SceneManager::currentScene = NULL; 
	SceneManager::nextScene = sc;
}

void SceneManager::ProcessOneFrame()
{
	// first check if there a scene change request.
	if ( currentScene != nextScene)
	{
		delete currentScene;
		currentScene = nextScene;
		currentScene->Initialize();
	}

	currentScene->ProcessOneFrame();
}

void SceneManager::ChangeScene( Scene* sc)
{
	nextScene = sc;
}

Scene* SceneManager::GetCurrentScene(){ return currentScene;}
