#include "Glyph.h"
#include "SpriteSheet.h"
#include "Game Components\TEAL\ResourceManager.h"

Glyph::Glyph()
{
	spritesheet = 0;
	cell = 0; 
	position = sf::Vector2f(0,0);
}

Glyph::Glyph( SpriteSheet* sheet, int cellInd, sf::Vector2f pos )
{
	spritesheet = sheet;
	position = pos;
	cell = cellInd;
}

void Glyph::Draw()
{
	spritesheet->Draw(cell, position);
}
