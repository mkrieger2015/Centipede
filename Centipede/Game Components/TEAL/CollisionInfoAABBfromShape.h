// CollisionInfoAABBfromShape.h
// Andre Berthiaume, July 2012

#ifndef _CollisionInfoAABBfromShape
#define _CollisionInfoAABBfromShape

#include "CollisionInfoAABB.h"

class CollisionInfoAABBfromShape : public CollisionInfoAABB
{
private:
	sf::Shape *shape;

public:
	CollisionInfoAABBfromShape(sf::Shape &s):shape(&s){};

	virtual sf::FloatRect getAABB();
	virtual sf::Vector2f getPosition();
	virtual float getRotation();
	virtual sf::Transform getTransform();

	virtual void Draw();
};

#endif _CollisionInfoAABBfromShape