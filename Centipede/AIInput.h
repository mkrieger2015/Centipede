#ifndef _AIInput
#define _AIInput

#include "Game Components\TEAL\CommonElements.h"
#include "InputController.h"
#include <queue>

class AIInput : public InputController
{
public:
	AIInput();
	AIInput (Blaster *blast);
	virtual void Update();
	void MoveUpLeft();
	void MoveUpRight();
	void MoveDownLeft();
	void MoveDownRight();
	void StandStill();

private:
	int up;
	int down;
	int left;
	int right;
	int iterator;
};

#endif 