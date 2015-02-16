#ifndef _Head
#define _Head

#include "Game Components\TEAL\CommonElements.h"
#include "Centipede.h"

class Blaster;
class Bullet;

class Head : public Centipede
{
public:
	Head(){ };
	Head(float xpos, float ypos, bool initial_direction, int segments);
	virtual void Update();
	virtual void Destroy();
	void InitializeNew(float xpos, float ypos, bool initial_direction, int segments);
	void InitializeTransform(float xpos, float ypos, std::queue<orders> in_queue, std::queue<orders> old_out, Centipede *new_next, bool direction);
	
	virtual void Collision( GameObject *other ){};
	void Collision(Blaster *other);
	void Collision(Bullet *other);
	
	void NormalMove();
	void ChangeRow(bool up_down);

	virtual void WaveStop();
	virtual void DoubleSpeed();

private:
	std::queue<float> offset_queue_master;
	std::queue<float> offset_queue_copy;

	CollisionTools::TextureBitmap bitmap;

	bool ascending_descending;
	bool poisoned;
	bool row_changing;
	int x_direction_multiplier;

	float speed;
	float rotation_increment;
	float rotation_cumulative;
	float detection_range;
	float queue_angles;

	float xprevious;
	float yprevious;
};

#endif _Head