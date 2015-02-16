#ifndef _ModeSelect
#define _ModeSelect

#include "Game Components\TEAL\CommonElements.h"

class ModeSelect : public GameObject
{
public:
	ModeSelect();
	virtual void KeyPressed(sf::Keyboard::Key k, bool altKey, bool ctrlKey, bool shiftKey, bool systemKey);
	virtual void Destroy();
protected:
private:
};

#endif 