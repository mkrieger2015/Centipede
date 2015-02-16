#ifndef _Spider
#define _Spider

#include "Game Components\TEAL\CommonElements.h"
#include <queue>

class Bullet;
class Mushroom;

class Spider : public GameObject
{
public:
	struct spider_orders
	{
		float x_position;
		float y_position;
	};

	Spider();
	virtual void Update();
	virtual void Draw();
	virtual void Destroy();
	void Initialize();

	virtual void Alarm1();
	virtual void Collision( GameObject *other ){};
	void Collision(Bullet *other);
	void Collision(Mushroom *other);
	
	void EmptyQueue();
	void MoveUp();
	void MoveDown();
	void MoveDiagonalUp();
	void MoveDiagonalDown();
	void AwardPoints();
	void WaveStop();

private:
	bool direction;
	int direction_multiplier;
	float speed;

	sf::Vector2f Pos;

	AnimatedSprite MainSprite;
	CollisionTools::TextureBitmap bitmap;

	std::queue<spider_orders> orders_queue;
	spider_orders temp_orders;

};

#endif