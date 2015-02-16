#ifndef _PoisonMushroom
#define _PoisonMushroom

#include "Game Components\TEAL\CommonElements.h"
#include "GridObject.h"

class Bullet;

class PoisonMushroom : public GridObject
{
public:
	PoisonMushroom();
	virtual void Draw();
	virtual void Destroy();
	virtual void Collision( GameObject *other ){};
	virtual void Collision (Bullet *other);
	void Initialize(float xpos, float ypos);
	void InitializeDamaged(float xpos, float ypos, int prev_health);
	virtual void Repair();

private:

	AnimatedSprite MainSprite;
	CollisionTools::TextureBitmap bitmap;

	bool converted;
};
#endif 