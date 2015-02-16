// CollisionInfoSprite.h
// Andre Berthiaume, July 2012

#ifndef _CollisionInfoSprite
#define _CollisionInfoSprite

#include "CollisionInfo.h"

class CollisionInfoSprite : public CollisionInfo
{
private:
	sf::Sprite *sprite;
	CollisionTools::TextureBitmap *bitmap;

public:
	CollisionInfoSprite( sf::Sprite &s, CollisionTools::TextureBitmap &b):sprite(&s), bitmap(&b){};

	virtual sf::FloatRect getAABB();
	virtual sf::Vector2f getPosition();
	virtual float getRotation();
	virtual sf::Transform getTransform();

	virtual bool PairwiseCollisionTest( CollisionInfo *other );
	virtual bool CollisionTest( sf::FloatRect *r );
	virtual bool CollisionTest( sf::Shape *s);
	virtual bool CollisionTest( sf::Sprite *s, CollisionTools::TextureBitmap *b );

	virtual void Draw();
};

#endif _CollisionInfoSprite