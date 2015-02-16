#ifndef _HighScoreInput
#define _HighScoreInput

#include "Game Components\TEAL\CommonElements.h"
#include <list>
#include "GameFont.h"

class HighScoreInput : public GameObject
{
public:
	HighScoreInput(){ };
	HighScoreInput(int player);
	virtual void TextEntered(sf::Uint32 UnicodeValue);
	virtual void Alarm0();
	virtual void Draw();
private:
	GameFont myfont;
	std::list<Glyph> great_score_text;
	std::list<Glyph> type_your_initials_text;
	std::list<Glyph> input;
	sf::Vector2f great_score_nextpos;
	sf::Vector2f type_your_initials_nextpos;
	sf::Vector2f input_nextpos;
	int initials;
	int selected_player;
	std::string output_string;
};

#endif 