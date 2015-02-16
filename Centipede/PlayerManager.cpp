 #include "PlayerManager.h"
#include "Blaster.h"
#include "BlasterFactory.h"
#include "CommonFunctionsAndValues.h"
#include "GameOverObject.h"
#include "GameManager.h"

int PlayerManager::selected_player= 0;
PlayerManager* PlayerManager::self= NULL;
extern const int initial_lives;

void PlayerManager::Instantiate()
{
	// set player pointers to null
	Instance();
	Instance().current_player= NULL;
	Instance().player_1= NULL;
	Instance().player_2= NULL;
	Instance().ai_player= NULL;
	//read in initial lives values
	Instance().player_1_lives= initial_lives;
	Instance().player_2_lives= initial_lives;
	//create the ai player and set it to the selected player 
	Instance().ai_player= BlasterFactory::CreateBlaster(false);
	Instance().current_player= Instance().ai_player;
	Instance().selected_player= 3;
}

void PlayerManager::CreatePlayer1()
{
	//create player 1 and set it to the selected player
	Instance().player_1= BlasterFactory::CreateBlaster(true);
	Instance().current_player= Instance().player_1;
	Instance().selected_player= 1;
}

void PlayerManager::CreatePlayer2()
{
	//create player 2 and set it to the selected player
	Instance().player_2= BlasterFactory::CreateBlaster(true);
	Instance().current_player= Instance().player_2;
	Instance().selected_player= 2;
}

void PlayerManager::CreateAIPlayer()
{
	//create ai player and set it to the selected player (redundant, but leaving in for designers)
	Instance().ai_player= BlasterFactory::CreateBlaster(false);
	Instance().current_player= Instance().ai_player;
	Instance().selected_player= 3;
}

void PlayerManager::SwapPlayers()
{
	//if selected player is p1, create a new player for p2 and destroy p1
	//then set selected player to p2
	if(Instance().current_player == Instance().player_1)
	{
		Instance().player_2= BlasterFactory::CreateBlaster(true);
		Instance().current_player= Instance().player_2;
		Instance().selected_player= 2;
		Instance().player_1->MarkForDestroy();
	}
	//if selected player is p2, create a new player for p1 and destroy p2
	//then set selected player to p1
	else if (Instance().current_player == Instance().player_2)
	{
		Instance().player_1= BlasterFactory::CreateBlaster(true);
		Instance().current_player= Instance().player_1;
		Instance().selected_player= 1;
		Instance().player_2->MarkForDestroy();
	}
	//if selected player is the ai, create a new player for p1 and destroy the ai
	//then set selected player to p1
	else if (Instance().current_player == Instance().ai_player)
	{
		Instance().player_1= BlasterFactory::CreateBlaster(true);
		Instance().current_player= Instance().player_1;
		Instance().selected_player= 1;
		Instance().ai_player->MarkForDestroy();
	}
}

void PlayerManager::SetPlayer(int which_player)
{
	//like above function, but lets you manually set player. Not currently used, but kept in for designers
	if(which_player == 1)
	{
		if(Instance().player_1 == NULL)
		{
			CreatePlayer1();
		}
		Instance().current_player= Instance().player_1;
		Instance().selected_player= 1;
	}
	else if (Instance().selected_player == 2)
	{
		if(Instance().player_2 == NULL)
		{
			CreatePlayer2();
		}
		Instance().current_player= Instance().player_2;
		Instance().selected_player= 2;
	}
	else if (Instance().selected_player == 3)
	{
		if(Instance().ai_player == NULL)
		{
			CreateAIPlayer();
		}
		Instance().current_player= Instance().ai_player;
		Instance().selected_player= 3;
	}
}

float PlayerManager::ReturnPlayerX()
{
	//returns x position 
	return Instance().current_player->ReturnX();
}

float PlayerManager::ReturnPlayerY()
{
	//returns y position
	return Instance().current_player->ReturnY();
}

Blaster* PlayerManager::ReturnCurrentPlayer()
{
	return Instance().current_player;
}

int PlayerManager::ReturnLives()
{
	//returns lives of currently selected player 
	//for specific lives, see the two functions below
	if(Instance().current_player == Instance().player_1)
		return Instance().player_1_lives;
	else if(Instance().current_player == Instance().player_2)
		return Instance().player_2_lives;
}

int PlayerManager::ReturnPlayer1Lives()
{
	return Instance().player_1_lives;
}

int PlayerManager::ReturnPlayer2Lives()
{
	return Instance().player_2_lives;
}

void PlayerManager::IncreaseLives()
{
	//increases lives of currently selected player
	if(Instance().current_player == Instance().player_1)
		Instance().player_1_lives++;
	else if(Instance().current_player == Instance().player_2)
		Instance().player_2_lives++;
}

void PlayerManager::DecreaseLives()
{
	//decreases lives of currently selected player
	if(Instance().current_player == Instance().player_1)
		Instance().player_1_lives--;
	else if(Instance().current_player == Instance().player_2)
		Instance().player_2_lives--;
}

void PlayerManager::Respawn()
{
	//if the game is 1 player mode...
	if(GameManager::ReturnGameType() == 1)
	{
		//if player 1 is selected (as in, NOT the ai)
		if(Instance().current_player == Instance().player_1)
		{
			//if player 1 has more lives
			if(Instance().player_1_lives > 0)
				//respawn player 1
				Instance().current_player->Respawn();
			//if not
			else if (Instance().player_1_lives <= 0)
				//game over
				new GameOverObject(1);
		}
	}
	//if the game is 2 player mode...
	else if(GameManager::ReturnGameType() == 2)
	{
		//if player 1 is selected
		if(Instance().current_player == Instance().player_1)
		{
			//if player 2 still has lives...
			if(Instance().player_2_lives > 0)
				//then swap players
				GameManager::SwapPlayers();
			//if player 2 is dead
			else if (Instance().player_2_lives <= 0)
			{
				//if player 1 has more lives
				if(Instance().player_1_lives > 0)
					//respawn player 1
					Instance().current_player->Respawn();
				//if not
				else if(Instance().player_1_lives <= 0)
					//game over
					new GameOverObject(1);
			}
		}
		//if player 2 is selected
		else if(Instance().current_player == Instance().player_2)
		{
			//if player 1 still has lives...
			if(Instance().player_1_lives > 0)
				//then swap players
				GameManager::SwapPlayers();
			//if player 1 is dead
			else if (Instance().player_1_lives <= 0)
			{
				//if player 2 has more lives
				if(Instance().player_2_lives > 0)
					//respawn player 1
					Instance().current_player->Respawn();
				//if not
				else if(Instance().player_2_lives <= 0)
					//game over
					new GameOverObject(2);
			}
		}
	}
	//if the game is in ai mode, simply respawn the ai
	else if(GameManager::ReturnGameType() == 0)
		Instance().current_player->Respawn();
}

void PlayerManager::DeleteAllPlayers()
{
	if(Instance().player_1 != NULL)
		Instance().player_1->MarkForDestroy();
	if(Instance().player_2 != NULL)
		Instance().player_2->MarkForDestroy();
	if(Instance().ai_player != NULL)
		Instance().ai_player->MarkForDestroy();
}