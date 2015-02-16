// WindowManager.cpp
// Andre Berthiaume, June 2012
//
// General Window controls and settings
// (Future versions will include View managements as well)

#include "WindowManager.h"
#include "../../CommonFunctionsAndValues.h"

extern const int window_width;
extern const int window_height;

WindowManager* WindowManager::windowInstance = NULL;

sf::RenderWindow WindowManager::MainWindow;

int	WindowManager::windowWidth = window_width;
int	WindowManager::windowHeight = window_height;
std::string	WindowManager::windowCaption = "TEAL/SFML";
sf::Color WindowManager::backgroundColor = sf::Color(0,64,64); // sf::Color(32,32,64,255);
std::string WindowManager::captionMsg = "";

void WindowManager::Initialize()
{
	WindowManager::MainWindow.create( sf::VideoMode(windowWidth, windowHeight), windowCaption + ": " + captionMsg );
	WindowManager::MainWindow.setKeyRepeatEnabled(false);
	WindowManager::MainWindow.setFramerateLimit(60);
	WindowManager::MainWindow.setMouseCursorVisible(false);
}

void WindowManager::Clear()
{
	WindowManager::MainWindow.clear(backgroundColor);
}

void WindowManager::Display()
{
	WindowManager::MainWindow.display();
}



