// WindowManager.h
// Andre Berthiaume, June 2012
//
// General Window controls and settings
// (Future versions will include View managements as well)

#ifndef _WINDOWING_H
#define _WINDOWING_H

#include "SFML\Graphics.hpp"

class WindowManager                  
{
private:
	static WindowManager* windowInstance;			// Setting up a singleton
	WindowManager(){};								// Private constructor
	WindowManager( const WindowManager&);			// Prevent copy construction
	WindowManager& operator=(const WindowManager&);	// prevent assignment
	static WindowManager& Instance()  
	{
		if (! windowInstance) 
			windowInstance = new WindowManager;
		return *windowInstance;
	};

	static void SetTitle(){MainWindow.setTitle(windowCaption + ": " + captionMsg); };

	static int windowWidth;
	static int windowHeight;
	static std::string windowCaption;	
	static sf::Color backgroundColor;
	static std::string captionMsg;

public:
	static sf::RenderWindow MainWindow;	
	
	static void Initialize();
	static void Clear();
	static void Display();
	static void SetCaptionName(std::string name){ windowCaption = name; SetTitle();  };
	static void SetCaptionMessage(std::string msg){ captionMsg = msg; SetTitle();  };

	static void SetBackgroundColor(sf::Color col){ backgroundColor = col; };

	/// \brief Returns the mouse position in window coordinates.
	/// 
	/// \return A <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Vector2.php">sf::Vector2f</a> of the coordinates	
	/// 
	/// Typically used in the GameObject::Update. For exameple:
	/// 
	///		\code
	///		void Object1::Update()
	///		{
	///			sf::Vector2f pos =  WindowManager::MousePosition();
	///			 
	///			< Do something with pos, like move a sprite or shape. >
	///		}
	///		\endcode
	static sf::Vector2f MousePosition(){ return static_cast<sf::Vector2f>( sf::Mouse::getPosition( WindowManager::MainWindow )); }
};

#endif _WINDOWING_H