// KeyEventTest
// Andre Berthiaume, March 2013

#ifndef _KeyEventTest
#define _KeyEventTest

#include "TEAL\CommonElements.h"

class KeyEventTest : public GameObject
{
public:
	KeyEventTest();
	virtual void Draw();
	virtual void Destory();
	virtual void KeyPressed(sf::Keyboard::Key k, bool altKey, bool ctrlKey, bool shiftKey, bool systemKey);
	virtual void KeyReleased(sf::Keyboard::Key k, bool altKey, bool ctrlKey, bool shiftKey, bool systemKey);

private:
	sf::Font myfont;
	sf::Text myText;
	std::string myString;
};

#endif _KeyEventTest