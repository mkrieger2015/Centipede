#ifndef _GameOverPlayer2
#define _GameOverPlayer2

#include "Game Components\TEAL\CommonElements.h"
#include <list>
#include "GameFont.h"

class GameOverPlayer2 : public GameObject
{
public:
	GameOverPlayer2();
	virtual void Alarm0();
	virtual void Draw();
private:
	GameFont myfont;
	std::list<Glyph> game_over_text;
	std::list<Glyph> player_2_text;
	sf::Vector2f player_2_nextpos;
	sf::Vector2f game_over_nextpos;
};

#endif 