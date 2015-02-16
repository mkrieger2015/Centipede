#ifndef _PlayerManager
#define _PlayerManager

#include "Game Components\TEAL\CommonElements.h"

class Blaster;

class PlayerManager
{
private: 
	static PlayerManager *self;

	PlayerManager(){ };
	PlayerManager(const PlayerManager&){ };
	PlayerManager& operator= (const PlayerManager&){ };

	~PlayerManager(){ };
	
	static PlayerManager& Instance()
	{
		if (!self)
			self= new PlayerManager();
		return *self;
	};

	Blaster *current_player;
	Blaster *player_1;
	Blaster *player_2;
	Blaster *ai_player;

	int player_1_lives;
	int player_2_lives;
	
	static int selected_player;

public:
	static void Instantiate();
	static void CreatePlayer1();
	static void CreatePlayer2();
	static void CreateAIPlayer();
	static void SwapPlayers();
	static void SetPlayer(int which_player);
	static float ReturnPlayerX();
	static float ReturnPlayerY();
	static int ReturnLives();
	static int ReturnPlayer1Lives();
	static int ReturnPlayer2Lives();
	static void IncreaseLives();
	static void DecreaseLives();
	static void Respawn();
	static Blaster* ReturnCurrentPlayer();

	static void DeleteAllPlayers();
};

#endif