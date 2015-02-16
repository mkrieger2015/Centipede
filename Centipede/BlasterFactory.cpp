#include "Game Components\TEAL\CommonElements.h"
#include "BlasterFactory.h"
#include "Blaster.h"

std::stack<Blaster*> BlasterFactory::recycledItems;
BlasterFactory* BlasterFactory::self= NULL;

Blaster* BlasterFactory::CreateBlaster(bool mode)
{
	//object pointer to be returned
	Blaster* b;

	//if there are no recycled objects...
	if ( recycledItems.empty() )
	{
		//create a new one and set it to be recylced when destroyed
		b = new Blaster();
		b->SetExternalManagement( RecycleBlaster );
	}
	else
	{
		//get an object from the recycling stack
		b = recycledItems.top();
		recycledItems.pop();
		b->RegisterToCurrentScene();  
	}

	//initialize the object (this may set default variable values, etc).
	b->Initialize(mode);
	return b;
}

void BlasterFactory::RecycleBlaster(GameObject* b)
{
	recycledItems.push( (Blaster*) b);
}

void BlasterFactory::Empty()
{
	while ( ! recycledItems.empty() ) 
	{
		delete recycledItems.top();
		recycledItems.pop();
	}
}