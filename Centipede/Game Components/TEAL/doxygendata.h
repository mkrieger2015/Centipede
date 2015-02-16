
/** \defgroup TEAL-API TEAL API
\brief These commands are available to TEAL users
*/

/** \defgroup GAMEOBJECT GameObject
\ingroup TEAL-API
\brief All game entities will be derived from GameObject. 
*/

/** \defgroup DEFAULTEVENTS Default Events
\ingroup GAMEOBJECT
\brief Events that all GameObjects are automatically registered to receive.
*/

/** \defgroup INPUTEVENTS Input events. 
\ingroup GAMEOBJECT
\brief Callbacks associated with window, keyboard, mouse and game controllers events. *IMPORTANT*: 
In order for any of these callbacks to be activated, the 
GameObject must be registered using the GameObject::RegisterInput function and the associated InputFlags.
*/

/** \defgroup ALARMS Alarm control and events
\ingroup GAMEOBJECT

\brief Each GameObjects can have set up to 5 seperate alarms (numbered 0 through 4) and have the associated callback triggered.

Alarms are simple countdown mechanism to help control time-based events. Below is a simple example of a self-resetting alarm to make
the Bug move about randomly. (Please refer to <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Shape.php"><b>sf::Shape</b></a> for
more information about its member functions)

	\code
	Bug::Bug( )
	{
		myshape.setPointCount(3);
		myshape.setRadius( 15 );
		myshape.setPosition( 100, 100 );

		SetAlarm( 1, 2); 	// sets Alarm 1 to go off in 2 secs
	}

	void Bug::Update()
	{
		// ... move myshape forward...
	}

	void Bug::Alarm1() // This is the Alarm1 callback
	{
		myshape.rotate( (float) (90 - 2 * ( rand() % 90 )) ); // adds a random rotation angle [-45..45] degrees
		SetAlarm(  1, 1.0f + rand() % 4); // Resets Alarm 1 to a random value [1..5] secs;
	}
	\endcode

*/

/** \defgroup COLLISION Collision set-up
\ingroup GAMEOBJECT
\brief Basic structure and operations of the collision system.

Long winded description of how it works
*/

/** \defgroup COLREG Collision registration
\ingroup GAMEOBJECT
\brief Just assigning collision information is not enough: a GameObject must also be registered to the collision system.

*/

/** \defgroup METHODS Methods
\ingroup GAMEOBJECT
\brief Various methods to control internal aspects of the GameObject.
*/

/** \defgroup RESOURCES Resources
\ingroup TEAL-API
\brief Loading external resources into the game.
*/

/** \defgroup SCENE Scene
\ingroup TEAL-API
\brief A Scene is the space in which GameObjects exist. 

Scenes are the sandbox in which GameObjects exists. Each level of a game will have a corresponding Scene of course, but other 
important game steps such as splash pages, menu selection, inventory, etc. will likely use a seperate Scene.

When deriving from Scene, there is only one method to implement: Scene::Initialize. This method is called when creating a new Scene. Using this method,
create all the initial GameObject that are required as well as indicating which collision pair are expected to be used.
*/


/** \defgroup TEAL-INTERNAL Internal to TEAL
\brief These should not be messed with
*/

/**************************************************************************************************************/


/*! \mainpage About TEAL

TEAL is a simple game framework in C++ for 2D games that extends the excellent <a href="http://www.sfml-dev.org/index.php"><b>SFML API</b></a>.

\section features Features
TEAL provides the following components:
	- A resource manager organize your external assets such as textures, images, sounds, etc. 
	- Base classes Scene and GameObject from which to derive your own levels and game entities.
	- A built-in game loop to update and draw the current Scene and its associated GameObject.
	- A built-in collision system for GameObject with both axis-aligned boundig box and precise collision checking.
	- Callbacks for Collision events, alarms events, input events (keyboard, controller, mouse, etc.), window events (Focus, resize, etc.).

The framework was designed to be fairly minimalistic: it adds the minimum layer of code on top of the <a href="http://www.sfml-dev.org/index.php"><b>SFML API</b></a> 
to allow you to make simple 2D game. 

*/



/*! \page install Installation
Installing TEAL is easy: 

- <b>Step 1:</b> Download and install <a href="http://www.sfml-dev.org/download.php"><b>SFML 2.0</b></a> 
	- Note: it must be version 2.0 and prefereably using Visual Studio 2010
	- Make sure to follow these <a href="http://www.sfml-dev.org/tutorials/2.0/start-vc.php"><b>installation intructions</b></a>.

- <b>Step 2</b>: Download the TEAL package here 
	- simply include all these files in your c++ project.

*/

/*! \page usingteal Using TEAL
There are two documentation modules for TEAL:

- \ref TEAL-API "Module 1: TEAL API": This is the most common case where you jsut want to use the framework

- \ref TEAL-INTERNAL "Module 1: TEAL's internal workings": This is provided only for those interested in looking inside 
	the framework and see how it works.

*/

