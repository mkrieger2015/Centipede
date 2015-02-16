#ifndef _InputController
#define _InputController

#include "Game Components\TEAL\CommonElements.h"

class Blaster;

class InputController : public GameObject
{
public:
	InputController();
	virtual void Update();
protected:
	Blaster *player;
private:
};

#endif