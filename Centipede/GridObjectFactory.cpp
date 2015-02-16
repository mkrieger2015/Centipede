#include "Game Components\TEAL\CommonElements.h"
#include "GridObjectFactory.h"
#include "GridObject.h"

std::stack<GridObject*> GridObjectFactory::recycledItems;
GridObjectFactory* GridObjectFactory::self= NULL;

GridObject* GridObjectFactory::CreateGridObject()
{
	//object pointer to be returned
	GridObject* m;

	//if there are no recycled objects...
	if ( recycledItems.empty() )
	{
		//create a new one and set it to be recylced when destroyed
		m = new GridObject();
		m->SetExternalManagement( RecycleGridObject );
	}
	else
	{
		//get an object from the recycling stack
		m = recycledItems.top();
		recycledItems.pop();
		m->RegisterToCurrentScene();  
	}
	//initialize the object (this may set default variable values, etc).
	m->Initialize();
	return m;
}

void GridObjectFactory::RecycleGridObject(GameObject* m)
{
	recycledItems.push( (GridObject*) m);
}

void GridObjectFactory::Empty()
{
	while ( ! recycledItems.empty() ) 
	{
		delete recycledItems.top();
		recycledItems.pop();
	}
}