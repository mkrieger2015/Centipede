// CollisionInfo.h
// Andre Berthiaume, July 2012
//
// Base class for gathering collision mode information function hookups

#ifndef _CollisionInfo
#define _CollisionInfo

#include "CollisionTools.h"

class CollisionInfo
{
public:
	CollisionInfo(){};

	bool Marked;
	int numericMark;
	
	virtual sf::FloatRect getAABB(){ return sf::FloatRect(); };
	virtual sf::Vector2f getPosition(){ return sf::Vector2f(); };
	virtual float getRotation(){ return 0.0f;};
	virtual sf::Transform getTransform(){ return sf::Transform::Identity; };

	virtual bool PairwiseCollisionTest( CollisionInfo *other ){ return false; };
	virtual bool CollisionTest( sf::FloatRect *r ){ return false; };
	virtual bool CollisionTest( sf::Shape *s){ return false; };
	virtual bool CollisionTest( sf::Sprite *s, CollisionTools::TextureBitmap *b ){ return false; };

	virtual void Draw(){};
};

#endif _CollisionInfo