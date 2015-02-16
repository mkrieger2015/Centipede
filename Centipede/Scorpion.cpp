#include "Scorpion.h"
#include "Bullet.h"
#include "Mushroom.h"
#include "WaveManager.h"
#include "GridManager.h"
#include "SoundManager.h"
#include "ScoreManager.h"
#include "CommonFunctionsAndValues.h"
#include "ExplosionEffectFactory.h"

extern const float scorpion_SPEED;
extern const int window_width;
extern const int window_height;
extern const float y_upperbound;
extern const int scorpion_points;

Scorpion::Scorpion()
{
	//set sprite, animation, and collider
	bitmap = ResourceManager::GetTextureBitmap("Scorpion");
	MainSprite = AnimatedSprite(  ResourceManager::GetTexture("Scorpion"), 4, 1, 15);
	MainSprite.SetAnimation(0,3,true,true);
	MainSprite.setOrigin(MainSprite.getTextureRect().width / 2.0f, MainSprite.getTextureRect().height / 2.0f);
	MainSprite.setScale(1,1);
	MainSprite.setPosition(Pos);
	SetCollider(MainSprite, bitmap, true);
}

void Scorpion::Update()
{
	//set new x position (direction determines left or right)
	Pos.x += scorpion_SPEED*direction;
	MainSprite.Update();
	MainSprite.setPosition(Pos);
	//if the scorpion has walked off screen, destroy it 
	//direction is important, since we do not want the spider to be destroyed the minute he is spawned
	//thus, if the scorpion is moving left, his direction int is -1, and he will only be destroyed if he walks off the left edge of the screen
	//same process is applied for moving right
	if(direction == -1 && Pos.x < 0 - (MainSprite.getTextureRect().width / 2.0f))
	{
		MarkForDestroy();
	}
	else if(direction == 1 && Pos.x > window_width + (MainSprite.getTextureRect().width / 2.0f))
	{
		MarkForDestroy();
	}
		
}

void Scorpion::Draw()
{
	WindowManager::MainWindow.draw(MainSprite);
}

void Scorpion::Destroy()
{
	SoundManager::ScorpionStop();
	//re-scale sprites back to proper position (for recycling)
	if(direction == -1)
	{
		MainSprite.scale(1,1);
	}
	else if (direction == 1)
	{
		MainSprite.scale(-1,1);
	}
	DeregisterCollision<Scorpion>(*this);
	//deregister this scorpion from the wave manager
	WaveManager::ScorpionDestroyed();
	//reset the scorpion spawn timer in the wave manager
	WaveManager::ResetScorpionTimer();
}

void Scorpion::Initialize()
{
	float y_position;
	float x_position;
	//set the y position to a random row above the player area
	y_position= fmod(rand(),y_upperbound);
	//snap the y position to an exact row (this must be exact so that mushroom creation is not misaligned)
	y_position= GridManager::ConvertFloatToGridUnits(y_position);
	y_position= GridManager::ConvertGridUnitsToFloat(y_position);
	//offset the y position by 1/2 the sprite height, so that the scorpion is on the row properly
	y_position+= MainSprite.getTextureRect().height / 2.0f;
	//set direction, random pick
	//sprite is scaled so that the scorpin faces either left or right
	direction= rand() % 2;
	if(direction == 0)
	{
		MainSprite.scale(1,1);
		direction= -1;
		x_position= window_width + (MainSprite.getTextureRect().width / 2.0f);
	}
	else if (direction == 1)
	{
		MainSprite.scale(-1,1);
		x_position= 0 - (MainSprite.getTextureRect().width / 2.0f);
	}
	Pos.x= x_position;
	Pos.y= y_position;
	//play the scorpion sound
	SoundManager::ScorpionPlay();
	//play the sprite animation
	MainSprite.Play();
	RegisterCollision<Scorpion>(*this);
	//register this scorpion with the wave manager, so that it can be frozen when the wave is paused/player dies
	WaveManager::ScorpionCreated(this);
}

void Scorpion::Collision(Bullet *other)
{
	//add appropriate points to player's score
	ScoreManager::AddPoints(scorpion_points);
	other->MarkForDestroy();
	SoundManager::CentipedeKillPlay();
	//create an explosion effect at position (re-adjusted for sprite centering)
	ExplosionEffectFactory::CreateExplosionEffect((Pos.x - (MainSprite.getTextureRect().width / 2.0f)), (Pos.y - (MainSprite.getTextureRect().height / 2.0f)));
	MarkForDestroy();
}

void Scorpion::Collision(Mushroom *other)
{
	//if the grid position is filled (double-check), convert it to a poison mushroom
	if(GridManager::CheckGridPosition(Pos.x,Pos.y) == true)
		GridManager::ConvertPoisonMushroom(Pos.x,Pos.y);
}

void Scorpion::WaveStop()
{
	//stop movement
	direction= 0;
	//stop sprite
	MainSprite.Pause();
	//stop sound
	SoundManager::ScorpionStop();
}