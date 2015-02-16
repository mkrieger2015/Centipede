#ifndef _Blaster
#define _Blaster

#include "Game Components\TEAL\CommonElements.h"

class Head;
class Bullet;
class Mushroom;
class Body;
class Spider;
class Flea;
class InputController;

class Blaster : public GameObject
{
public:
	Blaster();
	void Initialize(bool mode);
	virtual void Update();
	virtual void Draw();
	virtual void Destroy();
	virtual void Alarm1();
	void Up(int mult);
	void Down(int mult);
	void Left(int mult);
	void Right(int mult);
	void Fire();
	void Reload();
	void Death();
	void Respawn();

	virtual void Collision( GameObject *other ){};
	void Collision(Mushroom *other);
	void Collision(Head *other);
	void Collision(Body *other);
	void Collision(Spider *other);
	void Collision(Flea *other);

	float ReturnX();
	float ReturnY();

private:
	float xprev;
	float yprev;

	bool loaded;
	bool control_mode;

	Bullet* active_bullet;

	sf::Vector2f Pos;

	InputController* input;

	AnimatedSprite *DisplaySprite;

	AnimatedSprite MainSprite;
	AnimatedSprite ExplosionSprite;
	CollisionTools::TextureBitmap bitmap;

	int movement_lock;

};
#endif 