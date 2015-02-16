#ifndef _GridObject
#define _GridObject

#include "Game Components\TEAL\CommonElements.h"

class GridObject : public GameObject
{
public:
	GridObject();
	bool IsPoison();
	virtual int ReturnHealth();
	virtual void Repair(){ };
	virtual void Initialize();
	virtual sf::Vector2f ReturnPosition();
protected:
	bool is_poison;
	int health;
	sf::Vector2f Pos;
};

#endif