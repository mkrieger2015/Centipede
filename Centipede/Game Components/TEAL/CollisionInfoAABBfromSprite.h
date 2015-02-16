// CollisionInfoAABBfromSprite.h
// Andre Berthiaume, July 2012

#ifndef _CollisionInfoAABBfromSprite
#define _CollisionInfoAABBfromSprite

#include "CollisionInfoAABB.h"

class CollisionInfoAABBfromSprite : public CollisionInfoAABB
{
private:
	sf::Sprite *sprite;

public:
	CollisionInfoAABBfromSprite(sf::Sprite &s):sprite(&s){};

	virtual sf::FloatRect getAABB();
	virtual sf::Vector2f getPosition();
	virtual float getRotation();
	virtual sf::Transform getTransform();

	virtual void Draw();
};

#endif _CollisionInfoAABBfromSprite