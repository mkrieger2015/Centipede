#include "ScoreManager.h"
#include "HUDManager.h"
#include "WaveManager.h"
#include "CommonFunctionsAndValues.h"
#include "PlayerManager.h"

ScoreManager* ScoreManager::self= NULL;

extern const int oneup_points;

void ScoreManager::Instantiate()
{
	//set all point values to 0
	Instance();
	Instance().p1_points= 0;
	Instance().p2_points= 0;
	Instance().ai_points= 0;
	//update the HUD
	HUDManager::UpdatePlayerScore(Instance().p1_points);
	HUDManager::SwapPlayerHUDs();
	HUDManager::UpdatePlayerScore(Instance().p2_points);
	HUDManager::SwapPlayerHUDs();
	//set current points to AI points (which are dummy, don't display)
	Instance().current_points= &Instance().ai_points;
	//set amount of points both p1 and p2 need to get a 1up
	Instance().p1_points_to_1up= oneup_points;
	Instance().p2_points_to_1up= oneup_points;
	Instance().current_points_to_1up= &Instance().p1_points_to_1up;
}

void ScoreManager::AddPoints(int point_value)
{
	//don't add points if the game's in ai mode
	if(Instance().current_points != &Instance().ai_points)
		*Instance().current_points+= point_value;
	//update the selected player's score, and the high score if the player score is bigger
	HUDManager::UpdatePlayerScore(*Instance().current_points);
	if(*Instance().current_points > Instance().high_score_points)
	{
		Instance().high_score_points= *Instance().current_points;
		HUDManager::UpdateHighScore(Instance().high_score_points);
	}
	//if the player hits the 1up point value, award them a 1up
	if(*Instance().current_points >= *Instance().current_points_to_1up)
	{
		PlayerManager::IncreaseLives();
		SoundManager::OneUp();
		*Instance().current_points_to_1up += oneup_points;
	}
}

void ScoreManager::SwapPlayerPoints()
{
	//if p1 is selected, swap to p2
	if(Instance().current_points == &Instance().p1_points)
	{
		Instance().current_points= &Instance().p2_points;
		Instance().current_points_to_1up= &Instance().p2_points_to_1up;
	}
	//if p2 is selected, or the ai is selected, swap to p1
	else if(Instance().current_points == &Instance().p2_points || Instance().current_points == &Instance().ai_points)
	{
		Instance().current_points= &Instance().p1_points;
		Instance().current_points_to_1up= &Instance().p1_points_to_1up;
	}
}

void ScoreManager::LoadHighScores()
{
	//pop high score master list, to make sure it's empty
	while(Instance().high_score_master.empty() == false)
	{
		Instance().high_score_master.pop_front();
	}
	//open file and read in, filling the master list
	std::ifstream myfile;
	myfile.open("resources\\high_scores.txt");
	high_score temp;
	for (int i= 0; i < 8; i++)
	{
		myfile >> temp.score;
		myfile >> temp.user_name;
		Instance().high_score_master.push_back(temp);
	}
	myfile.close();
	//to get the #1 high score drawn on the hud, read from the front of the master list
	//then, send that data off to the HUD manager
	temp= Instance().high_score_master.front();
	Instance().high_score_points= temp.score;
	HUDManager::UpdateHighScore(Instance().high_score_points);
}

std::string ScoreManager::SendUserName()
{
	high_score temp;
	temp= Instance().high_score_queue.front();
	return temp.user_name;
}

int ScoreManager::SendScore()
{
	high_score temp;
	temp= Instance().high_score_queue.front();
	return temp.score;
}

void ScoreManager::CopyHighScoreList()
{
	//set the temporary high score queue, used for updating the HUD, equal to the master queue
	Instance().high_score_queue = Instance().high_score_master;
}

void ScoreManager::PopHighScoreList()
{
	Instance().high_score_queue.pop_front();
}

void ScoreManager::Update()
{
	//cheat for you! hold p to increase your score
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::P)) 
		Instance().AddPoints(100);
}

int ScoreManager::ReturnMinHighScore()
{
	//returns the minimum high score (the back of the master list). used to test qualification for the high score table
	return Instance().high_score_master.back().score;
}

void ScoreManager::InsertHighScore(std::string input, int score)
{
	high_score temp;
	temp.score= score;
	temp.user_name= input;
	int queue_iterator= 8; 
	std::list<high_score> high_score_comparison_list;
	//sets a new "comparison list" equal to the master list (which is never modified)
	high_score_comparison_list = Instance().high_score_master;
	bool kill_loop= false;
	while (kill_loop == false)
	{
		if(high_score_comparison_list.empty() == false)
		{
			//if the score input is higher than the back of this list...
			if(score > high_score_comparison_list.back().score)
			{
				//pop it off, and decrement the queue operator (used later)
				queue_iterator--; 
				high_score_comparison_list.pop_back();
			}
			//if it's not, break out of this loop
			else if (score <= high_score_comparison_list.back().score)
				kill_loop= true;
		}
		//alternatively, if it goes through the whole queue, break out of the loop
		else if(high_score_comparison_list.empty() == true)
			kill_loop= true;
	}
	
	//this is where the queue iterator comes into play
	//this new list of high scores will be what we eventually read INTO the file
	//at first, we only push in values to this list up to JUST BEFORE the insertion position
	//for example, if the new entry is to be inserted at slot #5, this loop pushes the first 4 values
	std::list<high_score> new_high_scores;
	for(int i= 0; i < queue_iterator; i++)
	{
		new_high_scores.push_back(high_score_comparison_list.front());
		high_score_comparison_list.pop_front();
	}

	//then, we push the new entry into the back of the list. this is our hypothetical slot #5.
	new_high_scores.push_back(temp);

	//finally, we need to create the remainder of the list. 
	//we re-copy the high score comparison list (from the master), and then pop it so that we are at one position AHEAD of the new entry
	//so, in our hypothetical case, this will pick up from the new slot #6
	high_score_comparison_list = Instance().high_score_master;
	for(int i= 0; i < queue_iterator+1; i++)
	{
		high_score_comparison_list.pop_front();
	}
	while(high_score_comparison_list.empty() == false)
	{
		new_high_scores.push_back(high_score_comparison_list.front());
		high_score_comparison_list.pop_front();
	}
	
	//with all of these complex operations, we have our updated table, and are finally ready to write into the file!

	std::ofstream myfile;
	myfile.open("resources\\high_scores.txt");
	for(int i=0; i < 8; i++)
	{
		myfile << new_high_scores.front().score;
		myfile << " ";
		myfile << new_high_scores.front().user_name;
		myfile << std::endl;
		new_high_scores.pop_front();
	}
	myfile.close();
	//re-load the high scores, since we now have a newer version
	LoadHighScores();

}

