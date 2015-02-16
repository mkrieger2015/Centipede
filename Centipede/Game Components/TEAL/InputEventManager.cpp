// InputEventManager.cpp
// Andre Berthiaume, June 2012
//
// Processes inputs for keyboard, mouse, joystick, etc and dispatches the calls

#include "InputEventManager.h"
#include "WindowManager.h"
#include "GameObject.h"
#include "SFML\Window.hpp"

InputEventManager::GameObjectList InputEventManager::registeredGameObjects;
InputEventManager::GameObjectFlagMap InputEventManager::registrationMap;
InputEventManager::FlagListMap InputEventManager::DistpatchMap;

void InputEventManager::CallWindowResized(GameObject* go, sf::Event e)
{ 
	go->WindowResized(); 
}

void InputEventManager::CallWindowLostFocus(GameObject* go, sf::Event e)
{ 
	go->WindowLostFocus(); 
}

void InputEventManager::CallWindowGainedFocus(GameObject* go, sf::Event e)
{ 
	go->WindowGainedFocus(); 
}

void InputEventManager::CallWindowClosed(GameObject* go, sf::Event e)
{ 
	go->WindowClosed(); 
}

void InputEventManager::CallKeyPressed(GameObject* go, sf::Event e)
{ 
	go->KeyPressed( e.key.code, e.key.alt, e.key.control, e.key.shift, e.key.system);  
}

void InputEventManager::CallKeyReleased(GameObject* go, sf::Event e)
{ 
	go->KeyReleased( e.key.code, e.key.alt, e.key.control, e.key.shift, e.key.system); 
}

void InputEventManager::CallTextEntered(GameObject* go, sf::Event e)
{ 
	go->TextEntered( e.text.unicode ); 
}

void InputEventManager::CallMouseWheelMoved(GameObject* go, sf::Event e)
{ 
	go->MouseWheelMoved(e.mouseWheel.delta, e.mouseWheel.x, e.mouseWheel.y ); 
}

void InputEventManager::CallMouseButtonPressed(GameObject* go, sf::Event e)
{ 
	go->MouseButtonPressed(e.mouseButton.button, e.mouseButton.x, e.mouseButton.y ); 
}

void InputEventManager::CallMouseButtonReleased( GameObject* go, sf::Event e)
{ 
	go->MouseButtonReleased(e.mouseButton.button, e.mouseButton.x, e.mouseButton.y ); 
}

void InputEventManager::CallMouseMoved(GameObject* go, sf::Event e)
{ 
	go->MouseMoved( e.mouseMove.x, e.mouseMove.y ); 
}

void InputEventManager::CallMouseEntered(GameObject* go, sf::Event e)
{ 
	go->MouseEntered(); 
}

void InputEventManager::CallMouseLeft(GameObject* go, sf::Event e)
{
	go->MouseLeft();
}

void InputEventManager::CallJoystickConnected( GameObject* go, sf::Event e)
{ 
	go->JoystickConnected( e.joystickConnect.joystickId ); 
}

void InputEventManager::CallJoystickDisconnected( GameObject* go, sf::Event e)
{
	go->JoystickDisconnected( e.joystickConnect.joystickId ); 
}

void InputEventManager::CallJoystickButtonPressed( GameObject* go, sf::Event e)
{
	go->JoystickButtonPressed( e.joystickButton.joystickId, e.joystickButton.button ); 
}

void InputEventManager::CallJoystickButtonReleased( GameObject* go, sf::Event e)
{
	go->JoystickButtonReleased( e.joystickButton.joystickId, e.joystickButton.button ); 
}

void InputEventManager::CallJoystickMoved( GameObject* go, sf::Event e)
{
	go->JoystickMoved( e.joystickMove.joystickId, e.joystickMove.axis, e.joystickMove.position ); 
}

void InputEventManager::Register(GameObject& go, BitFlags flagvalues)
{ 
	GameObjectFlagMap::iterator it;
	BitFlags currentflags;

	it = registrationMap.find(&go);
	if (it != registrationMap.end() )
	{
		currentflags = it->second;
	}
	else
	{
		registrationMap[&go] = 0;	// assign a map entry with no flags. The following loop will add the right value in the end
		currentflags = 0;
	}

	// Adjust the flags and the dispatch lists
	for( BitFlags flag = (BitFlags)InputFlags::Resized; flag <= (BitFlags)InputFlags::TextEntered; flag = flag << 1 )
	{
		//ConsoleMsg::WriteLine("flag = " + Tools::ToString( flag) + "\tval = " + Tools::ToString(flag & ~currentflags & flagvalues));
		if ( flag & ~currentflags & flagvalues)  
		{
			DistpatchMap[ (InputFlags::Flags) flag ].push_back(&go);
			registrationMap[&go] += flag;
		}
	}
}


void InputEventManager::Deregister(GameObject& go, BitFlags flagvalues)
{ 
	GameObjectFlagMap::iterator it;

	it = registrationMap.find(&go);
	if (it != registrationMap.end() )
	{
		BitFlags currentflags = it->second;

		// Adjust the flags and the dispatch lists
		for( BitFlags flag = (BitFlags)InputFlags::Resized; flag <= (BitFlags)InputFlags::TextEntered; flag = flag << 1 )
		{
			if ( flag & currentflags & flagvalues)  
			{
				DistpatchMap[ (InputFlags::Flags) flag].remove(&go);
				it->second -= flag;
			}
		}

		// If no flags left, remove map entry altogether
		if ( it->second == 0) 
			registrationMap.erase(&go);
	}
}

void  InputEventManager::TraverseList( GameObjectList gol, sf::Event e, TraversalOperation f )
{
	for( GameObjectList::iterator listItem = gol.begin(); listItem != gol.end(); ++listItem)
		(*f)( *listItem, e);
}

void InputEventManager::ProcessInputEvents()
{
	sf::Event winEvent;

	while ( WindowManager::MainWindow.pollEvent(winEvent))
    {
		switch ( winEvent.type )
		{
			//case sf::Event::Closed: WindowManager::MainWindow.close(); 	break;

			// events to broadcast to registered GameObjects
			case sf::Event::Resized:				
				TraverseList( DistpatchMap[InputFlags::Resized], winEvent, CallWindowResized );	
				break;

			case sf::Event::LostFocus:
				TraverseList( DistpatchMap[InputFlags::LostFocus], winEvent, CallWindowLostFocus );
				break;

			case sf::Event::GainedFocus:
				TraverseList( DistpatchMap[InputFlags::GainedFocus], winEvent, CallWindowGainedFocus );
				break;

			case sf::Event::KeyPressed:				
				TraverseList( DistpatchMap[InputFlags::KeyPressed], winEvent, CallKeyPressed );
				break;

			case sf::Event::KeyReleased:			
				TraverseList( DistpatchMap[InputFlags::KeyReleased], winEvent, CallKeyReleased );
				break;

			case sf::Event::TextEntered:			
				TraverseList( DistpatchMap[InputFlags::TextEntered], winEvent, CallTextEntered );
				break;
			case sf::Event::MouseWheelMoved:		
				TraverseList( DistpatchMap[InputFlags::MouseWheelMoved], winEvent, CallMouseWheelMoved );
				break;

			case sf::Event::MouseButtonPressed:		
				TraverseList( DistpatchMap[InputFlags::MouseButtonPressed], winEvent, CallMouseButtonPressed );
				break;

			case sf::Event::MouseButtonReleased:	
				TraverseList( DistpatchMap[InputFlags::MouseButtonReleased], winEvent, CallMouseButtonReleased );
				break;

			case sf::Event::MouseMoved:				
				TraverseList( DistpatchMap[InputFlags::MouseMoved], winEvent, CallMouseMoved );
				break;

			case sf::Event::MouseEntered:			
				TraverseList( DistpatchMap[InputFlags::MouseEntered], winEvent, CallMouseEntered );
				break;

			case sf::Event::MouseLeft:				
				TraverseList( DistpatchMap[InputFlags::MouseLeft], winEvent, CallMouseLeft );
				break;

			case sf::Event::JoystickButtonPressed:	
				TraverseList( DistpatchMap[InputFlags::JoystickButtonPressed], winEvent, CallJoystickButtonPressed );
				break;

			case sf::Event::JoystickButtonReleased:	
				TraverseList( DistpatchMap[InputFlags::JoystickButtonReleased], winEvent, CallJoystickButtonReleased );
				break;

			case sf::Event::JoystickMoved:			
				TraverseList( DistpatchMap[InputFlags::JoystickMoved], winEvent, CallJoystickMoved );
				break;

			case sf::Event::JoystickConnected:		
				TraverseList( DistpatchMap[InputFlags::JoystickConnected], winEvent, CallJoystickConnected );
				break;

			case sf::Event::JoystickDisconnected:	
				TraverseList( DistpatchMap[InputFlags::JoystickDisconnected], winEvent, CallJoystickDisconnected );
				break;

			case sf::Event::Closed:
				if (DistpatchMap[InputFlags::Closed].size() == 0)
					WindowManager::MainWindow.close(); // Automatic window close if no gameobject registered for this event
				else
					TraverseList( DistpatchMap[InputFlags::Closed], winEvent, CallWindowClosed ); 
				break;

			default:
				ConsoleMsg::ShowError("Unknown even in InputEventManager");
		}
    }
}