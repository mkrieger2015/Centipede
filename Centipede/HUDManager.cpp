#include "HUDManager.h"
#include "ScoreManager.h"
#include "PlayerManager.h"
#include "GridManager.h"
#include "CommonFunctionsAndValues.h"

extern const float pixelvalue;
extern const int max_digits;
extern const float p1_score_position;
extern const float p2_score_position;
extern const float hs_score_position;
extern const float p1_lives_position;
extern const float p2_lives_position;
extern const float high_score_position_x;
extern const float high_score_position_y;
extern const float hs1_position_x;
extern const float hs1_position_y;
extern const float credits_2_position_x;
extern const float credits_2_position_y;
extern const float bonus_every_12000_position_x;
extern const float bonus_every_12000_position_y;

HUDManager* HUDManager::self= NULL;

void HUDManager::Instantiate()
{
	Instance();
	
	//load blaster sprite for lives 
	Instance().blaster_sprite = AnimatedSprite(  ResourceManager::GetTexture("Blaster"), 2, 1);
	Instance().blaster_sprite.SetAnimation(1,1,false,false); 

	Instance().blaster_sprite.setOrigin(0,0);
	Instance().blaster_sprite.setScale(1,1);

	//set the current score and score drawing position to player 1 by default
	Instance().current_score = &Instance().p1_score;
	Instance().current_nextpos = &Instance().p1_nextpos;
	Instance().p1_scorepos= p1_score_position;
	Instance().p2_scorepos= p2_score_position;
	Instance().current_scorepos= &Instance().p1_scorepos;
}

void HUDManager::UpdatePlayerScore(int new_score)
{
	//empty the score's glyph list
	while(Instance().current_score->size() != 0)
	{
		Instance().current_score->pop_back();
	}
	std::string score;
	//convert the new score to a string
	score= Tools::ToString(new_score);
	//if the score is less than 10, make sure to add the extra "0" to the string
	if(score.length() <= 1)
	{
		score= "0" + score;
	}	

	//write the new score into the glyph list
	*Instance().current_nextpos = sf::Vector2f(*Instance().current_scorepos+(((max_digits - score.length())*Instance().myfont.CellWidth())),0);
	
	for (int i=0; i < score.length(); i++)
	{
		Instance().current_score->push_back( Instance().myfont.GetGlyph(score.at(i), *Instance().current_nextpos) );
		Instance().current_nextpos->x += Instance().myfont.CellWidth();
	}
}

void HUDManager::UpdateHighScore(int new_score)
{
	//identical to the above function, except this updates the high score at the top center of the screen
	while(Instance().high_score.size() != 0)
	{
		Instance().high_score.pop_back();
	}
	std::string score;
	score= Tools::ToString(new_score);
	if(score.length() <= 1)
	{
		score= "0" + score;
	}
	Instance().hs_nextpos = sf::Vector2f(hs_score_position+(((max_digits - score.length())*Instance().myfont.CellWidth())),0);
	
	for (int i=0; i < score.length(); i++)
	{
		Instance().high_score.push_back(  Instance().myfont.GetGlyph(score.at(i), Instance().hs_nextpos) );
		Instance().hs_nextpos.x += Instance().myfont.CellWidth();
	}
}

void HUDManager::Draw()
{
	//gets the lives for p1 and p2 from the player manager
	int p1_lives= PlayerManager::ReturnPlayer1Lives();
	int p2_lives= PlayerManager::ReturnPlayer2Lives();
	float lives_position_offset= 0;
	std::list<Glyph>::iterator it;

	//using a glyph iterator, simply navigate through all the scores and draw them
	for (it = Instance().p1_score.begin(); it != Instance().p1_score.end(); it++)
		it->Draw();
	for (it = Instance().high_score.begin(); it != Instance().high_score.end(); it++)
		it->Draw();
	for (it = Instance().p2_score.begin(); it != Instance().p2_score.end(); it++)
		it->Draw();
	for (it = Instance().high_score_text.begin(); it != Instance().high_score_text.end(); it++)
		it->Draw();
	for (int i= 0; i < 8; i++)
	{
		for (it= Instance().hs[i].begin(); it != Instance().hs[i].end(); it++)
			it->Draw();
	}
	for (it = Instance().credits_2.begin(); it != Instance().credits_2.end(); it++)
		it->Draw();
	for (it = Instance().bonus_every_12000.begin(); it != Instance().bonus_every_12000.end(); it++)
		it->Draw();
	
	//draw both p1 and p2's lives
	Instance().blaster_sprite.setPosition(p1_lives_position,-4);
	for (int i= 0; i < p1_lives; i++)
	{
		WindowManager::MainWindow.draw( Instance().blaster_sprite );
		lives_position_offset+= 16;
		Instance().blaster_sprite.setPosition(p1_lives_position+lives_position_offset,-4);
	}

	lives_position_offset= 0;
	Instance().blaster_sprite.setPosition(p2_lives_position,-4);
	for (int i= 0; i < p2_lives; i++)
	{
		WindowManager::MainWindow.draw( Instance().blaster_sprite );
		lives_position_offset-= 16;
		Instance().blaster_sprite.setPosition(p2_lives_position+lives_position_offset,-4);
	}

}

void HUDManager::SwapPlayerHUDs()
{
	//if p1 is the current player, swap all relevant data to p2
	if(Instance().current_score == &Instance().p1_score)
	{
		Instance().current_score= &Instance().p2_score;
		Instance().current_nextpos= &Instance().p2_nextpos;
		Instance().current_scorepos= &Instance().p2_scorepos;
	}
	//if p2 is the current player, swap all relevant data to p1
	else if (Instance().current_score == &Instance().p2_score )
	{
		Instance().current_score= &Instance().p1_score;
		Instance().current_nextpos= &Instance().p1_nextpos;
		Instance().current_scorepos= &Instance().p1_scorepos;
	}
}

void HUDManager::TitleScreenOn()
{
	highscore temp;
	//update the temporary high score list from the master list (see ScoreManager for more info)
	ScoreManager::CopyHighScoreList();
	//read in the 8 high scores and store them here
	for(int i= 0; i < 8; i++)
	{
		temp.user_name= ScoreManager::SendUserName();
		temp.score= ScoreManager::SendScore();
		Instance().high_score_queue.push(temp);
		ScoreManager::PopHighScoreList();
	}
	//simple "HIGH SCORE" text - store and set position
	std::string score_converter;
	std::string highscores= "HIGH SCORES";
	Instance().high_score_nextpos.x= high_score_position_x;
	Instance().high_score_nextpos.y= high_score_position_y;
	//read string into glyph list
	for (int i=0; i < highscores.length(); i++)
	{
		Instance().high_score_text.push_back(  Instance().myfont.GetGlyph(highscores.at(i), Instance().high_score_nextpos) );
		GridManager::SpawnGridObjectAtPosition(Instance().high_score_nextpos.x, Instance().high_score_nextpos.y);
		Instance().high_score_nextpos.x += Instance().myfont.CellWidth();
	}
	
	std::string score_values;
	Instance().high_score_nextpos.x= hs1_position_x;
	Instance().high_score_nextpos.y= hs1_position_y;
	//do this for each of the 8 high scores
	for (int i= 0; i < 8; i++)
	{
		//read in the initials and high score and store it in a string
		temp= Instance().high_score_queue.front();
		score_converter= Tools::ToString(temp.score);
		score_values= score_converter + " " + temp.user_name;
		
		//store this string in a list of glyphs (hs is an array of glyph lists)
		for (int j = 0; j < score_values.length(); j++)
		{
			Instance().hs[i].push_back(  Instance().myfont.GetGlyph(score_values.at(j), Instance().high_score_nextpos) );
			GridManager::SpawnGridObjectAtPosition(Instance().high_score_nextpos.x, Instance().high_score_nextpos.y);
			Instance().high_score_nextpos.x += Instance().myfont.CellWidth();
		}

		//update drawing positions and pop the list to get to the next high score
		Instance().high_score_nextpos.x= hs1_position_x;
		Instance().high_score_nextpos.y += Instance().myfont.CellHeight();
		Instance().high_score_queue.pop();
	}
	//simple "CREDITS 2" text - store and set position
	std::string credits2= "CREDITS 2";
	Instance().high_score_nextpos.x= credits_2_position_x;
	Instance().high_score_nextpos.y= credits_2_position_y;
	//read string into glyph list
	for (int i=0; i < credits2.length(); i++)
	{
		Instance().credits_2.push_back(  Instance().myfont.GetGlyph(credits2.at(i), Instance().high_score_nextpos) );
		GridManager::SpawnGridObjectAtPosition(Instance().high_score_nextpos.x, Instance().high_score_nextpos.y);
		Instance().high_score_nextpos.x += Instance().myfont.CellWidth();
	}
	//simple "BONUS EVERY 12000" text - store and set position
	std::string bonusevery12000= "BONUS EVERY 12000";
	Instance().high_score_nextpos.x= bonus_every_12000_position_x;
	Instance().high_score_nextpos.y= bonus_every_12000_position_y;
	//read string into glyph list
	for (int i=0; i < bonusevery12000.length(); i++)
	{
		Instance().bonus_every_12000.push_back(  Instance().myfont.GetGlyph(bonusevery12000.at(i), Instance().high_score_nextpos) );
		GridManager::SpawnGridObjectAtPosition(Instance().high_score_nextpos.x, Instance().high_score_nextpos.y);
		Instance().high_score_nextpos.x += Instance().myfont.CellWidth();
	}
}

void HUDManager::TitleScreenOff()
{
	//clear out all of the glyph lists for the title screen
	std::list<Glyph>::iterator it;
	while(Instance().high_score_text.empty() == false)
		Instance().high_score_text.pop_front();
		
	for (int i= 0; i < 8; i++)
	{
		while(Instance().hs[i].empty() == false)
			Instance().hs[i].pop_front();
	}
	while (Instance().credits_2.empty() == false)
		Instance().credits_2.pop_front();
	while (Instance().bonus_every_12000.empty() == false)
		Instance().bonus_every_12000.pop_front();
}

void HUDManager::Delete()
{
	//Instance().MarkForDestroy();
}