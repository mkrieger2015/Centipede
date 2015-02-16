#ifndef _Flea
#define _Flea

#include "Game Components\TEAL\CommonElements.h"

class Bullet;

class Flea : public GameObject
{
public:
	Flea();
	virtual void Update();
	virtual void Draw();
	virtual void Destroy();
	void Initialize();

	virtual void Collision( GameObject *other ){};
	void Collision(Bullet *other);

	void WaveStop();

private:
	float speed_multiplier;
	int life;

	sf::Vector2f Pos;

	AnimatedSprite MainSprite;
	CollisionTools::TextureBitmap bitmap;

};

#endif