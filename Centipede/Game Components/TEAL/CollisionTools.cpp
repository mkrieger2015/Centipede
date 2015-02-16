// CollisionTools.cpp
// Andre Berthiaume, June 2012
//
// Collection of basic functions to determine intersection, containment, etc.
// See also http://www.sfml-dev.org/wiki/en/sources/simple_collision_detection

#include "CollisionTools.h"
#include <limits>
#include "ConsoleMsg.h"
#include "TEALShow.h"
#include "Tools.h"

const float CollisionTools::epsilon = 0.0001f;// std::numeric_limits<float>::min();

CollisionTools::TextureBitmap CollisionTools::ContructTextureBitmap( sf::Image &img )
{
	return ContructTextureBitmap(img, sf::IntRect(0,0, img.getSize().x, img.getSize().y) );
}

CollisionTools::TextureBitmap CollisionTools::ContructTextureBitmap( sf::Image &img, sf::IntRect rect)
{
	CollisionTools::TextureBitmap bitmap;
	// Resize both dimentions of the vector of vectors
	bitmap.resize( rect.height );
	for (int i = 0; i < rect.height; ++i)
		bitmap[i].resize(rect.width);

	// Flip the boolean only is the bit alpha value isn't zero
	for( int h = 0; h < rect.height; h++)
		for( int w = 0; w < rect.width; w++)
			bitmap[h][w] = (img.getPixel(rect.left + w, rect.top + h).a != 0);

	return bitmap;
}

// Intersection of two segments
// See also http://community.topcoder.com/tc?module=Static&d1=tutorials&d2=geometry2#line_line_intersection
bool CollisionTools::SegmentSegmentIntersection( sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f q1, sf::Vector2f q2, sf::Vector2f &t)
{
	float A1 = p2.y-p1.y;
	float B1 = p1.x-p2.x;
	float C1 = A1*p1.x+B1*p1.y;

	float A2 = q2.y-q1.y;
	float B2 = q1.x-q2.x;
	float C2 = A2*q1.x+B2*q1.y;

	float det = A1*B2 - A2*B1;
    if(det == 0){
        return false; //Lines are parallel
    }else{
        t.x = (B2*C1 - B1*C2)/det;
        t.y = (A1*C2 - A2*C1)/det;

		// t is on the two lines, but is it on the segments?
		float minx = p1.x < p2.x ? p1.x : p2.x;
		float maxx = p1.x > p2.x ? p1.x : p2.x;
		if ( t.x < minx - epsilon || t.x > maxx + epsilon) return false;

		float miny = p1.y < p2.y ? p1.y : p2.y;
		float maxy = p1.y > p2.y ? p1.y : p2.y;
		if ( t.y < miny - epsilon || t.y > maxy + epsilon) return false;

		minx = q1.x < q2.x ? q1.x : q2.x;
		maxx = q1.x > q2.x ? q1.x : q2.x;
		if ( t.x < minx - epsilon || t.x > maxx + epsilon) return false;

		miny = q1.y < q2.y ? q1.y : q2.y;
		maxy = q1.y > q2.y ? q1.y : q2.y;
		if ( t.y < miny - epsilon || t.y > maxy + epsilon) return false;

		return true;
    }
}


// Distance from point C to line or segment formed by A and B
// See also http://community.topcoder.com/tc?module=Static&d1=tutorials&d2=geometry1#line_point_distance
float CollisionTools::LinePointDistance(  sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, bool isSegment)
{
	float dist = Tools::Cross( B-A, C-A ) / Tools::Length(B-A) ;
    if(isSegment)
	{
        float dot1 = Tools::Dot( C-B, B-A);
        if (dot1 > 0) return Tools::Length(B-C);
        float dot2 = Tools::Dot( C-A, A-B);
        if(dot2 > 0) return Tools::Length(A-C);
    }

    return fabs(dist);
}

// True if p is inside the convex shape define by the points in Poly
// See also http://community.topcoder.com/tc?module=Static&d1=tutorials&d2=geometry3
bool CollisionTools::PointInPolygon( sf::Vector2f Poly[], int vcount, sf::Vector2f p)
{
    int cnt = 0;
	sf::Vector2f RdmInf( rand()*1000+1000.0f, rand()*1000+1000.0f);
	sf::Vector2f dummy;

	std::string m = "";

    for(int i = 0; i < vcount; i++)
	{
		if( LinePointDistance(Poly[i], Poly[(i+1) % vcount] , p, true) == 0) // Exactly on the boundary...
            return true;
        if( SegmentSegmentIntersection( Poly[i], Poly[(i+1) % vcount], p, RdmInf, dummy) )
		{
            cnt++;
			m += Tools::ToString(i);
		}
    }

    if(cnt%2 == 0) return false;
    else return true;
}

// True is p (in global coord) is inside the shape S
bool CollisionTools::PointInShape( sf::Shape &S, sf::Vector2f p)
{
	int vcount = S.getPointCount();
    int cnt = 0;
	sf::Vector2f RdmInf(rand()*1000+1000.0f, rand()*1000+1000.0f);
	sf::Vector2f dummy;

	// convert segment to shape's local coord
	p = S.getInverseTransform().transformPoint(p);

    for(int i = 0; i < vcount; i++)
	{
        if( LinePointDistance( S.getPoint(i), S.getPoint((i+1) % vcount), p, true) == 0) // Exactly on the boundary...
            return true;
        if( SegmentSegmentIntersection( S.getPoint(i), S.getPoint((i+1) % vcount), p, RdmInf, dummy) )
            cnt++;
    }

    if(cnt%2 == 0) return false;
    else return true;

}

// True the segment p1-p2 (in global coord.) intersects S. If true, t is the intersection point closest to p1
bool CollisionTools::SegmentShapeIntersection( sf::Vector2f p1, sf::Vector2f p2, sf::Shape &S, sf::Vector2f& t)
{
	// If p1 is already inside S, we're done.
	if ( PointInShape( S, p1) )
	{
		t = p1;
		return true;
	}

	int vcount = S.getPointCount();
	bool found = false;
	sf::Vector2f temp = p2;

	// convert segment to shape's local coord
	p1 = S.getInverseTransform().transformPoint(p1);
	p2 = S.getInverseTransform().transformPoint(p2);

    for(int i = 0; i < vcount; i++)
	{
        if( SegmentSegmentIntersection( S.getPoint(i), S.getPoint((i+1) % vcount), p1, p2, t) )
		{
			if ( Tools::Length(t-p1) < Tools::Length(temp-p1)) // keep track of nearest intersection to p1
			{
				found = true;
				temp = t;
			}
		}
    }


	// convert back the nearest intersection to global coord
    t = S.getTransform().transformPoint(temp);

	return found;
}

bool CollisionTools::SegmentRectangleIntersection( sf::Vector2f p1, sf::Vector2f p2, sf::FloatRect &R, sf::Vector2f& t)
{
	bool found[4];
	sf::Vector2f temp[4];
	sf::Vector2f nearest;
	
	found[0] = SegmentSegmentIntersection(p1, p2, sf::Vector2f(R.left, R.top), sf::Vector2f(R.left + R.width, R.top), temp[0]); 
	found[1] = SegmentSegmentIntersection(p1, p2, sf::Vector2f(R.left, R.top + R.height), sf::Vector2f(R.left + R.width, R.top + R.height), temp[1]);
	found[2] = SegmentSegmentIntersection(p1, p2, sf::Vector2f(R.left, R.top), sf::Vector2f(R.left, R.top + R.height), temp[2]); 
	found[3] = SegmentSegmentIntersection(p1, p2, sf::Vector2f(R.left + R.width, R.top), sf::Vector2f(R.left + R.width, R.top + R.height), temp[3]);

	// find the first valid intersection point, if any
	int i = 0;
	while (i < 4 && ! found[i] ) { i++; }
	if (i>=4) {
		return false; 
	}
	
	// now we find the closest intersection point to p1
	nearest = temp[i];
	for (int j = i; j < 4; j++)
	{
		if (found[j] && Tools::LengthSquare(nearest - p1) > Tools::LengthSquare(temp[j] - p1))
			nearest = temp[j];
	}

	t = nearest;
	return true;
}

// Finds whether there is intersection betweeen p1-p2 and Bounding box (not AABB!) of S. 
// In case of intersection, t is the point on the segment nearest p1 that intersects S.
bool CollisionTools::SegmentSpriteIntersection( sf::Vector2f p1, sf::Vector2f p2, sf::Sprite &S, TextureBitmap &bitmap, sf::Vector2f &t)
{
	sf::Vector2f boxstart, boxend;

	sf::FloatRect BBox = S.getLocalBounds();	// bounding rectangle for S
	BBox.width = BBox.width - 1;				// Adjusments so that coordiante on both far edges are within the bitmap
	BBox.height = BBox.height - 1;

	//TEALShow::Box( BBox, S.getTransform(), sf::Color(255,255,0,50) );

	// Convert segment to local coordinate
	p1 = S.getInverseTransform().transformPoint(p1);
	p2 = S.getInverseTransform().transformPoint(p2);

	if ( S.getLocalBounds().contains(p1) ) 
		boxstart = p1; // Segment starts inside the sprite's bounding box
	else if ( ! CollisionTools::SegmentRectangleIntersection(p1, p2, BBox, boxstart))
		return false; // p1 is outside BBox and no intersection at all
	// at this point, boxstart is either inside the sprite's bounding box or on its edge

	if ( S.getLocalBounds().contains(p2) ) boxend = p2;
	else if ( ! CollisionTools::SegmentRectangleIntersection(p2, p1, BBox, boxend) )
		return false; // occurs when boxstart is on the edge and p2 is pointing away from the box
	// at this point, boxend is either inside the sprite's bounding box or on its edge
			
	// Now we 'walk' along the segment one pixel at a time.
	sf::Vector2f delta = boxend - boxstart;
	sf::Vector2f localoffset = Tools::Normalize(delta);		// Compute the 'slope' of the line, per step
	int maxstep = (int) Tools::Length(delta) + 1;			// Number of steps/pixel to walk along the slope
		
	// We try every points between boxstart and boxend along the slope localoffset until we find a non alpha pixel.
	sf::Vector2f currentpoint = boxstart;
	int i=0;
	sf::IntRect TextSubRect = S.getTextureRect(); // If animated sprite, look at the bitmap relative to the proper subrectangle
	while ( i < maxstep )
	{
		//TEALShow::Mark(S.getTransform().transformPoint(currentpoint), sf::Color::Green);

		if ( bitmap[ TextSubRect.top + (int) currentpoint.y][ TextSubRect.left + (int) currentpoint.x ] )
		{
			t = S.getTransform().transformPoint(currentpoint);
			return true;
		}

		currentpoint += localoffset;
		i++;
	}

	return false;
}

sf::FloatRect CollisionTools::GetLocalOverlapRect( sf::FloatRect &r1, const sf::Transform &t1, sf::FloatRect &r2, const sf::Transform &t2) 
{
	// Used to transforms points to either local coordinates
	sf::Transform r2toLocalr1 = t1.getInverse() * t2;

	// get r2's AABB in r1's coordinate
	sf::FloatRect r1LocalAABB = r2toLocalr1.transformRect(r2);

	// Get the portion of s1 that overlaps with s1LocalAABB (the only part worth scanning)
	sf::FloatRect Overlap;
	r1.intersects(r1LocalAABB, Overlap);

	return Overlap;
}

bool CollisionTools::SpriteSpriteIntersection( sf::Sprite &s1, TextureBitmap &b1, sf::Sprite &s2, TextureBitmap &b2)
{
	//TEALShow::OrientedBox(  s1, sf::Color(80,80,80,128) );
	//TEALShow::OrientedBox(  s2, sf::Color(80,80,0,128) );
	//TEALShow::AABB(  s1, sf::Color(80,80,80,192) );
	//TEALShow::AABB(  s2, sf::Color(80,80,0,192) );

	// Get the portion of s1 that overlaps with s2 (the only part worth scanning)
	sf::FloatRect ScanRect = GetLocalOverlapRect(s1.getLocalBounds(), s1.getTransform(), s2.getLocalBounds(), s2.getTransform() );

	int ystop = (int) (ScanRect.top + ScanRect.height);		// Precompute loop
	int xstop = (int) (ScanRect.left + ScanRect.width);		// end points
	sf::Transform s1toLocals2 = s2.getInverseTransform() * s1.getTransform(); // transforms points from s1 to local s2 coordinates
	sf::Vector2f p;

	sf::IntRect TextSubRect1 = s1.getTextureRect(); // If animated sprite, look at the bitmap relative to the proper subrectangle
	sf::IntRect TextSubRect2 = s2.getTextureRect(); // If animated sprite, look at the bitmap relative to the proper subrectangle

	// We try every pixel within the overlapping rectangle
	for( int y = (int) ScanRect.top; y < ystop; y++)
		for ( int x = (int) ScanRect.left; x < xstop; x++)
		{
			if (b1[TextSubRect1.top + y][TextSubRect1.left + x]) // early exit from more work
			{
				p = s1toLocals2.transformPoint( (float) x, (float) y);

				if ( p.x >= 0 && p.x < TextSubRect2.width && p.y >= 0 && p.y < TextSubRect2.height 
					 && b2[ TextSubRect2.top + (unsigned int) p.y][ TextSubRect2.left + (unsigned int) p.x] )
				{
					//TEALShow::Mark( s1.getTransform().transformPoint(x,y), sf::Color::Yellow);
					return true;
				}
				//else
					//TEALShow::Mark( s1.getTransform().transformPoint(x,y), sf::Color(255,255,255,60));
			}
			
		}

	return false;
}

bool CollisionTools::PointInSprite( sf::Vector2f p, sf::Sprite &s, TextureBitmap &b )
{
	// get p in local space
	p = s.getInverseTransform().transformPoint(p);

	sf::IntRect TextSubRect = s.getTextureRect(); // If animated sprite, look at the bitmap relative to the proper subrectangle
	if ( p.x >= 0 && p.x < TextSubRect.width && p.y >= 0 && p.y < TextSubRect.height
		&& b[TextSubRect.top + (unsigned int) p.y][ TextSubRect.left + (unsigned int) p.x] )
	{
		return true;
	}
	else
		return false;
}

bool CollisionTools::ShapeSpriteIntersection( sf::Shape  &sh, sf::Sprite &sp, TextureBitmap &b)
{
	//TEALShow::OrientedBox(  sp, sf::Color(80,80,80,128) );
	//TEALShow::OrientedBox(  sh, sf::Color(80,80,0,128) );
	//TEALShow::AABB(  sp, sf::Color(80,80,80,192) );
	//TEALShow::AABB(  sh, sf::Color(80,80,0,192) );

	// Get the portion of sp that needs scanning
	sf::FloatRect ScanRect = GetLocalOverlapRect(sp.getLocalBounds(), sp.getTransform(), sh.getLocalBounds(), sh.getTransform() );

	int ystop = (int) (ScanRect.top + ScanRect.height);		// Precompute loop
	int xstop = (int) (ScanRect.left + ScanRect.width);		// end points
	sf::Vector2f p;

	sf::IntRect TextSubRect = sp.getTextureRect(); // If animated sprite, look at the bitmap relative to the proper subrectangle

	// We try every pixel within the overlapping rectangle
	for( int y = (int) ScanRect.top; y < ystop; y++)
		for ( int x = (int) ScanRect.left; x < xstop; x++)
		{
			if (b[TextSubRect.top + y][TextSubRect.left + x]) // early exit from more work
			{
				p = sp.getTransform().transformPoint( (float) x, (float) y);

				if ( PointInShape( sh, p) )
				{
					//TEALShow::Mark( p, sf::Color::Yellow);
					return true;
				}
				//else
					//TEALShow::Mark( p, sf::Color(255,255,255,60));
			}
			
		}

	return false;
}

bool CollisionTools::ShapeShapeIntersection( sf::Shape &s1, sf::Shape &s2, sf::Vector2f &hit)
{
	int vcount = s2.getPointCount();

	// Test every segment of s2 for intersection with s1
    for(int i = 0; i < vcount; i++)
	{
		//TEALShow::Line( s2.getTransform().transformPoint( s2.getPoint(i)), s2.getTransform().transformPoint( s2.getPoint((i+1) % vcount)), sf::Color::Green );
        if( SegmentShapeIntersection( s2.getTransform().transformPoint( s2.getPoint(i)) , s2.getTransform().transformPoint( s2.getPoint((i+1) % vcount)), s1, hit) )
		{
			return true;
		}
    }

	// Test is s1 is actually inside s2 (we only need to try one point)
	if ( PointInShape( s2, s1.getTransform().transformPoint( s1.getPoint(0))) ) 
	{
		hit = s1.getTransform().transformPoint( s1.getPoint(0));
		return true;
	}

	return false;
}

bool CollisionTools::RectangleShapeIntersection( sf::FloatRect r, sf::Shape &s)
{
	//TEALShow::Box(r, sf::Color(255,255,255,80));

	// If s is already inside r, we're done.
	if ( r.contains( s.getTransform().transformPoint(s.getPoint(0))) )
	{
		return true;
	}
	else 
	{
		sf::Vector2f dummy, UL, UR, LL, LR;
		UL = sf::Vector2f( r.left, r.top);
		UR = UL + sf::Vector2f( r.width, 0);
		LR = UR + sf::Vector2f( 0, r.height);
		LL = UL + sf::Vector2f( 0, r.height);

		if (  SegmentShapeIntersection( UL, UR, s, dummy) ||
			  SegmentShapeIntersection( UR, LR, s, dummy) ||
			  SegmentShapeIntersection( LR, LL, s, dummy) ||
			  SegmentShapeIntersection( LL, UL, s, dummy) ) 
		{
			//TEALShow::Mark(dummy, sf::Color::Yellow);
			return true;
		}

		return false;
	}
}


bool CollisionTools::RectangleSpriteIntersection( sf::FloatRect r, sf::Sprite &s, TextureBitmap &b )
{
	//TEALShow::Box(r, sf::Color(255,255,255,80));
	//TEALShow::OrientedBox(s, sf::Color(255,255,255,80));
	//TEALShow::AABB(s, sf::Color(255,255,255,80));

	sf::Vector2f dummy, UL, UR, LL, LR;
	UL = sf::Vector2f( r.left, r.top);
	UR = UL + sf::Vector2f( r.width, 0);
	LR = UR + sf::Vector2f( 0, r.height);
	LL = UL + sf::Vector2f( 0, r.height);

	// Easy case: Do any of sides of r run through s?
	if (  SegmentSpriteIntersection( UL, UR, s, b, dummy) ||
			SegmentSpriteIntersection( UR, LR, s, b, dummy) ||
			SegmentSpriteIntersection( LR, LL, s, b, dummy) ||
			SegmentSpriteIntersection( LL, UL, s, b, dummy) ) 
	{
		//TEALShow::Mark(dummy, sf::Color::Red);
		return true;
	}
	else // no side of r runs through s, so we do a pixel scan:
		 // (Note: Assuming that we reach here only if s is either completely inside r or completely outside is wrong for textures with non-contiguous pixel arrangement)
	{
		// Get the portion of s that overlaps with r (the only part worth scanning)
		sf::FloatRect ScanRect; 
		r.intersects( s.getGlobalBounds(), ScanRect);					// Intersection of s's AABB and and r in global coordinate (note ScanRect is axis aligned)
		ScanRect = s.getInverseTransform().transformRect(ScanRect);		// Moves the above intersection to s's local coordinate (parts of it will lie outside s's local bounds) 
		s.getLocalBounds().intersects(ScanRect, ScanRect);				// Take only the part that _is_ inside s's local bounds

		int ystop = (int) (ScanRect.top + ScanRect.height);		// Precompute loop
		int xstop = (int) (ScanRect.left + ScanRect.width);		// end points
		sf::Vector2f p;

		//TEALShow::Box( s.getLocalBounds(), sf::Color::White);
		//TEALShow::Box( ScanRect, sf::Color::Green);

		sf::IntRect TextSubRect = s.getTextureRect(); // If animated sprite, look at the bitmap relative to the proper subrectangle

		// We try every pixel within the overlapping rectangle
		for( int y = (int) ScanRect.top; y < ystop; y++)
			for ( int x = (int) ScanRect.left; x < xstop; x++)
			{
				if (b[ TextSubRect.top + y][ TextSubRect.left + x]) // early exit from more work
				{
					p = s.getTransform().transformPoint( (float) x, (float) y); // transforms points from s to global coordinates

					if ( r.contains(p) )
					{
						//TEALShow::Mark( s.getTransform().transformPoint(x,y), sf::Color::Red);
						return true;
					}
					//else
						//TEALShow::Mark( s.getTransform().transformPoint(x,y), sf::Color::Yellow );
				}
				//else
						//TEALShow::Mark( s.getTransform().transformPoint(x,y), sf::Color(255,255,255,20));
			}

		return false;

	}
}