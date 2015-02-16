#ifndef _Glyph
#define _Glyph

#include "SFML\Graphics.hpp"

class SpriteSheet; 

class Glyph
{
public:
	Glyph();
	Glyph( SpriteSheet* sheet, int cell, sf::Vector2f pos);
	void Draw();

private:
	SpriteSheet* spritesheet;
	int cell;
	sf::Vector2f position;
};


#endif _Glyph