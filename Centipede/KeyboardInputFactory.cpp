#include "Game Components\TEAL\CommonElements.h"
#include "KeyboardInputFactory.h"
#include "KeyboardInput.h"
#include "Blaster.h"

std::stack<KeyboardInput*> KeyboardInputFactory::recycledItems;
KeyboardInputFactory* KeyboardInputFactory::self= NULL;

KeyboardInput* KeyboardInputFactory::CreateKeyboardInput(Blaster* blast)
{
	//object pointer to be returned
	KeyboardInput* k;

	//if there are no recycled objects...
	if ( recycledItems.empty() )
	{
		//create a new one and set it to be recylced when destroyed
		k = new KeyboardInput(blast);
		k->SetExternalManagement( RecycleKeyboardInput );
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

void KeyboardInputFactory::RecycleKeyboardInput(GameObject* k)
{
	recycledItems.push( (KeyboardInput*) k);
}

void KeyboardInputFactory::Empty()
{
	while ( ! recycledItems.empty() ) 
	{
		delete recycledItems.top();
		recycledItems.pop();
	}
}