// CollisionInfoAABB.h
// Andre Berthiaume, July 2012

#ifndef _CollisionInfoAABB
#define _CollisionInfoAABB

#include "CollisionInfo.h"

class CollisionInfoAABB : public CollisionInfo
{
public:
	CollisionInfoAABB(){};

	virtual bool PairwiseCollisionTest( CollisionInfo *other );
	virtual bool CollisionTest( sf::FloatRect *r );
	virtual bool CollisionTest( sf::Shape *s);
	virtual bool CollisionTest( sf::Sprite *s, CollisionTools::TextureBitmap *b );

};

#endif _CollisionInfoAABB