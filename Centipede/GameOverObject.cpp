#include "GameOverObject.h"
#include "Game Components\MainScene.h"
#include "CommonFunctionsAndValues.h"
#include "GameManager.h"
#include "PlayerManager.h"
#include "HighScoreInput.h"
#include "ScoreManager.h"
#include "GridManager.h"

extern const float gameover_x;
extern const float gameover_y;
extern const float playertext_x;
extern const float playertext_y;


GameOverObject::GameOverObject(int player)
{
	//standard flyweight implementation (as used in HUDManager) to write two lines
	//"GAME OVER"
	//and
	//"PLAYER _" (depends on player parameter)
	//no real surprises here
	std::string player_string;
	std::string gameover= "GAME OVER";
	if(player == 1)
		player_string = "PLAYER 1";
	else if(player == 2)
		player_string= "PLAYER 2";

	player_nextpos.x= playertext_x;
	player_nextpos.y= playertext_y;
	game_over_nextpos.x= gameover_x;
	game_over_nextpos.y= gameover_y;

	for (int i=0; i < gameover.length(); i++)
	{
		game_over_text.push_back(  myfont.GetGlyph(gameover.at(i), game_over_nextpos) );
		GridManager::DeleteMushroomAtPosition(game_over_nextpos.x,game_over_nextpos.y);
		game_over_nextpos.x += myfont.CellWidth();
	}

	for (int i=0; i < player_string.length(); i++)
	{
		player_text.push_back(  myfont.GetGlyph(player_string.at(i), player_nextpos) );
		GridManager::DeleteMushroomAtPosition(player_nextpos.x,player_nextpos.y);
		player_nextpos.x += myfont.CellWidth();
	}
	//this item is to only be on screen for 3 seconds
	SetAlarm(0,3);
}

void GameOverObject::Draw()
{
	std::list<Glyph>::iterator it;
	for (it = game_over_text.begin(); it != game_over_text.end(); it++)
		it->Draw();
	for (it = player_text.begin(); it != player_text.end(); it++)
		it->Draw();
}

void GameOverObject::Alarm0()
{
	//if p1 got a high score, create a high score object for p1
	if(ScoreManager::ReturnP1Score() > ScoreManager::ReturnMinHighScore())
		new HighScoreInput(1);
	//if p1 didn't get a high score but p2 did, create a high score object for p2
	else if(ScoreManager::ReturnP2Score() > ScoreManager::ReturnMinHighScore())
		new HighScoreInput(2);
	//if neither got a high score, just reset the game
	else
		GameManager::ResetGame();

	MarkForDestroy();
}