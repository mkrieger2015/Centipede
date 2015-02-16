// CollisionTools.h
// Andre Berthiaume, June 2012
//
// Collection of basic functions to determine intersection, containment, etc.

#ifndef _CollisionTools
#define _CollisionTools

#include "SFML\Graphics.hpp"

class CollisionTools
{
public: 
	/// \brief Defines a bitmap of pixels associated with a <a href="http://www.sfml-dev.org/documentation/2.0/classsf_1_1Texture.php">sf::Texture</a>.
	/// 
	/// For technical reasons related to how the SFML deals with textures and sprites, there is no way to access a specific pixels of a sprite's texture.
	/// Therefore, in order to have pixel-perfect collision test, TEAL creates an associated bitmap of pixels for each texture it loads. This bitmap 
	/// can then be made available to the collision system whenever using precise collision checking on sprites.
	typedef std::vector<std::vector<bool>> TextureBitmap;

private:
	static const float epsilon;
	static sf::FloatRect GetLocalOverlapRect( sf::FloatRect &r1, const sf::Transform &t1, sf::FloatRect &r2, const sf::Transform &t2);

public:
	static TextureBitmap ContructTextureBitmap( sf::Image &img );
	static TextureBitmap ContructTextureBitmap( sf::Image &img, sf::IntRect rect);

	static bool SegmentSegmentIntersection( sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f q1, sf::Vector2f q2, sf::Vector2f& t);
	static float LinePointDistance(  sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, bool isSegment = false);
	static bool PointInPolygon( sf::Vector2f Poly[], int vcount, sf::Vector2f p);
	static bool PointInShape( sf::Shape &S, sf::Vector2f p);
	static bool PointInSprite( sf::Vector2f p, sf::Sprite &s, CollisionTools::TextureBitmap &b );
	static bool SegmentShapeIntersection( sf::Vector2f p1, sf::Vector2f p2, sf::Shape &S, sf::Vector2f& t);
	static bool SegmentRectangleIntersection( sf::Vector2f p1, sf::Vector2f p2, sf::FloatRect &R, sf::Vector2f& t);
	static bool SegmentSpriteIntersection( sf::Vector2f p1, sf::Vector2f p2, sf::Sprite &S, TextureBitmap &bitmap, sf::Vector2f &t);
	static bool SpriteSpriteIntersection( sf::Sprite &s1, TextureBitmap &b1, sf::Sprite &s2, TextureBitmap &b2);
	static bool ShapeSpriteIntersection( sf::Shape  &sh, sf::Sprite &sp, TextureBitmap &b);
	static bool ShapeShapeIntersection( sf::Shape &s1, sf::Shape &s2, sf::Vector2f &hit);
	static bool RectangleShapeIntersection( sf::FloatRect r, sf::Shape &s);
	static bool RectangleSpriteIntersection( sf::FloatRect r, sf::Sprite &s, TextureBitmap &b );

};

#endif _CollisionTools