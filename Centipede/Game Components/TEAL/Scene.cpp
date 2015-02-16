// Scene.h
// Andre Berthiaume, Juen 2012
//
// Base scene class

#include "SceneManager.h"
#include "Scene.h"
#include "TEALShow.h"
#include "WindowManager.h"
#include "GameObject.h"
#include "Game.h"

using namespace std;

void Scene::UpdateOneGameObject( GameObject* go){ go->Update();}

void Scene::DrawOneGameObject( GameObject* go){ go->Draw();}

void Scene::OperateOnAllGameObjects( MapOfGameObjectLists& map, void (*f)(GameObject*) )
{
	MapOfGameObjectLists::iterator mapPair;
	GameObjectList::iterator listItem;

	for(mapPair = map.begin(); mapPair != map.end(); ++mapPair)
	{
		for( listItem = mapPair->second.begin(); listItem != mapPair->second.end(); ++listItem)
		{
			(*f)(*listItem);
		}
	}
}

Scene::~Scene()
{
	// Deleting all objects from update list is sufficient to clean up those in the other two lists
	OperateOnAllGameObjects( updateListMap, DeleteOneGameObject );
}

// Default operation when cleaning up an unmanaged GameObjects marked for destruction
void Scene::DeleteGameObject(GameObject* go)
{
	delete go;
}

// Used by the ~Scene: calls Destroy on each remaining GameObject to allow proper housekeeping
void Scene::DeleteOneGameObject( GameObject* go)
{ 
	go->Destroy();
	FinalDeleteOrRelease(go);
}

// Either deletes a GameObject or uses its stated Managed operation
void Scene::FinalDeleteOrRelease( GameObject* go )
{
	if ( ! go->GetExternalManagement() ) 
	{
		DeleteGameObject( go );
	}
	else
	{
		(go->ManagedDeleteOperation)(go);
	}
}

void Scene::Initialize() {}

void Scene::RegisterGameObject( GameObject* go)
{
	updateListMap[ go->GetUpdateOrder() ].push_back( go );
	drawListMap[ go->GetDrawOrder()].push_back( go );
}

void Scene::DeregisterGameObject( GameObject* go)
{
	GameObjectstoBeRemoved[ go->GetUpdateOrder() ].push_back( go );
}

void  Scene::ProcessOneFrame()
{
	// Various events
	MyAlarmManager.ProcessTimeEvents();
	MyInputEventMgr.ProcessInputEvents();

	UpdateAllObjects();

	// Process collisions
	MyCollisionMgr.ProcessCollisionEvents();

	// draw
	WindowManager::Clear();
		// FPS display
		WindowManager::SetCaptionMessage( "FPS " + Tools::ToString( 1/Game::FrameTime() ));

		DrawAllObjects();
		TEALShow::Draw(); // Draw the debug markers, if any...
	WindowManager::Display();

	// Cleaning up the deceased...
	DeleteMarkedObjects();
}

AlarmManager& Scene::Alarms(){ return MyAlarmManager; }
InputEventManager& Scene::InputEvents(){ return MyInputEventMgr; }

void Scene::UpdateAllObjects()
{
	OperateOnAllGameObjects( updateListMap, UpdateOneGameObject );
} 

void Scene::DrawAllObjects()
{
	OperateOnAllGameObjects( drawListMap, DrawOneGameObject );
}

void Scene::DeleteMarkedObjects()
{
	OperateOnAllGameObjects( GameObjectstoBeRemoved, DeleteOneMarkedGameObject );

	GameObjectstoBeRemoved.clear();
}

void Scene::DeleteOneMarkedGameObject( GameObject* go)
{
	SceneManager::GetCurrentScene()->updateListMap[ go->GetUpdateOrder() ].remove(go);
	SceneManager::GetCurrentScene()->drawListMap[ go->GetDrawOrder() ].remove(go);

	FinalDeleteOrRelease(go);
}

void Scene::ChangeUpdateOrder(GameObject* go, int neworder)
{
	updateListMap[go->GetUpdateOrder()].remove(go);
	updateListMap[ neworder ].push_back( go );
}

void Scene::ChangeDrawOrder(GameObject* go, int neworder)
{
	drawListMap[go->GetDrawOrder()].remove(go);
	drawListMap[ neworder ].push_back( go );
}



