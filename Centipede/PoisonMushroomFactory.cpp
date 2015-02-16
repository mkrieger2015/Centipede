#include "Game Components\TEAL\CommonElements.h"
#include "PoisonMushroomFactory.h"
#include "PoisonMushroom.h"

std::stack<PoisonMushroom*> PoisonMushroomFactory::recycledItems;
PoisonMushroomFactory* PoisonMushroomFactory::self= NULL;

PoisonMushroom* PoisonMushroomFactory::CreatePoisonMushroom(float xpos,float ypos)
{
	//object pointer to be returned
	PoisonMushroom* m;

	//if there are no recycled objects...
	if ( recycledItems.empty() )
	{
		//create a new one and set it to be recylced when destroyed
		m = new PoisonMushroom();
		m->SetExternalManagement( RecyclePoisonMushroom );
	}
	else
	{
		//get an object from the recycling stack
		m = recycledItems.top();
		recycledItems.pop();
		m->RegisterToCurrentScene();  
	}

	//initialize the object (this may set default variable values, etc).
	m->Initialize(xpos,ypos);
	return m;
}

PoisonMushroom* PoisonMushroomFactory::ConvertPoisonMushroom(float xpos, float ypos, int prev_health)
{
	PoisonMushroom* m;

	if ( recycledItems.empty() )
	{
		m = new PoisonMushroom();
		m->SetExternalManagement( RecyclePoisonMushroom );
	}
	else
	{
		m = recycledItems.top();
		recycledItems.pop();
		m->RegisterToCurrentScene();  
	}

	m->InitializeDamaged(xpos,ypos,prev_health);
	return m;
}

void PoisonMushroomFactory::RecyclePoisonMushroom(GameObject* m)
{
	recycledItems.push( (PoisonMushroom*) m);
}

void PoisonMushroomFactory::Empty()
{
	while ( ! recycledItems.empty() ) 
	{
		delete recycledItems.top();
		recycledItems.pop();
	}
}