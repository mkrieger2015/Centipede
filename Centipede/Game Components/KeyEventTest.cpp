// KeyEventTest
// Andre Berthiaume, March 2013

#include "KeyEventTest.h"

KeyEventTest::KeyEventTest()
{
	myfont = sf::Font::getDefaultFont();
	myString = "This last key you pressed has code: ";

	myText = sf::Text( myString, myfont, 20);
	myText.setColor( sf::Color::Yellow );
	myText.setPosition( 50, 50 );
	
	RegisterInput( InputFlags::KeyPressed | InputFlags::KeyReleased );
}

void KeyEventTest::Destory()
{
	DeregisterInput(); 
}

void KeyEventTest::Draw()
{
	WindowManager::MainWindow.draw(myText);
}

void KeyEventTest::KeyPressed(sf::Keyboard::Key k, bool altKey, bool ctrlKey, bool shiftKey, bool systemKey)
{
	myText.setString( myString + Tools::ToString(k) );
}

void KeyEventTest::KeyReleased(sf::Keyboard::Key k, bool altKey, bool ctrlKey, bool shiftKey, bool systemKey)
{
	myText.setString( myString );
}