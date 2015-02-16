#include "Game Components\TEAL\CommonElements.h"
#include "BulletFactory.h"
#include "Bullet.h"
#include "Blaster.h"
#include "PlayerManager.h"

std::stack<Bullet*> BulletFactory::recycledItems;

BulletFactory* BulletFactory::self= NULL;

Bullet* BulletFactory::CreateBullet(float xpos,float ypos)
{
	//object pointer to be returned
	Bullet* b;

	//if there are no recycled objects...
	if ( recycledItems.empty() )
	{
		//create a new one and set it to be recylced when destroyed
		b = new Bullet();
		b->SetExternalManagement( RecycleBullet );
	}
	else
	{
		//get an object from the recycling stack
		b = recycledItems.top();
		recycledItems.pop();
		b->RegisterToCurrentScene();  
	}
	//initialize the object (this may set default variable values, etc).
	b->Initialize(PlayerManager::ReturnCurrentPlayer(),xpos,ypos);

	return b;

}

void BulletFactory::RecycleBullet(GameObject* b)
{
	recycledItems.push( (Bullet*) b);
}


void BulletFactory::Empty()
{
	while ( ! recycledItems.empty() ) 
	{
		delete recycledItems.top();
		recycledItems.pop();
	}
}