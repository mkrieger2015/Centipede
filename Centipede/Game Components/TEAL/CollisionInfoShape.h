// CollisionInfoShape.h
// Andre Berthiaume, July 2012
//

#ifndef _CollisionInfoShape
#define _CollisionInfoShape

#include "CollisionInfo.h"

class CollisionInfoShape : public CollisionInfo
{
private:
	sf::Shape *shape;

public:
	CollisionInfoShape( sf::Shape &s):shape(&s){};

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


#endif _CollisionInfoShape