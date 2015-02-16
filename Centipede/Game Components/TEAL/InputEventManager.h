// InputEventManager.h
// Andre Berthiaume, June 2012
//
// Processes inputs for keyboard, mouse, joystick, etc and dispatches the calls

#ifndef _InputEventManager
#define _InputEventManager

//#include "GameObject.h"
#include "GlobalDefinitions.h"
#include <list>
#include <map>
#include "SFML\Window.hpp"

// Forward declarations
class GameObject;

class InputEventManager
{
public: 


private:
	typedef int BitFlags;
	typedef std::map<GameObject*, BitFlags> GameObjectFlagMap;
	typedef std::list<GameObject*> GameObjectList;
	typedef std::map<InputFlags::Flags, GameObjectList> FlagListMap;

	static GameObjectList registeredGameObjects;
	static GameObjectFlagMap registrationMap;
	static FlagListMap DistpatchMap;


	typedef void (*TraversalOperation)(GameObject*, sf::Event e); // generic type for a pointer to functions below
	static void TraverseList( GameObjectList gol, sf::Event e, TraversalOperation f );

	// these are required because lambdas are borked in VS 2010 according to https://connect.microsoft.com/VisualStudio/feedback/details/572138
	static void CallWindowResized(GameObject* go, sf::Event e);
	static void CallWindowLostFocus(GameObject* go, sf::Event e);
	static void CallWindowGainedFocus(GameObject* go, sf::Event e);
	static void CallWindowClosed(GameObject* go, sf::Event e);
	static void CallKeyPressed(GameObject* go, sf::Event e);
	static void CallKeyReleased(GameObject* go, sf::Event e);
	static void CallTextEntered(GameObject* go, sf::Event e);
	static void CallMouseWheelMoved(GameObject* go, sf::Event e);
	static void CallMouseButtonPressed(GameObject* go, sf::Event e);
	static void CallMouseButtonReleased( GameObject* go, sf::Event e);
	static void CallMouseMoved(GameObject* go, sf::Event e);
	static void CallMouseEntered(GameObject* go, sf::Event e);
	static void CallMouseLeft(GameObject* go, sf::Event e);
	static void CallJoystickConnected( GameObject* go, sf::Event e);
	static void CallJoystickDisconnected( GameObject* go, sf::Event e);
	static void CallJoystickButtonPressed( GameObject* go, sf::Event e);
	static void CallJoystickButtonReleased( GameObject* go, sf::Event e);
	static void CallJoystickMoved( GameObject* go, sf::Event e);

public:
	InputEventManager(){};

	void ProcessInputEvents();
	void Register(GameObject& go, BitFlags flagvalues = 0xfffff);
	void Deregister(GameObject& go, BitFlags flagvalues = 0xfffff);
};


#endif _InputEventManager