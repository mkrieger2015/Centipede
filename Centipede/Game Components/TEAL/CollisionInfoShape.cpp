// CollisionInfoShape.cpp
// Andre Berthiaume, July 2012

#include "CollisionInfoShape.h"
#include "WindowManager.h"

sf::Vector2f CollisionInfoShape::getPosition(){ return shape->getPosition(); }
float CollisionInfoShape::getRotation(){ return shape->getRotation();}
sf::Transform CollisionInfoShape::getTransform(){ return shape->getTransform(); }

void CollisionInfoShape::Draw(){ WindowManager::MainWindow.draw( *shape ); }

sf::FloatRect CollisionInfoShape::getAABB()
{
	return shape->getGlobalBounds();
}

bool CollisionInfoShape::PairwiseCollisionTest( CollisionInfo *other )
{
	return other->CollisionTest( shape );
}

bool CollisionInfoShape::CollisionTest( sf::FloatRect *r )
{
	return CollisionTools::RectangleShapeIntersection( *r, *shape );
}

bool CollisionInfoShape::CollisionTest( sf::Shape *s )
{
	sf::Vector2f dummy;
	return CollisionTools::ShapeShapeIntersection( *shape, *s, dummy );
}

bool CollisionInfoShape::CollisionTest( sf::Sprite *s, CollisionTools::TextureBitmap *b )
{
	return CollisionTools::ShapeSpriteIntersection( *shape, *s, *b);
}