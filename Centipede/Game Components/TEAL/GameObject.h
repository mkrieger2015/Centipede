// GameObject.h
// Andre Berthiaume, June 2012
//
// Base gameobject class

#ifndef _GameObject
#define _GameObject

#include "ConsoleMsg.h"
#include "CollisionInfo.h"
#include "CollisionGrid.h"
#include "GlobalDefinitions.h"
#include "Scene.h"

class GameObject
{
private:
	typedef int BitFlags;	// used as a way to allow OR-ing of InputFlags
	bool ManageExternally;  // Default false. True if the gameobject is the be managed/deleted by an external entity (typically, a factory)
	int updateOrder;
	int drawOrder;
	bool toBeRemoved;		// whether the gameobject has been marked to be removed from the scene 

	CollisionInfo *colliderInfo;

	template <class T> friend class CollisionGrid;			// This friendship exist only to grant access to the getCollider() method
	CollisionInfo* getCollider(){ return colliderInfo; }	// Technically not needed because of the friendship, but it's cleaner...

friend class Scene;
	Scene::DeleteOperation ManagedDeleteOperation;

public:
	sf::Transform WorldMat;

	void SetExternalManagement( Scene::DeleteOperation f);

	/// \brief Returns true is the gameobject is set to be managed/deleted by an another entity than the scene.
	bool GetExternalManagement();
	
	/// \brief	Removes all collision information.
	/// \ingroup COLLISION
	/// 		 
	/// The gameobject will not be able to receive collision events. </summary>
	void SetCollider();

	/// \brief Associates the colliding entity with the shape s.
	/// \ingroup COLLISION
	///
	/// \param s The sf::Shape &amp; to use.
	/// 		 Please refer to <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Shape.php"><b>sf::Shape</b></a> for more information on shapes.
	/// \param precision If precision is set to true, then the exact shape will be used to test for collision.
	///					Otherwise, the axis-aligned bouding box of the shape will be used. </param>
	void SetCollider( sf::Shape &s, bool precision = true);

	/// \brief Associates the colliding entity with the sprite s and bitmap b.
	/// \ingroup COLLISION
	/// 		 
	/// \param s The sf::Sprite &amp; to use.
	/// 		 Please refer to <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Sprite.php">SFML::Sprite</a> for more information.
	/// \param b The CollisionTools::TextureBitmap &amp; to use. 
	/// 		  Normally, the associated with the sprite through #ResourceManager::SetSpriteTextureAndBitmap.
	/// \param precision  If true then the exact bitmap will be used to test for collision.
	/// Otherwise, the axis-aligned bouding box of the sprite will be used.
	void SetCollider( sf::Sprite &s, CollisionTools::TextureBitmap &b,  bool precision = true);
	
	GameObject();
	virtual ~GameObject();

	/// \brief Called once per frame on all GameObjects in the current Scene. 
	/// 	   
	/// \ingroup DEFAULTEVENTS
	/// All GameObjects are automatically registered to the current Scene which triggers this event once per frame.
	/// By default this event does nothing: you will need to implement it in your derived classes.
	///
	/// \note in rare cases it might be useful to have certain GameObjects updated before others. See setUpdateOrder if needed.
	virtual void Update(){};

	/// \brief Called once per frame on all GameObjects in the Current Scene. 
	/// 
	/// \ingroup DEFAULTEVENTS
	/// All GameObjects are automatically registered to the current Scnee which triggers this event once per frame.
	/// By default this event draws whatever was set with SetCollider. You can implement it in your derived class to have more complex
	/// drawing events
	///  
	/// \note In some cases it may be useful to control the draw order. This is particularly important with HUD (Heads up display) 
	/// which need to be drawn last so as to appear 'on top' of the other objects. Please refer to setDrawOrder if needed.
	virtual void Draw(){ colliderInfo->Draw(); };

	/// \brief Callback used when the window is resized.
	/// \ingroup INPUTEVENTS
	virtual void WindowResized(){};

	/// \brief Callback used when the window has lost focus.
	/// \ingroup INPUTEVENTS
	virtual void WindowLostFocus(){};

	/// \brief Callback used when the window has gained focus.
	/// \ingroup INPUTEVENTS
	virtual void WindowGainedFocus(){};

	/// \brief Callback used when the window closes. 
	/// 	   NOTE: If this callback is used, the window will not actually be closed autimatically. 
	/// 	   To closed the window, used "WindowManager::MainWindow.close();" within the callback
	virtual void WindowClosed(){};

	/// \brief Callback used when a key pressed (momentary event only).
	/// \ingroup INPUTEVENTS
	/// 		 
	/// \param k		 The <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Keyboard.php">SFML::Keyboard</a> that is pressed.
	/// \param altKey    true if alternate key is pressed.
	/// \param ctrlKey   true if control key the control key is pressed.
	/// \param shiftKey  true if shift key is pressed.
	/// \param systemKey true if system key is pressed.
	/// 	
	/// \note for _continuous_ key presses/releases, it would probably be better to use 
	/// 	<a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Keyboard.php#a80a04b2f53005886957f49eee3531599">sf::Keyboard::IsKeyPressed</a>. 
	///  inside the GameObject::Update event.
	virtual void KeyPressed(sf::Keyboard::Key k, bool altKey, bool ctrlKey, bool shiftKey, bool systemKey){};

	/// \brief Callback used when a key released (momentary event only).
	/// \ingroup INPUTEVENTS
	/// 		 
	/// \param k		 The <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Keyboard.php">SFML::Keyboard</a> released.
	/// \param altKey    true if alternate key is pressed.
	/// \param ctrlKey   true if control key the control key is pressed.
	/// \param shiftKey  true if shift key is pressed.
	/// \param systemKey true if system key is pressed.
	/// 
	/// \note for _continuous_ key presses/releases, it would probably be better to use 
	/// 	<a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Keyboard.php#a80a04b2f53005886957f49eee3531599">sf::Keyboard::IsKeyPressed</a>. 
	///  inside the GameObject::Update event.
	virtual void KeyReleased(sf::Keyboard::Key k, bool altKey, bool ctrlKey, bool shiftKey, bool systemKey){};

	/// \brief Callback used when the user enters text
	///
	/// \param UnicodeValue The unicode value of the character entered.
	virtual void TextEntered(sf::Uint32 UnicodeValue){};

	/// \brief Callback used when when the mouse wheel moved.
	/// \ingroup INPUTEVENTS
	/// 		 
	/// \param delta  How much the wheel has moved.
	/// \param mouseX The current mouse x coordinate.
	/// \param mouseY The current mouse y coordinate.
	virtual void MouseWheelMoved(int delta, int mouseX, int mouseY){};

	/// \brief Callback used when a mouse button is pressed (momentary event only).
	/// \ingroup INPUTEVENTS
	/// 		 
	/// \param b	  Which button that was pressed (see <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Mouse.php">SFML::Mouse</a> for more details).
	/// \param mouseX The current mouse x coordinate.
	/// \param mouseY The current mouse y coordinate.
 	virtual void MouseButtonPressed( sf::Mouse::Button b,  int mouseX, int mouseY){};

	/// \brief Callback used when a mouse button released  (momentary event only).
	/// \ingroup INPUTEVENTS
	/// 		 
	/// \param b	  Which button was releasesd (see <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Mouse.php">SFML::Mouse</a> for more details)..
	/// \param mouseX The current mouse x coordinate.
	/// \param mouseY The current mouse y coordinate.
	virtual void MouseButtonReleased( sf::Mouse::Button b,  int mouseX, int mouseY){};

	/// \brief Callback used when a mouse moved.
	/// \ingroup INPUTEVENTS
	/// 		 
	/// \param mouseX The current mouse x coordinate.
	/// \param mouseY The current mouse y coordinate.
	/// 			   
	/// \note Alternatively, you can use WindowManager::MousePosition inside a GameObject::Update.
	virtual void MouseMoved(int mouseX, int mouseY){};

	/// \brief Callback used when the mouse has entered the window
	/// \ingroup INPUTEVENTS
	virtual void MouseEntered(){};

	/// \brief Callback used when the mouse has left the window
	/// \ingroup INPUTEVENTS
	virtual void MouseLeft(){};

	/// \brief Callback used when a joystick is connected.
	/// \ingroup INPUTEVENTS
	/// 		 
	/// \param Id The joystick number. See <a href="http://www.sfml-dev.org/documentation/2.0/structsf_1_1Event_1_1JoystickConnectEvent.php">sf::Event::JoystickConnectEvent</a>
	virtual void JoystickConnected( unsigned int Id ){};

	/// \brief Callback used when a joystick is disconnected.
	/// \ingroup INPUTEVENTS
	/// 		 
	/// \param Id The joystick number. See <a href="http://www.sfml-dev.org/documentation/2.0/structsf_1_1Event_1_1JoystickConnectEvent.php">sf::Event::JoystickConnectEvent</a>
	virtual void JoystickDisconnected( unsigned int Id ){};

	/// \brief Callback used when a joystick button is pressed.
	/// \ingroup INPUTEVENTS
	/// 
	/// See <a href="http://www.sfml-dev.org/documentation/2.0/structsf_1_1Event_1_1JoystickButtonEvent.php">sf::Event::JoystickButtonEvent</a> for more information.
	/// \param id	  The joystick number.
	/// \param button The button number.
	virtual void JoystickButtonPressed( unsigned int id, unsigned int button ){};

	/// \brief Callback used when a joystick button is released
	/// \ingroup INPUTEVENTS
	///
	///  See <a href="http://www.sfml-dev.org/documentation/2.0/structsf_1_1Event_1_1JoystickButtonEvent.php">sf::Event::JoystickButtonEvent</a> for more information.
	/// \param id	  The joystick nubmer.
	/// \param button The button nubmer.
	virtual void JoystickButtonReleased( unsigned int id, unsigned int button ){};

	/// \brief Callback used when a joystick's axis is moved.
	/// \ingroup INPUTEVENTS
	/// 		 
	/// See <a href="http://www.sfml-dev.org/documentation/2.0/structsf_1_1Event_1_1JoystickMoveEvent.php#aba5a70815420161375fd2e756689c32a">sf::Event::JoystickMoveEvent</a> for more information.
	/// \param id   The joystick number.
	/// \param axis The axis that moved.
	/// \param pos  The position on the axis.
	virtual void JoystickMoved( unsigned int id, sf::Joystick::Axis axis, float pos ){};

	/// \brief Callback used for Alarm 0.
	/// \ingroup ALARMS
	virtual void Alarm0(){};

	/// \brief Callback used for Alarm 1.
	/// \ingroup ALARMS
	virtual void Alarm1(){};

	/// \brief Callback used for Alarm 2.
	/// \ingroup ALARMS
	virtual void Alarm2(){};

	/// \brief Callback used for Alarm 3.
	/// \ingroup ALARMS
	virtual void Alarm3(){};

	/// \brief Callback used for Alarm 4.
	/// \ingroup ALARMS
	virtual void Alarm4(){};

	// Miscellaneous Callbacks 
	virtual void Collision( GameObject *other ){}; 

	/// \brief Called when this GameObject is set to be removed from TEAL control. 
	/// \ingroup DEFAULTEVENTS
	/// 		 
	/// 
	/// This is similar to the a C++ destructor: Whenever GameObject::MarkForDestroy method is called, this method is executed. 
	/// When deriving form GameObject, you will need to implement this method if for no other reason to deregister your derived gameobject from
	/// the various event manager you might have registered with.
	///
	/// \par Example:
	///    
	///  The following example shows how a Bug being killed will trigger the death sound, add points and remove the Bug from the collision system
	///  
	///		\code
	///		void Bug::Destroy()
	///		{
	///			// play a death sound
	///			// add points to the score
	///
	///			// Always deregister collisions in the destructor!
	///			DeregisterCollision<Bug>(*this);
	///		}
	///		\endcode
	virtual void Destroy(){};

	void RegisterToCurrentScene();


	// These two currently only work when used in the constructor
	// Using them later has no effect 
	void SetUpdateOrder(int n);
	void SetDrawOrder(int n);
	int GetUpdateOrder();
	int GetDrawOrder();

	/// \brief Mark the GaneObject for destruction.
	/// \ingroup METHODS
	/// 		
	/// Begins the process of removing the GameObject from TEAL's control. Also triggers the GameObject::Destroy event.
	///
	/// \par Example: 
	/// 	
	/// The following code show how a Bug colliding with a Block will play a sound, add points and destroy both itself and the colliding Block
	/// 
	///		\code
	///		void Bug::Collision( Block *other )
	///		{
	///			// Play Boom sound
	///			// Add points
	///
	///			MarkForDestroy();		// Destroys this Bug
	///			other->MarkForDestroy();	// Destroy the Block it was colliding with
	///		}
	///		\endcode
	void MarkForDestroy();

	bool GetDestroyStatus(){ return toBeRemoved; };

	/// \brief Sets an alarm to go off in a set amount of time. 
	/// \ingroup ALARMS
	/// 		 
	/// When the time is up, the associated callback Alarm0(), ..., Alarm4() will be called.
	/// 
	/// \param AlarmNumber The alarm number [0...5].
	/// \param time		   The time in seconds
	void SetAlarm(int AlarmNumber, float time);

	/// \brief Adds (or substract) time to an alarm.
	/// \ingroup ALARMS
	/// 		 
	/// 
	/// \param AlarmNumber The alarm number [0...4].
	/// \param time		   The amount of time in seconds to add (if positive) or substract (if negative). 
	void AddTime(int AlarmNumber, float time);

	/// \brief Returns how much time is left (in seconds) before the alarm goes off and the callback is activated.
	/// \ingroup ALARMS
	///
	/// \param AlarmNumber The alarm number.
	///
	/// \return The time in second left before the alarm goes off
	float GetAlarm(int AlarmNumber);

	/// \brief Returns true if the alarm is currently active (that is, not yet triggered its event callback)
	/// \ingroup ALARMS
	/// 		 
	/// \param AlarmNumber The alarm number.
	///
	/// \return true if the alarm is still in the process of counting down.
	bool AlarmActive(int AlarmNumber);

	/// \brief Cancels AlarmNumber. The associated alarm event is not executed.
	/// \ingroup ALARMS
	///
	/// \param AlarmNumber The alarm number.
	void AlarmCancel(int AlarmNumber);

	/// \brief Registers the GameObject to be notified when event (indicated by flagvalues) occur.
	/// \ingroup INPUTEVENTS
	/// 		 
	/// \param flagvalues (optional) The flagvalue input is interpreted as a bit flag collection. 
	/// 				  The specifc event(s) to be register can be selected by using the bitwise-OR of values from InputFlags.
	/// 				  If no value is given, the GameObject is registered for _all_ input events.
	/// 				  
	/// \b Example: The following code will register Bug to be nottified whenver either (any) key presses and/or mouse movement occur.
	///			\code
	///			Bug::Bug()
	///			{
	///			    RegisterInput( InputFlags::KeyPressed | InputFlags::MouseMoved );
	///			}
	///			\endcode 
	void RegisterInput( BitFlags flagvalues = (InputFlags::Flags) 0xfffff );

	/// \brief Deregisters the GameObject from notification regarding input events.
	/// \ingroup INPUTEVENTS
	/// 		 
	/// \param flagvalues (optional) The flagvalue input is interpreted as a bit flag collection.
	/// 				  The specifc event(s) to be register can be selected by using the bitwise-OR of values from InputFlags.
	/// 				  If no value is given, the GameObject is deregistered for _all_ input events.
	void DeregisterInput( BitFlags flagvalues = (InputFlags::Flags) 0xfffff );

	/// \brief Registers the GameObject to the collision system.  <b>Note: This is a _template function_!  </b>
	/// \ingroup COLREG
	///
	/// \tparam GO Derived from GameObject.
	/// \param go Must absolutely be the '*this' pointer. Any other value will trigger an error.	  
	///
	/// \par Example:
	/// The following code correctly registers the Bug (derived from GameObject, obviously)
	///		\code
	///		Bug::Bug()
	///		{
	///			// ... setCollider was called ...
	///
	///			RegisterCollision<Bug>( *this );
	///		}
	///		\endcode
	template < class GO >
	void RegisterCollision(GO &go)
	{
		if (&go != this) 
			ConsoleMsg::ShowError("You _must_ pass the '*this' pointer as input to RegisterCollision...");

		CollisionGrid<GO>::Register( go ); 
	}

	/// \brief Deregisters the GameObject from the collision system.  <b>Note: This is a _template function_!  </b>
	/// \ingroup COLREG 
	/// 		 
	/// 
	/// Deregistering a GameObject prevents it from having its collision callbacks activated. This can be done anytime for various reasons: 
	/// For example, a respawning character might wait a few seconds before registering collisions to allow the player to move out
	/// of harms way. But as the note below indicates, the one place you must note forget to use it is in the GameObject::Destroy event callback.
	/// 
	/// \par 
	/// <b>IMPORTANT:</b> You must _always_ clean up your GameObject 's registration in its GameObject::Destroy event callback! Failing
	/// 	 to do so will cause the various event managers to follow invalid pointers.
	/// 		 
	/// \tparam GO Derived from GameObject.
	/// \param go Must absolutely be the '*this' pointer. Any other value will trigger an error.
	/// 
	///  \par Example:
	/// The following code correctly deregisters the Bug (derived from GameObject, obviously)
	///		\code
	///		Bug::Destroy()
	///		{
	///			// Don't forget to deregister collisions in the Destroy event callback!
	///			DeregisterCollision<Bug>(*this);
	///		}
	///		\endcode
	template <class GO >
	void DeregisterCollision(GO &go)
	{
		if (&go != this) 
			ConsoleMsg::ShowError("You _must_ pass the '*this' pointer as input to RegisterCollision...");

		CollisionGrid<GO>::Deregister( go ); 
	}

};


#endif _GameObject