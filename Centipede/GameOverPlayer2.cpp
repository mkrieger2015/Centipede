#include "GameOverPlayer2.h"
#include "Game Components\MainScene.h"
#include "CommonFunctionsAndValues.h"
#include "GameManager.h"
#include "PlayerManager.h"

extern const float gameover_x;
extern const float gameover_y;
extern const float playertext_x;
extern const float playertext_y;

GameOverPlayer2::GameOverPlayer2()
{
	std::string gameover= "GAME OVER";
	std::string player2= "PLAYER 2";

	player_2_nextpos.x= playertext_x;
	player_2_nextpos.y= playertext_y;
	game_over_nextpos.x= gameover_x;
	game_over_nextpos.y= gameover_y;

	for (int i=0; i < gameover.length(); i++)
	{
		game_over_text.push_back(  myfont.GetGlyph(gameover.at(i), game_over_nextpos) );
		game_over_nextpos.x += myfont.CellWidth();
	}

	for (int i=0; i < player2.length(); i++)
	{
		player_2_text.push_back(  myfont.GetGlyph(player2.at(i), player_2_nextpos) );
		player_2_nextpos.x += myfont.CellWidth();
	}
	SetAlarm(0,3);
}

void GameOverPlayer2::Draw()
{
	std::list<Glyph>::iterator it;
	for (it = game_over_text.begin(); it != game_over_text.end(); it++)
		it->Draw();
	for (it = player_2_text.begin(); it != player_2_text.end(); it++)
		it->Draw();
}

void GameOverPlayer2::Alarm0()
{
	if(PlayerManager::ReturnPlayer1Lives() > 0)
		GameManager::SwapPlayers();
	else if(PlayerManager::ReturnPlayer1Lives() <= 0)
		GameManager::ResetGame();
	MarkForDestroy();
}