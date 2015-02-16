#include "Game Components\TEAL\CommonElements.h"
#include "SpiderFactory.h"
#include "Spider.h"
#include "Blaster.h"


std::stack<Spider*> SpiderFactory::recycledItems;

SpiderFactory* SpiderFactory::self= NULL;

Spider* SpiderFactory::CreateSpider()
{
	//object pointer to be returned
	Spider* s;

	//if there are no recycled objects...
	if ( recycledItems.empty() )
	{
		//create a new one and set it to be recylced when destroyed
		s = new Spider();
		s->SetExternalManagement( RecycleSpider);
	}
	else
	{
		//get an object from the recycling stack
		s = recycledItems.top();
		recycledItems.pop();
		s->RegisterToCurrentScene();  
	}
	//initialize the object (this may set default variable values, etc).
	s->Initialize();
	return s;
}

void SpiderFactory::RecycleSpider(GameObject* s)
{
	recycledItems.push( (Spider*) s);
}

void SpiderFactory::Empty()
{
	while ( ! recycledItems.empty() ) 
	{
		delete recycledItems.top();
		recycledItems.pop();
	}
}