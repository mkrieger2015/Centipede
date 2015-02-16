#ifndef _Centipede
#define _Centipede

#include "Game Components\TEAL\CommonElements.h"
#include <queue>

class Centipede : public GameObject
{
public:
	struct orders
	{
		float x_offset;
		float y_offset;
		float rotation_degrees;
		bool direction;
		int x_direction_multiplier;
	};

	Centipede();
	virtual void Update();
	virtual void Draw();
	virtual void Initialize(float xcoord, float ycoord);
	virtual void Destroy();
	virtual void Collision( GameObject *other ){};
	virtual void ReadQueue();
	virtual void WaveStop(){ };
	virtual void DoubleSpeed(){ };
	//virtual void MakeInvisible(){ };

	virtual void ConvertToHead();

	std::queue<orders> order_queue;
	std::queue<orders> out_queue;
	std::queue<orders> storage_queue;

	Centipede *next;
	Centipede *previous;

protected:
	bool direction;
	bool was_shot;
	sf::Vector2f Pos;
	AnimatedSprite MainSprite;
	int rotation_switch;
	int movement_switch;



private:
};

#endif _Centipede