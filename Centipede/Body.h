#ifndef _Body
#define _Body

#include "Centipede.h"
#include "Game Components\TEAL\CommonElements.h"
#include <queue>

class Blaster;
class Bullet;

class Body : public Centipede
{
public:
	Body();
	Body(float x, float y, int segments, Centipede *prev);
	virtual void Update();
	virtual void Destroy();
	virtual void Initialize(float xpos, float ypos, int segments, Centipede *prev);
	void ConvertToHead();

	virtual void WaveStop();

	CollisionTools::TextureBitmap bitmap;
	
	virtual void Collision( GameObject *other ){};
	void Collision(Bullet *other);
};

#endif