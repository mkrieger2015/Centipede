#ifndef _Mushroom
#define _Mushroom

#include "Game Components\TEAL\CommonElements.h"
#include "GridObject.h"

class Bullet;

class Mushroom : public GridObject
{
public:
	Mushroom();
	virtual void Draw();
	virtual void Destroy();
	virtual void Collision( GameObject *other ){};
	virtual void Collision (Bullet *other);
	void Initialize(float xpos, float ypos);
	virtual int ReturnHealth();
	virtual void Repair();

private:

	AnimatedSprite MainSprite;
	CollisionTools::TextureBitmap bitmap;

};
#endif 