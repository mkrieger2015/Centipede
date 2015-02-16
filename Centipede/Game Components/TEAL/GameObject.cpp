// GameObject.h
// Andre Berthiaume, June 2012
//
// Base gameobject class

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "CollisionInfoShape.h"
#include "CollisionInfoSprite.h"
#include "CollisionInfoAABBfromShape.h"
#include "CollisionInfoAABBfromSprite.h"

GameObject::GameObject()
{
	updateOrder = 0;
	drawOrder = 0;

	ManageExternally = false;
	ManagedDeleteOperation = NULL;

	colliderInfo = new CollisionInfo();
	RegisterToCurrentScene();

	toBeRemoved = false;
}

GameObject::~GameObject()
{
	delete colliderInfo;
	SceneManager::GetCurrentScene()->Alarms().ClearAllAlarms(*this);
};

void GameObject::RegisterToCurrentScene()
{
	SceneManager::GetCurrentScene()->RegisterGameObject(this);
	toBeRemoved = false;
}

void GameObject::SetExternalManagement(Scene::DeleteOperation f)
{
	ManageExternally = true;
	ManagedDeleteOperation = f;
	//SceneManager::GetCurrentScene()->DeregisterGameObject(this);
}

bool GameObject::GetExternalManagement()
{
	return ManagedDeleteOperation != NULL;
}

void GameObject::MarkForDestroy()
{
	if ( ! toBeRemoved)
	{
		toBeRemoved = true;
		Destroy(); 
		SceneManager::GetCurrentScene()->DeregisterGameObject(this);
	}
}

void GameObject::SetCollider( sf::Sprite &s, CollisionTools::TextureBitmap &b,  bool precision)
{
	delete colliderInfo;

	if (precision)
		colliderInfo = new CollisionInfoSprite(s,b);
	else
		colliderInfo = new CollisionInfoAABBfromSprite(s);
}

void GameObject::SetCollider(sf::Shape &s, bool precision)
{
	delete colliderInfo;

	if (precision)
		colliderInfo = new CollisionInfoShape(s);
	else
		colliderInfo = new CollisionInfoAABBfromShape(s);
}

void GameObject::SetCollider( )
{
	delete colliderInfo;
	colliderInfo = new CollisionInfo();
}

void GameObject::SetUpdateOrder(int n)
{ 
	SceneManager::GetCurrentScene()->ChangeUpdateOrder(this, n);
	updateOrder = n; 
}

void GameObject::SetDrawOrder(int n)
{
	SceneManager::GetCurrentScene()->ChangeDrawOrder(this, n);
	drawOrder = n; 
};

int GameObject::GetUpdateOrder(){ return updateOrder; };
int GameObject::GetDrawOrder(){ return drawOrder; };

void GameObject::SetAlarm(int AlarmNumber, float time)
{ 
	SceneManager::GetCurrentScene()->Alarms().SetAlarm( *this, AlarmNumber, time);
}

void GameObject::AddTime(int AlarmNumber, float time)
{ 
	SceneManager::GetCurrentScene()->Alarms().AddTime( *this, AlarmNumber, time); 
}

float GameObject::GetAlarm(int AlarmNumber)
{ 
	return SceneManager::GetCurrentScene()->Alarms().GetAlarm( *this, AlarmNumber); 
}

bool GameObject::AlarmActive(int AlarmNumber)
{ 
	return SceneManager::GetCurrentScene()->Alarms().AlarmActive( *this, AlarmNumber); 
}

void GameObject::AlarmCancel(int AlarmNumber)
{ 
	SceneManager::GetCurrentScene()->Alarms().AlarmCancel( *this, AlarmNumber); 
}

void GameObject::RegisterInput( BitFlags flagvalues)
{  
	SceneManager::GetCurrentScene()->InputEvents().Register(*this, flagvalues);  
}

void GameObject::DeregisterInput( BitFlags flagvalues)
{ 
	SceneManager::GetCurrentScene()->InputEvents().Deregister(*this, flagvalues); 
}