#include "Game Components\TEAL\CommonElements.h"
#include "ScorpionFactory.h"
#include "Scorpion.h"


std::stack<Scorpion*> ScorpionFactory::recycledItems;

ScorpionFactory* ScorpionFactory::self= NULL;

Scorpion* ScorpionFactory::CreateScorpion()
{
	//object pointer to be returned
	Scorpion* s;

	//if there are no recycled objects...
	if ( recycledItems.empty() )
	{
		//create a new one and set it to be recylced when destroyed
		s = new Scorpion();
		s->SetExternalManagement( RecycleScorpion );
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

void ScorpionFactory::RecycleScorpion(GameObject* s)
{
	recycledItems.push( (Scorpion*) s);
}

void ScorpionFactory::Empty()
{
	while ( ! recycledItems.empty() ) 
	{
		delete recycledItems.top();
		recycledItems.pop();
	}
}
