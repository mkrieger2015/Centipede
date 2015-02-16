#ifndef _Bullet
#define _Bullet

#include "Game Components\TEAL\CommonElements.h"

class Blaster;
class Mushroom;
class BulletFactory;

class Bullet: public GameObject
{
public:
	Bullet();
	void Initialize(Blaster *blast, float startPos_x, float startPos_y);
	virtual void Update();
	virtual void Draw();
	virtual void Destroy();
	virtual void Collision( GameObject *other ){};

private:
	AnimatedSprite MainSprite;
	CollisionTools::TextureBitmap bitmap;
	sf::Vector2f Pos;
	Blaster *player;
};

#endif