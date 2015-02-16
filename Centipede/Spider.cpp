#include "CommonFunctionsAndValues.h"
#include "Spider.h"
#include "Blaster.h"
#include "Bullet.h"
#include "Mushroom.h"
#include "GridManager.h"
#include "SoundManager.h"
#include "ScoreManager.h"
#include "WaveManager.h"
#include "PlayerManager.h"
#include "ExplosionEffectFactory.h"

extern const float spider_SPEED;
extern const float spider_leftpos;
extern const float spider_rightpos;
extern const float spider_spawnheight;
extern const float spider_upperbound;
extern const float spider_lowerbound;
extern const int window_width;
extern const int spider_minlength;
extern const int spider_maxlength;
extern const float spider_300_distance;
extern const float spider_900_distance;

//GET RID OF THE MAGIC NUMBERS HERE!

Spider::Spider()
{
	//set sprite, animation, and collider
	bitmap = ResourceManager::GetTextureBitmap("Spider");
	MainSprite.setOrigin(MainSprite.getTextureRect().width / 2.0f, MainSprite.getTextureRect().height / 2.0f);
	MainSprite = AnimatedSprite(  ResourceManager::GetTexture("Spider"), 8, 2, 15);
	MainSprite.setScale(1,1);
	MainSprite.setPosition(Pos);
	SetCollider(MainSprite, bitmap, true);
}

void Spider::EmptyQueue()
{
	//empty the queue of orders
	while(orders_queue.empty() == false)
	{
		orders_queue.pop();
	}
}

void Spider::MoveUp()
{
	//fill the orders queue (of randomized varying length) with orders to move up
	int queue_size;
	queue_size= (rand() % spider_maxlength) + spider_minlength;
	for (int i= 0; i < queue_size; i++)
	{
		temp_orders.x_position= 0;
		temp_orders.y_position= -speed;
		orders_queue.push(temp_orders);
	}
}

void Spider::MoveDown()
{
	//fill the orders queue (of randomized varying length) with orders to move down
	int queue_size;
	queue_size= (rand() % spider_maxlength) + spider_minlength;
	for (int i= 0; i < queue_size; i++)
	{
		temp_orders.x_position= 0;
		temp_orders.y_position= speed;
		orders_queue.push(temp_orders);
	}
}

void Spider::MoveDiagonalUp()
{
	//fill the orders queue (of randomized varying length) with orders to move diagonally up
	int queue_size;
	queue_size= (rand() % spider_maxlength) + spider_minlength;
	for (int i= 0; i < queue_size; i++)
	{
		temp_orders.x_position= speed*direction_multiplier;
		temp_orders.y_position= -speed;
		orders_queue.push(temp_orders);
	}
}

void Spider::MoveDiagonalDown()
{
	//fill the orders queue (of randomized varying length) with orders to move diagonally down
	int queue_size;
	queue_size= (rand() % spider_maxlength) + spider_minlength;
	for (int i= 0; i < queue_size; i++)
	{
		temp_orders.x_position= speed*direction_multiplier;
		temp_orders.y_position= speed;
		orders_queue.push(temp_orders);
	}
}

void Spider::Update()
{
	//if the orders queue is empty...
	if(orders_queue.empty() == true)
	{
		//roll a mod 4 to decide which kind of movement to fill the orders queue with
		int move_chance;
		move_chance= rand() % 4;
		if(move_chance == 0)
			MoveUp();
		else if(move_chance == 1)
			MoveDown();
		else if(move_chance == 2)
			MoveDiagonalUp();
		else if (move_chance == 3)
			MoveDiagonalDown();		
	}

	//if the orders queue still has orders to execute...
	if(orders_queue.empty() == false)
	{
		temp_orders = orders_queue.front();
		//if the spider is above its upper boundary...
		if(Pos.y + temp_orders.y_position < spider_upperbound)
		{
			//empty the queue and either move it down or diagonally down
			EmptyQueue();
			int move_down_chance;
			move_down_chance= rand () % 2;
			if(move_down_chance == 0)
				MoveDown();
			else if(move_down_chance == 1)
				MoveDiagonalDown();
			temp_orders= orders_queue.front();
		}
		//else, if the spider is below its lower boundary...
		else if(Pos.y + temp_orders.y_position > spider_lowerbound)
		{
			//empty the queue and either move it down or diagonally up
			EmptyQueue();
			int move_up_chance;
			move_up_chance= rand() % 2;
			if(move_up_chance == 0)
				MoveUp();
			else if(move_up_chance == 1)
				MoveDiagonalUp();
		}
		//set the new x position and y position based on the newest order, then pop it off the queue
			Pos.x+= temp_orders.x_position;
			Pos.y+= temp_orders.y_position;
			orders_queue.pop();
	}

	MainSprite.Update();
	MainSprite.setPosition(Pos);

	//if the spider has walked off screen, destroy it 
	//direction is important, since we do not want the spider to be destroyed the minute he is spawned
	//thus, if the spider is moving left, his direction bool is false, and he will only be destroyed if he walks off the left edge of the screen
	//same process is applied for moving right
	if(Pos.x <= (0 - (MainSprite.getTextureRect().width / 2.0f)) && direction == false)
	{
		DeregisterCollision<Spider>(*this);
		MarkForDestroy();
	}
	if(Pos.x >= (window_width + (MainSprite.getTextureRect().width / 2.0f)) && direction == true)
	{
		DeregisterCollision<Spider>(*this);
		MarkForDestroy();
	}
}

void Spider::Draw()
{
	WindowManager::MainWindow.draw(MainSprite);
}

void Spider::Destroy()
{
	DeregisterCollision<Spider>(*this);
	//tell the sound manager to stop the spider noise
	SoundManager::SpiderStop();
	//empty the orders queue
	EmptyQueue();
	//deregister the spider from the wave manager
	WaveManager::SpiderDestroyed();
	//reset the spider spawn timer in the wave manager
	WaveManager::ResetSpiderTimer();
}

void Spider::Initialize()
{
	//set the speed to the spider speed of the currently loaded wave
	speed= WaveManager::ReturnSpiderSpeed();
	//tell the sound manager to play the spider noise
	SoundManager::SpiderPlay();
	//reset and play animation
	MainSprite.SetAnimation(0,7,true,true);
	MainSprite.Play();
	int initial_direction;
	//direction is chosen at initialization
	initial_direction = rand() % 2;
	//if the direction is left, spawn him on the right side
	if(initial_direction == 0)
	{
		direction= false;
		direction_multiplier= -1;
		Pos.x= spider_rightpos;
		Pos.y= spider_spawnheight;
	}
	//if the direction is right, spawn him on the left side
	else if (initial_direction == 1)
	{
		direction= true;
		direction_multiplier= 1;
		Pos.x= spider_leftpos;
		Pos.y= spider_spawnheight;
	}
	RegisterCollision<Spider>(*this);
	//register this spider with the wave manager, so that it can stop it if the wave is frozen/player dies
	WaveManager::SpiderCreated(this);
	//start by moving diagonally down
	MoveDiagonalDown();
}

void Spider::AwardPoints()
{
	DeregisterCollision<Spider>(*this);
	//stop spider sound
	SoundManager::SpiderStop();
	//play death noise
	SoundManager::CentipedeKillPlay();
	//empty orders queue
	EmptyQueue();
	//stop speed
	speed= 0;
	//test distance from player to spider to determine points awarded
	if((PlayerManager::ReturnPlayerY() - Pos.y) <= spider_300_distance && (PlayerManager::ReturnPlayerY() - Pos.y) > spider_900_distance)
	{
		MainSprite.SetAnimation(9,9,false,false);
		SetAlarm(1,1);
		//Award and display 600 points
		ScoreManager::AddPoints(600);
	}
	else if((PlayerManager::ReturnPlayerY() - Pos.y) <= spider_900_distance)
	{
		MainSprite.SetAnimation(10,10,false,false);
		SetAlarm(1,1);
		//Award and display 900 points
		ScoreManager::AddPoints(900);
	}
	else if((PlayerManager::ReturnPlayerY() - Pos.y) > spider_300_distance)
	{
		MainSprite.SetAnimation(8,8,false,false);
		SetAlarm(1,1);
		//Award and display 300 points
		ScoreManager::AddPoints(300);
	}
}

void Spider::Collision(Bullet *other)
{
	other->MarkForDestroy();
	//create an explosion effect at position (re-adjusted for sprite centering)
	ExplosionEffectFactory::CreateExplosionEffect(Pos.x,Pos.y);
	AwardPoints();
}

void Spider::Collision(Mushroom *other)
{
	other->MarkForDestroy();
}

void Spider::Alarm1()
{
	//this alarm is called from ScorePoints
	MarkForDestroy();
}

void Spider::WaveStop()
{
	//set speed to zero
	speed= 0;
	//pause animation
	MainSprite.Pause();
	//ensure queue is empty
	while(orders_queue.empty() == false)
	{
		orders_queue.pop();
	}
	//stop spider sound
	SoundManager::SpiderStop();
}