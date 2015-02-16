// GlobalDefinitions
// Andre Berthiaume, July 2012

/** \file GlobalDefinitions.h */

/** \brief	Bitflags used when registering a GameObject for various input events.
* <summary>	Each of these flags is associated with a specific window, keyboard, mouse or controller event.
* 			These values are used in the RegisterEvent method. To register your GameObjet for more than one of these 
* 			events, use must bitwise-OR the values. 			
* 			</summary>

* \b Example: The following code will register Bug to be nottified whenver either (any) key presses and/or mouse movement occur.
* 			\code
*			Bug::Bug()
*			{
*			    RegisterInput( InputFlags::KeyPressed | InputFlags::MouseMoved );
*			}
*			\endcode
* \ingroup INPUTEVENTS
*/ 

#ifndef _GlobalDefinitions
#define _GlobalDefinitions

namespace InputFlags
{ 
	enum Flags { 
				/// The window was resized
				Resized					= 0x00001,	

				/// The window lost focust
				LostFocus				= 0x00002,	

				/// The window gained focus
				GainedFocus				= 0x00004,   

				/// Some key was pressed (momentary event only)
				KeyPressed				= 0x00008, 

				/// Some key was release (momentary event only)
				KeyReleased				= 0x00010, 

				/// The mouse wheel was used
				MouseWheelMoved			= 0x00020, 
				
				/// One of the mouse buttons was pressed (momentary event only)
				MouseButtonPressed		= 0x00040, 
				
				/// One of the mouse buttons was released (momentary event only)
				MouseButtonReleased		= 0x00080, 
				
				/// The mouse pointer has moved
				MouseMoved				= 0x00100,
				
				/// The mouse pointer is now within the window area
				MouseEntered			= 0x00200, 
				
				/// The mouse pointer has left the window area
				MouseLeft				= 0x00400, 
				
				/// A joystick button was pressed (momentary event only)
				JoystickButtonPressed	= 0x00800, 
				
				/// A joystick button was released (momentary event only)
				JoystickButtonReleased	= 0x01000,	 
				
				/// A joystick cursor has moved
				JoystickMoved			= 0x02000, 
				
				/// A joystick has been connected
				JoystickConnected		= 0x04000,
				
				/// A joystick has been connected
				JoystickDisconnected	= 0x08000,

				/// Window has been closed
				Closed					= 0x10000,

				// Text has been entered
				TextEntered				= 0x20000

			};
};


#endif _GlobalDefinitions