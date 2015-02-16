#include "Body.h"
#include "BodyFactory.h"
#include "HeadFactory.h"
#include "ScoreManager.h"
#include "SoundManager.h"
#include "GridManager.h"
#include "Blaster.h"
#include "Bullet.h"
#include "CommonFunctionsAndValues.h"
#include "WaveManager.h"
#include "ExplosionEffectFactory.h"

extern const float centipede_SPEED;
extern const int body_points;

Body::Body()
{
}

Body::Body(float x, float y, int segments, Centipede *prev)
{
	//initialize death marker
	was_shot= false;

	//rotation is on
	rotation_switch= 1;

	//movement is on
	movement_switch= 1;

	//set x and y position
	Pos.x= x;
	Pos.y= y;

	//recieve number of remaining segments to make
	int num_segments= segments;

	//if there are segments left to be made, decrement that number and create a new segment assigned to the "next" pointer
	if(num_segments > 0)
	{
		num_segments--;
		next= BodyFactory::CreateBody(x,y,num_segments,this);
	}
	else
		next= NULL;

	//set "previous" pointer
	previous= prev;

	//set sprite animation and collision
	MainSprite = AnimatedSprite(  ResourceManager::GetTexture("c_body_left_walk"), 8, 1);
	MainSprite.SetAnimation(0,7,true,true); 
	MainSprite.setOrigin(MainSprite.getTextureRect().width / 2.0f, MainSprite.getTextureRect().height / 2.0f);
	MainSprite.setScale(1,1);

	bitmap = ResourceManager::GetTextureBitmap("c_body_left_walk");

	WaveManager::CentipedeCreated(this);

	SetCollider(MainSprite, bitmap, true);
	RegisterCollision<Body>(*this);
}

void Body::Initialize(float xpos, float ypos, int segments, Centipede *prev)
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


	//set x and y position
	Pos.x= xpos;
	Pos.y= ypos;

	//if there are segments left to be made, decrement that number and create a new segment assigned to the "next" pointer
	if(segments > 0)
	{
		segments--;
		next= BodyFactory::CreateBody(xpos,ypos,segments,this);
	}
	else
		next= NULL;

	//set "previous" pointer
	previous= prev;
	SoundManager::CentipedeBeatPlay();

	RegisterCollision<Body>(*this);
	WaveManager::CentipedeCreated(this);
	
	MainSprite.SetAnimation(0,7,true,true); 
	MainSprite.Update();
	MainSprite.setPosition(Pos);

}

void Body::Update()
{
	//if order queue is empty, set your just-recently completed set of orders to your "out" queue (to be read in by the next segment), and recieve new orders from the "previous" pointer
	if(order_queue.empty() == true)
	{
		out_queue= storage_queue;
		if(previous != NULL)
			order_queue= previous->out_queue;
		storage_queue= order_queue;
	}

	//if the order queue is not empty, read it and move accordingly
	if(order_queue.empty() == false)
		ReadQueue();

	//update sprite and position
	MainSprite.Update();
	MainSprite.setPosition(Pos);
}

void Body::Destroy()
{
	//deregister collisions, sever next and previous pointers
	DeregisterCollision<Body>(*this);
	next= NULL;
	previous= NULL;
	WaveManager::CentipedeDestroyed(this);
	if(was_shot == true)
	{
		//tells the wave manager that it has been shot (this is for controlling a separate chain of events, compared to if the centipde were destroyed by clearing the screen)
		WaveManager::CentipedeShot();
	}

}

void Body::ConvertToHead()
{
	//set previous to null (heads should never have a previous)
	previous= NULL;
	//initialize a new head via the "transform initializer" so it recieves this body part's position, direction, orders, and "next" pointer
	HeadFactory::TransformHead(Pos.x,Pos.y,direction,order_queue,storage_queue,next);
	//destroy slef
	MarkForDestroy();
}

void Body::Collision(Bullet *other)
{
	//destroy bullet
	other->MarkForDestroy();
	//deregister collisions (seems redundant but I suspect there's a double collision issue so this will remain here for now)
	DeregisterCollision<Body>(*this);
	//add appropriate points
	ScoreManager::AddPoints(body_points);
	//play the kill sound
	SoundManager::CentipedeKillPlay();
	//create a mushroom at this position
	GridManager::SpawnMushroomAtPosition(Pos.x,Pos.y);
	//if there's a "next" segment, convert it to a head
	if(next != NULL)
	{
		next->ConvertToHead();
	}
	//if there's a "previous" segment, set its next to null
	if(previous != NULL)
		previous->next= NULL;
	//create an explosion effect at position (re-adjusted for sprite centering)
	ExplosionEffectFactory::CreateExplosionEffect((Pos.x - (MainSprite.getTextureRect().width / 2.0f)), (Pos.y - (MainSprite.getTextureRect().height / 2.0f)));
	//mark that this was killed by a bullet, rather than a screen clear event
	was_shot= true;
	//destroy self
	MarkForDestroy();
	
}

void Body::WaveStop()
{
	//pause main sprite
	MainSprite.Pause();
	//rotation is off
	rotation_switch= 0;
	//clear all queues
	while(order_queue.empty() == false)
	{
		order_queue.pop();
	}
	while(out_queue.empty() == false)
	{
		out_queue.pop();
	}
	while(storage_queue.empty() == false)
	{
		storage_queue.pop();
	}
	//stop centipede beat
	SoundManager::CentipedeBeatStop();
}