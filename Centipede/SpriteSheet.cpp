#include "SpriteSheet.h"
#include "Game Components\TEAL\WindowManager.h" 

SpriteSheet::SpriteSheet( sf::Texture& tex, int cellsWide, int cellsHigh )
{
	MainSprite = sf::Sprite(tex);

	// Rectangle dimension corresponding to a single cell of the sprite sheet
	cellWidth = tex.getSize().x / cellsWide;
	cellHeight = tex.getSize().y / cellsHigh;

	cells.resize(cellsWide * cellsHigh );

	// Determine the rectangle for each cell
	int left;
	int top;
	for (int j = 0; j < cellsHigh; j++)
	{
		top = j * cellHeight;
		for (int i = 0; i < cellsWide; i++)
		{
			left = i * cellWidth;
			cells[j*cellsWide + i] = sf::IntRect(left, top, cellWidth, cellHeight);
		}
	}
}

int SpriteSheet::CellWidth(){ 
	return cellWidth;
}

int SpriteSheet::CellHeight(){
	return cellHeight;
}

Glyph SpriteSheet::GetGlyph( char c, sf::Vector2f pos )
{
	return Glyph( this, CharToIndex(c), pos);
}

void SpriteSheet::Draw( int i, sf::Vector2f pos )
{
	MainSprite.setTextureRect(cells[i]);		// These two operations are fast to perform
	MainSprite.setPosition(pos);

	WindowManager::MainWindow.draw( MainSprite );
}