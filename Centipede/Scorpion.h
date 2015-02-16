#ifndef _Scorpion
#define _Scorpion

#include "Game Components\TEAL\CommonElements.h"

class Bullet;
class Mushroom;

class Scorpion : public GameObject
{
public:
	Scorpion();
	virtual void Update();
	virtual void Draw();
	virtual void Destroy();
	void Initialize();

	virtual void Collision( GameObject *other ){};
	void Collision(Bullet *other);
	void Collision(Mushroom *other);

	void WaveStop();

private:
	int direction;

	sf::Vector2f Pos;

	AnimatedSprite MainSprite;
	CollisionTools::TextureBitmap bitmap;

};

#endif