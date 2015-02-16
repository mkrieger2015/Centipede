#include "Game Components\TEAL\CommonElements.h"
#include "BodyFactory.h"
#include "Centipede.h"
#include "Body.h"


std::stack<Body*> BodyFactory::recycledItems;

BodyFactory* BodyFactory::self= NULL;

Centipede* BodyFactory::CreateBody(float xpos, float ypos, int num_segments, Centipede *prev)
{
	//object pointer to be returned
	Body* b;

	//if there are no recycled objects...
	if ( recycledItems.empty() )
	{
		//create a new one and set it to be recylced when destroyed
		b = new Body(xpos,ypos,num_segments,prev);
		b->SetExternalManagement( RecycleBody );
	}
	else
	{
		//get an object from the recycling stack
		b = recycledItems.top();
		recycledItems.pop();
		b->RegisterToCurrentScene(); 
		b->Initialize(xpos,ypos,num_segments,prev);
	}
	
	return b;
}

void BodyFactory::RecycleBody(GameObject* b)
{
	recycledItems.push( (Body*) b);
}

void BodyFactory::Empty()
{
	while ( ! recycledItems.empty() ) 
	{
		delete recycledItems.top();
		recycledItems.pop();
	}
}