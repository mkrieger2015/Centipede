#include "Game Components\TEAL\CommonElements.h"
#include "ExplosionEffectFactory.h"
#include "ExplosionEffect.h"

std::stack<ExplosionEffect*> ExplosionEffectFactory::recycledItems;
ExplosionEffectFactory* ExplosionEffectFactory::self= NULL;

ExplosionEffect* ExplosionEffectFactory::CreateExplosionEffect(float xpos, float ypos)
{
	//object pointer to be returned
	ExplosionEffect* e;

	//if there are no recycled objects...
	if ( recycledItems.empty() )
	{
		//create a new one and set it to be recylced when destroyed
		e = new ExplosionEffect();
		e->SetExternalManagement( RecycleExplosionEffect );
	}
	else
	{
		//get an object from the recycling stack
		e = recycledItems.top();
		recycledItems.pop();
		e->RegisterToCurrentScene();  
	}
	//initialize the object (this may set default variable values, etc).



	e->Initialize(xpos, ypos);
	return e;
}

void ExplosionEffectFactory::RecycleExplosionEffect(GameObject* e)
{
	recycledItems.push( (ExplosionEffect*) e);
}

void ExplosionEffectFactory::Empty()
{
	while ( ! recycledItems.empty() ) 
	{
		delete recycledItems.top();
		recycledItems.pop();
	}
}