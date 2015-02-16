#include "Game Components\TEAL\CommonElements.h"
#include "HeadFactory.h"
#include "Head.h"

std::stack<Head*> HeadFactory::recycledItems;

HeadFactory* HeadFactory::self= NULL;

void HeadFactory::CreateHead(float xpos, float ypos, bool initial_direction, int segments)
{
	//this creation function is to be used when a head is created from being spawned. heads that are created from a centipede split have their own creation function, below

	//object pointer to be returned
	Head* c;

	//if there are no recycled objects...
	if ( recycledItems.empty() )
	{
		//create a new one and set it to be recylced when destroyed
		c = new Head(xpos,ypos,initial_direction,segments);
		c->SetExternalManagement( RecycleHead );
	}
	else
	{
		//get an object from the recycling stack
		c = recycledItems.top();
		recycledItems.pop();
		c->RegisterToCurrentScene(); 
		//initialize the object (this may set default variable values, etc). NOTE: this initializer is only to be used for spawned heads, not from a split
		c->InitializeNew(xpos,ypos,initial_direction,segments);
	}
}

void HeadFactory::TransformHead(float xpos, float ypos, bool direction, std::queue<Centipede::orders> in_queue, std::queue<Centipede::orders> old_out, Centipede* new_next)
{
	//this creation function is to be used ONLY for centipede heads created via a split

	//object pointer to be returned
	Head* c;

	//if there are no recycled objects...
	if ( recycledItems.empty() )
	{
		//create a new one and set it to be recylced when destroyed
		c = new Head(xpos,ypos,direction,0);
		c->SetExternalManagement( RecycleHead );
	}
	else
	{
		//get an object from the recycling stack
		c = recycledItems.top();
		recycledItems.pop();
		c->RegisterToCurrentScene();  
	}

	//initialize the object (this may set default variable values, etc). NOTE: this initializer is only to be used for heads created via a split
	c->InitializeTransform(xpos,ypos,in_queue,old_out,new_next,direction);
}


void HeadFactory::RecycleHead(GameObject* c)
{
	recycledItems.push( (Head*) c);

}

void HeadFactory::Empty()
{
	while ( ! recycledItems.empty() ) 
	{
		delete recycledItems.top();
		recycledItems.pop();
	}
}
