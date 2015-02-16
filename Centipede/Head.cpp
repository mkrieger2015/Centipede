#include "Head.h"
#include "SpawningGrid.h"
#include "SoundManager.h"
#include "BodyFactory.h"
#include "ScoreManager.h"
#include "GridManager.h"
#include "Blaster.h"
#include "Bullet.h"
#include "WaveManager.h"
#include "ExplosionEffectFactory.h"

extern const float centipede_SPEED;
extern const float centipede_leftbound;
extern const float centipede_rightbound;
extern const float pixelvalue;
extern const float y_lowerbound;
extern const float y_upperbound;
extern const int head_points;

Head::Head(float xpos, float ypos, bool initial_direction, int segments)
{
	//initialize death marker
	was_shot= false;

	//rotation is on
	rotation_switch= 1;

	//movement is on
	movement_switch= 1;

	//not poisoned initially
	poisoned= false;

	//head should never have a previous
	previous= NULL;
	
	//read speed from extern
	if(segments > 0)
		speed= WaveManager::ReturnCentipedeSpeed();
	else if (segments == 0)
		speed= WaveManager::ReturnHeadSpeed();

	//set sprite and collision box
	MainSprite = AnimatedSprite(  ResourceManager::GetTexture("c_head_left_walk"), 8, 1);
	MainSprite.SetAnimation(0,7,true,true); 
	MainSprite.setOrigin(MainSprite.getTextureRect().width / 2.0f, MainSprite.getTextureRect().height / 2.0f);
	MainSprite.setScale(1,1);
	bitmap = ResourceManager::GetTextureBitmap("c_head_left_walk");
	SetCollider(MainSprite, bitmap, true);

	//set detection range for lookahead function
	detection_range= (MainSprite.getTextureRect().width);
	
	//set rotation increments for smooth turn based on current speed
	rotation_increment= (3.141592/(pixelvalue/speed))*(180/3.141592);
	rotation_cumulative= 0;

	//head is initially descending
	ascending_descending= false;

	//set x and y positions
	Pos.x= xpos;
	Pos.y= ypos;

	//if there are segments to be spawned, make one segment, decrement segment number by one and send the order to keep creating segments down the line
	int num_segments= segments;
	if(num_segments > 0)
	{
		num_segments--;
		next= BodyFactory::CreateBody(Pos.x,Pos.y,num_segments,this);
	}
	else
		next= NULL;

	//establish initial direction and set x multiplier appropriately
	direction= initial_direction;
	if(direction == true)
		x_direction_multiplier= 1;
	else if (direction == false)
		x_direction_multiplier= -1;

	//set sprite rotation based on direction
	if(direction== true)
	{
		MainSprite.setRotation(180);
		rotation_cumulative= 180;
	}
	else if (direction == false)
	{
		MainSprite.setRotation(0);
		rotation_cumulative= 0;
	}

	//play centipede sound
	SoundManager::CentipedeBeatPlay();

	//
	if(next != NULL)
		next->Initialize(Pos.x,Pos.y);
	
	WaveManager::CentipedeCreated(this);

	//set sprite position and register collisions
	MainSprite.setPosition(Pos);
	RegisterCollision<Head>(*this);

}

//this initializer is for when a centipede head is recycled and needs to have new position and pointers established
//it may appear largely redundant but recycled heads would otherwise retain their old pointers and position
void Head::InitializeNew(float xpos, float ypos, bool initial_direction, int segments)
{
	//reset death marker
	was_shot= false;

	//ensure to clear all queues
	while(out_queue.empty() == false)
	{
		out_queue.pop();
	}

	while(order_queue.empty() == false)
	{
		order_queue.pop();
	}
	while(storage_queue.empty() == false)
	{
		storage_queue.pop();
	}

	//rotation is on
	rotation_switch= 1;

	//movement is on
	movement_switch= 1;

	//not poisoned initially
	poisoned= false;

	//head should never have a previous
	previous= NULL;
	
	//read speed from extern
	if(segments > 0)
		speed= WaveManager::ReturnCentipedeSpeed();
	else if (segments == 0)
		speed= WaveManager::ReturnHeadSpeed();
	
	//set rotation increments for smooth turn based on current speed
	rotation_increment= (3.141592/(pixelvalue/speed))*(180/3.141592);
	rotation_cumulative= 0;

	//head is initially descending
	ascending_descending= false;

	//set x and y positions
	Pos.x= xpos;
	Pos.y= ypos;

	//if there are segments to be spawned, make one segment, decrement segment number by one and send the order to keep creating segments down the line
	int num_segments= segments;
	if(num_segments > 0)
	{
		num_segments--;
		next= BodyFactory::CreateBody(Pos.x,Pos.y,num_segments,this);
	}
	else
		next= NULL;

	//establish initial direction and set x multiplier appropriately
	direction= initial_direction;
	if(direction == true)
		x_direction_multiplier= 1;
	else if (direction == false)
		x_direction_multiplier= -1;

	//set sprite rotation based on direction
	if(direction== true)
	{
		MainSprite.setRotation(180);
		rotation_cumulative= 180;
	}
	else if (direction == false)
	{
		MainSprite.setRotation(0);
		rotation_cumulative= 0;
	}

	//play centipede sound
	SoundManager::CentipedeBeatPlay();

	//
	if(next != NULL)
		next->Initialize(Pos.x,Pos.y);
	
	//set sprite position and register collisions
	MainSprite.setPosition(Pos);
	MainSprite.Play();
	RegisterCollision<Head>(*this);
	WaveManager::CentipedeCreated(this);
}

void Head::InitializeTransform(float xpos, float ypos, std::queue<orders> in_queue, std::queue<orders> old_out, Centipede *new_next, bool old_direction)
{
	//reset death marker
	was_shot= false;

	//ensure to clear all queues
	while(out_queue.empty() == false)
	{
		out_queue.pop();
	}

	while(order_queue.empty() == false)
	{
		order_queue.pop();
	}
	while(storage_queue.empty() == false)
	{
		storage_queue.pop();
	}

	speed= WaveManager::ReturnCentipedeSpeed();

	//rotation is on
	rotation_switch= 1;

	//movement is on
	movement_switch= 1;

	//set this object's orders to the order queue of the now-former body (this theoretically should pick up movement right where the body left off)
	order_queue= in_queue;
	storage_queue= old_out;

	//set direction
	direction= old_direction;

	//set the new "next" segment
	next= new_next;

	//if there is a new next segment, set this head as the "previous" of the new next segment
	if(next != NULL)
		next->previous= this;

	//set x and y position (may be redundant if head is new, but this is needed for recycled heads)
	Pos.x= xpos;
	Pos.y= ypos;

	//reset sprite position and re-register collisions, in case the head is recycled
	MainSprite.setPosition(Pos);
	MainSprite.Play();
	RegisterCollision<Head>(*this);
	WaveManager::CentipedeCreated(this);
}

void Head::NormalMove()
{
	//builds an order queue for left or right movement, with size based on speed

	//store xprevious and yprevious for building the new x and y coordinates in the order queue
	xprevious= Pos.x;
	yprevious= Pos.y;

	//creates a temporary place to store orders while the queue is being built
	orders temp_orders;

	//the centipede is not changing rows; therefore, we don't want it to switch directions after this queue is empty
	row_changing= false;

	for(int i= 0; i < (pixelvalue/speed); i++)
	{
		temp_orders.x_offset= xprevious+((speed*(x_direction_multiplier))*movement_switch);
		xprevious= xprevious+((speed*(x_direction_multiplier))*movement_switch);
		temp_orders.y_offset= yprevious;
		yprevious= yprevious+0;
		temp_orders.x_direction_multiplier= x_direction_multiplier;
		temp_orders.direction= direction;
		temp_orders.rotation_degrees= rotation_cumulative;
		order_queue.push(temp_orders);
	}
	//store this order queue in storage_queue as a deep copy
	storage_queue= order_queue;
}

void Head::ChangeRow(bool up_down)
{
	//builds an order queue for up or down movement, with size based on speed
	//up or down is determined by parameter up_down 
	int y_multiplier;

	//store xprevious and yprevious for building the new x and y coordinates in the order queue
	xprevious= Pos.x;
	yprevious= Pos.y;

	//creates a temporary place to store orders while the queue is being built
	orders temp_orders;

	//the centipede is currently changing rows; it needs to know this so that the direction adjustment is applied when the change is finished
	row_changing= true;

	if(up_down == true)
		y_multiplier= -1;
	else if(up_down == false)
		y_multiplier= 1;

	for(int i= 0; i < (pixelvalue/speed); i++)
	{
		temp_orders.x_offset= xprevious;
		xprevious= xprevious;
		temp_orders.y_offset= yprevious+(speed*y_multiplier*movement_switch);
		yprevious= yprevious+(speed*y_multiplier*movement_switch);
		temp_orders.x_direction_multiplier= x_direction_multiplier;
		temp_orders.direction= direction;
		rotation_cumulative += rotation_increment*x_direction_multiplier*rotation_switch;
		temp_orders.rotation_degrees= rotation_cumulative;
		order_queue.push(temp_orders);
	}
	//store this order queue in storage_queue as a deep copy
	storage_queue= order_queue;
}

void Head::Update()
{
	//if the order queue is empty...
	if(order_queue.empty() == true)
	{
		//set output queue (sent off to the "next" segment) to the storage queue (deep copy of the now-empty order queue)
		out_queue= storage_queue;

		if(row_changing == true)
		{
			if(direction== true)
			{
				direction= false;
				x_direction_multiplier= -1;
			}
			else if(direction == false)
			{
				direction= true;
				x_direction_multiplier= 1;
			}
			row_changing= false;
		}

		//if poisoned, check if you're in the player area, and if you're not, descend a row. If you are in the player area, unpoison yourself.
		if (poisoned == true)
		{
			ChangeRow(false);
			if (Pos.y >= y_upperbound)
				poisoned= false;		
		}

		//if you're not poisoned...
		else if (poisoned == false)
		{
			//if the head is at the left edge of the screen, the right edge of the screen, or detects a mushroom in front of it...
			if((Pos.x+(detection_range*x_direction_multiplier)) <= centipede_leftbound || (Pos.x+(detection_range*x_direction_multiplier)) >= centipede_rightbound || GridManager::CheckGridPosition(Pos.x+(detection_range*x_direction_multiplier),Pos.y) == true)
			{
				//if that mushroom in front of you is a poison mushroom...
				if(GridManager::CheckPoison(Pos.x+(detection_range*x_direction_multiplier),Pos.y) == true)
				{
					//set poisoned to true
					poisoned= true;
				}

				//if you're at the bottom edge of the screen and you are descending, set the mode to ascending.
				if(Pos.y >= y_lowerbound && ascending_descending == false)
				{
					//release an extra head from the top of the player area
					WaveManager::ReleaseExtraHead();
					ascending_descending= true;
				}
				//if you're ascending and you hit the upper boundary of the player area, set the mode to descending.
				else if (Pos.y <= y_upperbound && ascending_descending == true)
				{
					//prepare an extra head to be released 
					WaveManager::PrimeExtraHead();
					ascending_descending= false;
				}

				//read if you're ascending or descending and either ascend or descend a row, respectively
				if(ascending_descending == false)
					ChangeRow(false);
				else if(ascending_descending == true)
					ChangeRow(true);
			}

			//if you are off the upper edge of the screen (this is for when the centipede is spawned at the beginning of each wave, descend until you're on screen
			else if(Pos.y <= 8)
			{
				ChangeRow(false);
			}
			//if you meet none of the requirements above, just move straight
			else
			{
				NormalMove();
			}
		}
	}

	//if the order queue is NOT empty, called ReadQueue to move appropriately
	if(order_queue.empty() == false)
		ReadQueue();

	//update sprite and set position
	MainSprite.Update();
	MainSprite.setPosition(Pos);
}

void Head::Destroy()
{
	//degregister collisions, sever "next" pointer
	DeregisterCollision<Head>(*this);
	WaveManager::CentipedeDestroyed(this);
	if(was_shot == true)
	{
		//tells the wave manager that it has been shot (this is for controlling a separate chain of events, compared to if the centipde were destroyed by clearing the screen)
		WaveManager::CentipedeShot();
	}

	next= NULL;
	
}

void Head::Collision(Blaster *other)
{
	//empty for now
}

void Head::Collision(Bullet *other)
{
	//destroy the bullet and deregister collision (this is reduntant, but I did it because I was suspecting there were double bullet collisions happening)
	other->MarkForDestroy();
	DeregisterCollision<Head>(*this);

	//add points for destroying head
	ScoreManager::AddPoints(head_points);

	//play the kill sound
	SoundManager::CentipedeKillPlay();

	//spawn a mushroom at death position
	GridManager::SpawnMushroomAtPosition(Pos.x,Pos.y);

	//if there's a next segment, convert it to a head
	if(next != NULL)
	{
		next->ConvertToHead();
	}

	//create an explosion effect at position (re-adjusted for sprite centering)
	ExplosionEffectFactory::CreateExplosionEffect((Pos.x - (MainSprite.getTextureRect().width / 2.0f)), (Pos.y - (MainSprite.getTextureRect().height / 2.0f)));

	//mark that this was killed by a bullet, rather than a screen clear event
	was_shot= true;

	//destroy self
	MarkForDestroy();
}

void Head::WaveStop()
{
	//pause main sprite
	MainSprite.Pause();
	//rotation is off
	rotation_switch= 0;
	//speed should be zero
	movement_switch = 0;
	//clear all queues
	while(order_queue.empty() == false)
	{
		order_queue.pop();
	}
	while(storage_queue.empty() == false)
	{
		storage_queue.pop();
	}
	while(out_queue.empty() == false)
	{
		out_queue.pop();
	}
	if(next != NULL)
		next->previous= NULL;
	//stop centipede beat
	SoundManager::CentipedeBeatStop();
}

void Head::DoubleSpeed()
{
	speed= WaveManager::ReturnHeadSpeed();
}