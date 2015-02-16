#include "Game Components\TEAL\CommonElements.h"
#include "FleaFactory.h"
#include "Flea.h"


std::stack<Flea*> FleaFactory::recycledItems;

FleaFactory* FleaFactory::self= NULL;

Flea* FleaFactory::CreateFlea()
{
	//object pointer to be returned
	Flea* f;

	//if there are no recycled objects...
	if ( recycledItems.empty() )
	{
		//create a new one and set it to be recylced when destroyed
		f = new Flea();
		f->SetExternalManagement( RecycleFlea );
	}
	else
	{
		//get an object from the recycling stack
		f = recycledItems.top();
		recycledItems.pop();
		f->RegisterToCurrentScene();  
	}
	//initialize the object (this may set default variable values, etc).
	f->Initialize();
	return f;
}

void FleaFactory::RecycleFlea(GameObject* f)
{
	recycledItems.push( (Flea*) f);
}

void FleaFactory::Empty()
{
	while ( ! recycledItems.empty() ) 
	{
		delete recycledItems.top();
		recycledItems.pop();
	}
}