#ifndef _KeyboardInput
#define _KeyboardInput

#include "Game Components\TEAL\CommonElements.h"
#include "InputController.h"

class KeyboardInput : public InputController
{
public:
	KeyboardInput();
	KeyboardInput (Blaster *blast);
	virtual void Update();
	virtual void Destroy();
protected:
private:
};

#endif 