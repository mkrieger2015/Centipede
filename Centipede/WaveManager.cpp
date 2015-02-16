#include "CommonFunctionsAndValues.h"
#include "HeadFactory.h"
#include "WaveManager.h"
#include "PlayerManager.h"
#include "Flea.h"
#include "Spider.h"
#include "Scorpion.h"
#include "Bullet.h"

WaveManager* WaveManager::self= NULL;

extern const int mushroom_limit;
extern const int spider_timer_min;
extern const int spider_timer_max;
extern const int scorpion_timer_min;
extern const int scorpion_timer_max;
extern const int flea_scorpion_wave;
extern const float y_upperbound;


void WaveManager::Instantiate()
{
	//set pointers for all instances to null
	Instance();
	Instance().is_flea_spawned= false;
	Instance().spider_pointer= NULL;
	Instance().flea_pointer= NULL;
	Instance().bullet_pointer= NULL;
	Instance().scorpion_pointer= NULL;
	//player is both alive and active 
	//these ARE used for different things
	Instance().active_player= true;
	Instance().player_alive= true;
	//load the ai wave queue, since the ai is active first
	Instance().loaded_wave= &Instance().ai_wave;
	//there is no head primed to be released
	Instance().head_primed= false;
}

void WaveManager::Alarm0()
{
	//creates a spider when alarm is called
	SpiderFactory::CreateSpider();
}

void WaveManager::Alarm1()
{
	//if the current wave permits fleas and scorpions, one is created 
	if(Instance().loaded_wave->wave > flea_scorpion_wave)
		ScorpionFactory::CreateScorpion();
}

void WaveManager::Alarm2()
{
	Instance().BeginWave();
}

void WaveManager::Alarm3()
{
	//this alarm system staggers the spawning of extra centipede heads at the top of the screen
	//this is so that they don't all spawn on top of one another
	if(Instance().temp_extra_heads > 0)
	{
		Instance().temp_extra_heads--;
		Instance().SpawnCentipede(8,8,true,0);
		Instance().SetAlarm(3,.25);
	}
}

void WaveManager::SwapPlayer()
{
	//swaps whether or not the player is active or inactive
	if(Instance().active_player == true)
		Instance().active_player == false;
	else if(Instance().active_player == false)
		Instance().active_player == true;
}

bool WaveManager::ReturnActivePlayer()
{
	//returns if the player is active or not
	if(Instance().active_player == true)
		return true;
	else if(Instance().active_player == false)
		return false;
}

void WaveManager::ResetSpiderTimer()
{	
	//sets the spider timer to a random value within a minimum and maximum
	int timer_value;
	timer_value= (rand() % (Instance().loaded_wave->spidertimemax - Instance().loaded_wave->spidertimemin)) + Instance().loaded_wave->spidertimemin;
	Instance().SetAlarm(0,timer_value);
}

void WaveManager::ResetScorpionTimer()
{
	//sets the scorpion timer to a random value within a minimum and maximum
	int timer_value;
	timer_value= (rand() % (Instance().loaded_wave->scorpiontimemax - Instance().loaded_wave->scorpiontimemin)) + Instance().loaded_wave->scorpiontimemin;
	Instance().SetAlarm(1,timer_value);

}

void WaveManager::SpawnCentipede(float xpos,float ypos,bool initial_direction,int segments)
{
	//accessor for head factory. Redundant? Look later
	HeadFactory::CreateHead(xpos,ypos,initial_direction,segments);
}

void WaveManager::SpawnFlea()
{
	//if the current wave permits fleas, and there is no flea spawned...
	if(Instance().is_flea_spawned == false && Instance().loaded_wave->wave > flea_scorpion_wave)
	{
		//if there are less mushrooms in the player area than the mushroom minimum, create a flea
		if(GridManager::ReturnMushInPlayerArea() < mushroom_limit)
			FleaFactory::CreateFlea();
	}
}

void WaveManager::FleaCreated(Flea* in_flea_pointer)
{
	//when the flea is created, register it to the flea pointer
	//in addition, set the is_flea_spawned bool to true, in order to prevent more than one flea on screen at a time
	Instance().flea_pointer= in_flea_pointer;
	Instance().is_flea_spawned= true;
}

void WaveManager::FleaDestroyed()
{
	//deregister flea when it is destroyed
	Instance().flea_pointer= NULL;
	Instance().is_flea_spawned= false;
}

void WaveManager::ScorpionCreated(Scorpion* in_scorpion_pointer)
{
	//register scorpion when it is created
	Instance().scorpion_pointer= in_scorpion_pointer;
}

void WaveManager::ScorpionDestroyed()
{
	//deregister scorpion when it is destroyed
	Instance().scorpion_pointer= NULL;
}

void WaveManager::SpiderCreated (Spider* in_spider_pointer)
{
	//register spider when it is created
	Instance().spider_pointer= in_spider_pointer;
}

void WaveManager::SpiderDestroyed()
{
	//deregister spider when it is destroyed
	Instance().spider_pointer= NULL;
}

void WaveManager::BulletCreated(Bullet *in_bullet_pointer)
{
	//register bullet when it is created
	Instance().bullet_pointer= in_bullet_pointer;
}

void WaveManager::BulletDestroyed()
{
	//deregister bullet when it is destroyed
	Instance().bullet_pointer= NULL;
}

void WaveManager::CentipedeCreated(Centipede *in_centipede_pointer)
{
	//when a centipede segment is created (head or body), push it onto the list
	Instance().centipede_list.push_front(in_centipede_pointer);
}

void WaveManager::CentipedeDestroyed(Centipede *in_centipede_pointer)
{
	//when a centipede segment is destroyed, search it in the list and remove it
	Instance().centipede_list.remove(in_centipede_pointer);
	//if this was the last centipede piece in the list, stop the centipede sound loop
	if(Instance().centipede_list.empty() == true)
		SoundManager::CentipedeBeatStop();
}

void WaveManager::CentipedeShot()
{
	//note - this event is different than the Destroy event because a centipede can be destroyed in multiple ways 
	//these events should only apply if the centipede is destroyed by a bullet

	//if the centipede list is empty...
	if(Instance().centipede_list.empty() == true)
	{
		//start the next wave, but only if the player is alive
		if(Instance().player_alive == true)
			Instance().NextWave();
		Instance().SetAlarm(2,.5);
	}
	//if, after that, the centipede on screen is the last remaining segment, double his speed
	if(Instance().centipede_list.size() == 1 && (Instance().loaded_wave->wave % 2) == 0 )
	{
		Instance().centipede_list.front()->DoubleSpeed();
	}
}

void WaveManager::WaveStop()
{
	//HERE, you can see why there is a registration system for every enemey in the game within this object
	//this function freezes every enemy on screen via callbacks once the player dies

	std::list<Centipede*>::iterator centipede_iterator;
	Centipede* temp;
	//iterate through centipede list and freeze each piece
	if (Instance().centipede_list.empty() == false)
	{
		for(centipede_iterator= Instance().centipede_list.begin(); centipede_iterator != Instance().centipede_list.end(); centipede_iterator++)
		{
			centipede_iterator._Ptr->_Myval->WaveStop();
		}
	}
	//creeze spider, flea, scorpion, and the bullet
	if(Instance().spider_pointer != NULL)
		Instance().spider_pointer->WaveStop();
	if(Instance().flea_pointer != NULL)
		Instance().flea_pointer->WaveStop();
	if(Instance().scorpion_pointer != NULL)
		Instance().scorpion_pointer->WaveStop();
	if(Instance().bullet_pointer != NULL)
		Instance().bullet_pointer->MarkForDestroy();
	//set scorpion and centipede respawning alarms to absurdly long times, so they won't go off
	Instance().SetAlarm(0,999);
	Instance().SetAlarm(1,999);
}

void WaveManager::ResetWave()
{
	//like the above function, except instead of using the callback to freeze the enemies, it uses the callback to destroy them
	Centipede* temp;
	while(Instance().centipede_list.empty() == false)
	{
		temp= Instance().centipede_list.front();
		temp->MarkForDestroy();
	}
	if(Instance().spider_pointer != NULL)
		Instance().spider_pointer->MarkForDestroy();
	if(Instance().flea_pointer != NULL)
		Instance().flea_pointer->MarkForDestroy();
	if(Instance().scorpion_pointer != NULL)
		Instance().scorpion_pointer->MarkForDestroy();

	//resets the timers for scorpion and spider, and restarts the countdown
	Instance().ResetScorpionTimer();
	Instance().ResetSpiderTimer();
}

void WaveManager::BeginWave()
{	
	//due to the technicalities of the spawning system, sometimes a flea will spawn when a new wave starts, even if he is not supposed to
	//this failsave prevents that, though for some reason it will still play the noise...
	if(Instance().flea_pointer != NULL)
	{
		Instance().flea_pointer->MarkForDestroy();
		SoundManager::FleaDropStop();
	}
	//resets the timers for scorpion and spider, and restarts the countdown
	Instance().ResetSpiderTimer();
	Instance().ResetScorpionTimer();
	//spawns the main centipede, with length based on wave data
	Instance().SpawnCentipede(248,-8,false,Instance().loaded_wave->centilength);
	//loads any extra heads the wave asks for
	//(note: this is staggered with an alarm system so that the heads don't all spawn on top of each other)
	Instance().temp_extra_heads= Instance().loaded_wave->extraheads;
	if(Instance().temp_extra_heads > 0)
	{
		Instance().temp_extra_heads--;
		Instance().SpawnCentipede(8,8,true,0);
		Instance().SetAlarm(3,.25);
	}
}

void WaveManager::PlayerStatus(bool dead_alive)
{
	//updates whether the player is alive or dead
	if(dead_alive == true)
		Instance().player_alive= true;
	else if(dead_alive == false)
		Instance().player_alive= false;
}

void WaveManager::LoadWaveData()
{
	//opens text file and reads into master wave queue
	std::ifstream myfile;
	std::string dummy;
	wave_data temp;
	myfile.open("resources\\wave_data.txt");
	for (int i= 0; i < 8; i++)
	{
		myfile >> dummy;
		myfile >> temp.wave;
		myfile >> dummy;
		myfile >> temp.centispeed;
		myfile >> dummy;
		myfile >> temp.headspeed;
		myfile >> dummy;
		myfile >> temp.centilength;
		myfile >> dummy;
		myfile >> temp.extraheads;
		myfile >> dummy;
		myfile >> temp.spiderspeed;
		myfile >> dummy;
		myfile >> temp.spidertimemin;
		myfile >> dummy;
		myfile >> temp.spidertimemax;
		myfile >> dummy; 
		myfile >> temp.scorpiontimemin;
		myfile >> dummy;
		myfile >> temp.scorpiontimemax;

		Instance().master_wave_queue.push(temp);
	}
	myfile.close();

	//copy p1, p2, and ai queues from the master
	Instance().p1_wave_queue= Instance().master_wave_queue;
	Instance().p2_wave_queue= Instance().master_wave_queue;
	Instance().ai_wave_queue= Instance().master_wave_queue;

	//set currently selected queue to ai queue
	*Instance().loaded_wave= Instance().ai_wave_queue.front();
	Instance().p1_wave= Instance().p1_wave_queue.front();
	Instance().p2_wave= Instance().p2_wave_queue.front();
}

void WaveManager::NextWave()
{
	//if p1 wave is loaded, pop the p1 queue
	if(Instance().loaded_wave == &Instance().p1_wave)
	{
		Instance().p1_wave_queue.pop();
		*Instance().loaded_wave= Instance().p1_wave_queue.front();
	}
	//if p2 wave is loaded, pop the p2 queue
	else if(Instance().loaded_wave == &Instance().p2_wave)
	{
		Instance().p2_wave_queue.pop();
		*Instance().loaded_wave= Instance().p2_wave_queue.front();
	}
	//if ai wave is loaded, pop the ai queue
	else if(Instance().loaded_wave == &Instance().ai_wave)
	{
		Instance().ai_wave_queue.pop();
		*Instance().loaded_wave= Instance().ai_wave_queue.front();
	}
}

float WaveManager::ReturnHeadSpeed()
{
	float return_float= Instance().loaded_wave->headspeed;
	return return_float;
}

float WaveManager::ReturnSpiderSpeed()
{
	float return_float= Instance().loaded_wave->spiderspeed;
	return return_float;
}

float WaveManager::ReturnCentipedeSpeed()
{
	float return_float= Instance().loaded_wave->centispeed;
	return return_float;
}

void WaveManager::SwapWaves()
{
	//if p1 selected, swap to p2
	if(Instance().loaded_wave == &Instance().p1_wave)
	{
		Instance().loaded_wave= &Instance().p2_wave;
	}
	//if p2 or ai selected, swap to p1
	else if (Instance().loaded_wave == &Instance().p2_wave || Instance().loaded_wave == &Instance().ai_wave)
	{
		Instance().loaded_wave= &Instance().p1_wave;
	}
}

void WaveManager::ResetAllWaveTimers()
{
	//sets all timers to huge values so that they will not go off until the game is reset completely
	Instance().SetAlarm(0,999999);
	Instance().SetAlarm(1,999999);
	Instance().SetAlarm(2,999999);
	Instance().SetAlarm(3,999999);
}

void WaveManager::PrimeExtraHead()
{
	//primes a head for release
	Instance().head_primed= true;
}

void WaveManager::ReleaseExtraHead()
{
	//if head is primed for release
	if(Instance().head_primed == true)
	{
		//clear the primer bool
		Instance().head_primed= false;
		//pick the left or right side and spawn a loose head
		int side_picker= (rand() % 2);
		if(side_picker == 0)
			HeadFactory::CreateHead(0,y_upperbound,true,0);
		else if(side_picker == 1)
			HeadFactory::CreateHead(480,y_upperbound,false,0);
	}
}
