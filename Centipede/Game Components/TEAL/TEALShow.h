// TEALShow.h
// Andre Berthiaume, June 2012
//
// Debugging tool to easily visualize graphical elements
//

#ifndef _TEALShow
#define _TEALShow

#include <list>
#include <string>
#include "SFML\Graphics.hpp"

class TEALShow
{

public:
		static int linecount;
private:
	static TEALShow* TSInstance;			// Game set up as a singleton
	TEALShow(){};							// Private constructor
	TEALShow(const TEALShow&);				// Prevent copy-construction
	TEALShow& operator=(const TEALShow&);	// Prevent assignment

	static TEALShow& Instance()				// Access reference (all public methods will be static)
	{
		if (! TSInstance) 
			TSInstance = new TEALShow;
		return *TSInstance;
	};

	// Text monitor support
	static sf::Text Monitor;
	static std::string Message;
	static sf::Color MonitorCol;

	// Line support
	static const int MAXCOUNT;
	static sf::Vertex Lines[];
	//static int linecount;

	// Linestrips support
	struct Linestrip
	{
		int size;
		std::vector<sf::Vertex> points;
	};
	static std::list<Linestrip> LinestripList;
	static void AddTransformedRectanglePoints( sf::FloatRect r, sf::Transform T, sf::Color col );

	// Shape support
	static sf::Shape* ShapeList[];
	static int shapecount;

	// Label support
	static std::list<sf::Text> LabelList;


public:
	static void Text( std::string msg );
	static void TextColor( sf::Color c){ MonitorCol = c;};
	static void Line( sf::Vector2f p1, sf::Vector2f p2, sf::Color c);
	static void Mark( sf::Vector2f p, sf::Color col, float radius = 3 );
	static void Dot( sf::Vector2f p, sf::Color col, float radius = 3 );
	static void Box( sf::FloatRect r, sf::Color col);
	static void Box( sf::FloatRect r, sf::Transform T, sf::Color col);
	static void Circle( sf::Vector2f center, float radius, sf::Color col);
	static void AABB( sf::Shape &s, sf::Color col);
	static void AABB( sf::Sprite &s, sf::Color col);
	static void OrientedBox( sf::Sprite &s, sf::Color col);
	static void OrientedBox( sf::Shape &s, sf::Color col);
	static void Label( std::string label, sf::Vector2f pos, sf::Color col );
	static void Draw();
};

#endif _TEALShow