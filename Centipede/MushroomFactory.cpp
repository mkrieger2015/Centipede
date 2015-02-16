#include "Game Components\TEAL\CommonElements.h"
#include "MushroomFactory.h"
#include "Mushroom.h"

std::stack<Mushroom*> MushroomFactory::recycledItems;
MushroomFactory* MushroomFactory::self= NULL;

Mushroom* MushroomFactory::CreateMushroom(float xpos,float ypos)
{
	//object pointer to be returned
	Mushroom* m;

	//if there are no recycled objects...
	if ( recycledItems.empty() )
	{
		//create a new one and set it to be recylced when destroyed
		m = new Mushroom();
		m->SetExternalManagement( RecycleMushroom );
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

void MushroomFactory::RecycleMushroom(GameObject* m)
{
	recycledItems.push( (Mushroom*) m);
}

void MushroomFactory::Empty()
{
	while ( ! recycledItems.empty() ) 
	{
		delete recycledItems.top();
		recycledItems.pop();
	}
}
