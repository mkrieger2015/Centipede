#ifndef _GameOverObject
#define _GameOverObject

#include "Game Components\TEAL\CommonElements.h"
#include <list>
#include "GameFont.h"

class GameOverObject : public GameObject
{
public:
	GameOverObject(){ };
	GameOverObject(int player);
	virtual void Alarm0();
	virtual void Draw();
private:
	GameFont myfont;
	std::list<Glyph> game_over_text;
	std::list<Glyph> player_text;
	sf::Vector2f player_nextpos;
	sf::Vector2f game_over_nextpos;
};

#endif 