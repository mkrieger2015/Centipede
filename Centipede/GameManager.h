#ifndef _GameManager
#define _GameManagerManager

#include "Game Components\TEAL\CommonElements.h"
#include <list>

class GameManager
{
private:
	static GameManager *self;

	GameManager(){ };
	GameManager(const GameManager&){ };
	GameManager& operator= (const GameManager&){ };

	~GameManager(){ };
	
	static GameManager& Instance()
	{
		if (!self)
			self= new GameManager();
		return *self;
	};

	int game_type;

public:

	static void Begin();
	static void ResetGame();
	static void OnePlayerGame();
	static void TwoPlayerGame();
	static int ReturnGameType();
	static void SwapPlayers();

};

#endif _GameManager