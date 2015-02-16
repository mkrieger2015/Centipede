// CollisionInfoAABB.h
// Andre Berthiaume, July 2012

#include "CollisionInfoAABB.h"

bool CollisionInfoAABB::PairwiseCollisionTest( CollisionInfo *other )
{
	return other->CollisionTest( &( getAABB()) );
}

bool CollisionInfoAABB::CollisionTest( sf::FloatRect *r )
{
	return getAABB().intersects(*r);
}

bool CollisionInfoAABB::CollisionTest( sf::Shape *s)
{
	return CollisionTools::RectangleShapeIntersection( getAABB(), *s );
}

bool CollisionInfoAABB::CollisionTest( sf::Sprite *s, CollisionTools::TextureBitmap *b )
{
	return CollisionTools::RectangleSpriteIntersection( getAABB(), *s, *b);
}