#include "HighScoreInput.h"
#include "Game Components\MainScene.h"
#include "CommonFunctionsAndValues.h"
#include "GameManager.h"
#include "ScoreManager.h"
#include "PlayerManager.h"
#include "GridManager.h"

extern const float greatscore_x;
extern const float greatscore_y;
extern const float typeyourinitials_x;
extern const float typeyourinitials_y;
extern const float input_x;
extern const float input_y;


HighScoreInput::HighScoreInput(int player)
{
	//much like the HUD manager, draws two lines of text
	//"GREAT SCORE P_" (depending on player)
	//and
	//"TYPE YOUR INITIALS"
	//follows typical flyweight string conventions
	RegisterInput( InputFlags::TextEntered );
	initials= 0;
	selected_player= player;
	std::string greatscore;
	if(player == 1)
		greatscore = "GREAT SCORE P1";
	else if(player == 2)
		greatscore= "GREAT SCORE P2";
	std::string typeyourinitials= "TYPE YOUR INITIALS";

	great_score_nextpos.x= greatscore_x;
	great_score_nextpos.y= greatscore_y;
	type_your_initials_nextpos.x= typeyourinitials_x;
	type_your_initials_nextpos.y= typeyourinitials_y;
	input_nextpos.x= input_x;
	input_nextpos.y= input_y;

	for (int i=0; i < greatscore.length(); i++)
	{
		great_score_text.push_back(  myfont.GetGlyph(greatscore.at(i), great_score_nextpos) );
		GridManager::DeleteMushroomAtPosition(great_score_nextpos.x,great_score_nextpos.y);
		great_score_nextpos.x += myfont.CellWidth();
	}

	for (int i=0; i < typeyourinitials.length(); i++)
	{
		type_your_initials_text.push_back(  myfont.GetGlyph(typeyourinitials.at(i), type_your_initials_nextpos) );
		GridManager::DeleteMushroomAtPosition(type_your_initials_nextpos.x,type_your_initials_nextpos.y);
		type_your_initials_nextpos.x += myfont.CellWidth();
	}
}

void HighScoreInput::TextEntered(sf::Uint32 UnicodeValue)
{
	//takes up to 3 key press entries for initials, and handles them like a typical flyweight implementation
	std::string tempstring;
	initials++;
	if(initials <= 3)
	{
		tempstring= Tools::ToString((char) UnicodeValue);
		output_string+= tempstring;
		input.push_back( myfont.GetGlyph( (char) UnicodeValue, input_nextpos ) );
		GridManager::DeleteMushroomAtPosition(input_nextpos.x,input_nextpos.y);
		input_nextpos.x += myfont.CellWidth();
	}
	//on the 4th key input, it tells the score manager to insert the high score, and then waits one second
	if(initials > 3)
	{
		if(selected_player == 1)
			ScoreManager::InsertHighScore(output_string,ScoreManager::ReturnP1Score());
		else if(selected_player == 2)
			ScoreManager::InsertHighScore(output_string,ScoreManager::ReturnP2Score());
		SetAlarm(0,1);
	}
}

void HighScoreInput::Draw()
{
	//draws all glyph lists
	std::list<Glyph>::iterator it;
	for (it = great_score_text.begin(); it != great_score_text.end(); it++)
		it->Draw();
	for (it = type_your_initials_text.begin(); it != type_your_initials_text.end(); it++)
		it->Draw();
	for (it = input.begin(); it != input.end(); it++)
		it->Draw();
}

void HighScoreInput::Alarm0()
{
	//if this is a 2 player game and p1 is selected...
	if(selected_player == 1 && GameManager::ReturnGameType() == 2)
	{
		//check to see if p2 got a high score as well. if he did, create a new high score input prompt for p2
		if(ScoreManager::ReturnP2Score() > ScoreManager::ReturnMinHighScore())
			new HighScoreInput(2);
		//if not, reset the game
		else if(ScoreManager::ReturnP2Score() <= ScoreManager::ReturnMinHighScore())
			GameManager::ResetGame();
	}
	//if this is a 2 player game and player 2 is selected, simply reset the game
	else if(selected_player == 2 && GameManager::ReturnGameType() == 2)
		GameManager::ResetGame();
	//same deal goes when the game is in 1 player mode
	else if(GameManager::ReturnGameType() == 1)
		GameManager::ResetGame();

	DeregisterInput();
	MarkForDestroy();
	
}