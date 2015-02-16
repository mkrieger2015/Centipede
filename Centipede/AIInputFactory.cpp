#include "Game Components\TEAL\CommonElements.h"
#include "AIInputFactory.h"
#include "AIInput.h"
#include "Blaster.h"

std::stack<AIInput*> AIInputFactory::recycledItems;
AIInputFactory* AIInputFactory::self= NULL;

AIInput* AIInputFactory::CreateAIInput(Blaster* blast)
{
	//object pointer to be returned
	AIInput* k;

	//if there are no recycled objects...
	if ( recycledItems.empty() )
	{
		//create a new one and set it to be recylced when destroyed
		k = new AIInput(blast);
		k->SetExternalManagement( RecycleAIInput );
	}
	else
	{
		//get an object from the recycling stack
		k = recycledItems.top();
		recycledItems.pop();
		k->RegisterToCurrentScene();  
	}

	return k;
}

void AIInputFactory::RecycleAIInput(GameObject* k)
{
	recycledItems.push( (AIInput*) k);
}

void AIInputFactory::Empty()
{
	while ( ! recycledItems.empty() ) 
	{
		delete recycledItems.top();
		recycledItems.pop();
	}
}