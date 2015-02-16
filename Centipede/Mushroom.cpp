#include "Mushroom.h"
#include "CommonFunctionsAndValues.h"
#include "Bullet.h"
#include "GridManager.h"
#include "WaveManager.h"
#include "SoundManager.h"
#include "ScoreManager.h"
#include "ExplosionEffectFactory.h"


extern const int mushroom_health;
extern const float y_upperbound;
extern const float pixelvalue;

Mushroom::Mushroom()
{
	//read in health
	health= mushroom_health;
	
	//is not poisoned on creation
	is_poison= false;

	//set sprite, animation, and collider
	bitmap = ResourceManager::GetTextureBitmap("Mushroom");
	MainSprite = AnimatedSprite(  ResourceManager::GetTexture("Mushroom"), 4, 2);
	MainSprite.SetAnimation(0,3,false,false); 

	MainSprite.setOrigin(0,0);
	MainSprite.setScale(1,1);
	
	SetCollider(MainSprite, bitmap, true);
	
	SetDrawOrder(10);
}

void Mushroom::Initialize(float xpos, float ypos)
{
	//re-read in health
	health= mushroom_health;
	//reset animation
	MainSprite.SetAnimation(0,3,false,false); 
	MainSprite.SetFrame(0);
	Pos = sf::Vector2f(xpos,ypos);
	MainSprite.setPosition(Pos);
	RegisterCollision<Mushroom>(*this);
	//if this mushroom is in the player area, tell the gridmanager 
	//this is used for when to spawn fleas
	if(ypos >= (y_upperbound-pixelvalue))
		GridManager::IncrementMushInPlayerArea();
}

void Mushroom::Draw()
{
	WindowManager::MainWindow.draw(MainSprite);
}

void Mushroom::Destroy()
{
	//clear the grid position in the grid manager first
	GridManager::ClearGridPosition(Pos.x,Pos.y);
	//if this mushroom is in the player area, tell the gridmanager 
	//this is used for when to spawn fleas
	if(Pos.y >= (y_upperbound-pixelvalue))
		GridManager::DecrementMushInPlayerArea();
	//reset health, to be used the next time it's initialized
	health= 4;
	DeregisterCollision<Mushroom>(*this);
	//tell the wavemanager to check and see if it should spawn a flea
	WaveManager::SpawnFlea();
}

int Mushroom::ReturnHealth()
{
	return health;
}

void Mushroom::Collision(Bullet *other)
{
	other->MarkForDestroy();
	if(health > 1)
	{
		//decrease health and cycle to next frame of animation to show damage
		health--;
		MainSprite.NextFrame();
	}
	else if (health <= 1)
	{
		//clear the grid position on the grid manager first 
		GridManager::ClearGridPosition(Pos.x,Pos.y);
		ScoreManager::AddPoints(1);
		MarkForDestroy();
	}

}

void Mushroom::Repair()
{
	//restore health and animation frame to full health, for recycling
	MainSprite.SetFrame(0);
	health= 4;
	//play mushroom repair sound
	SoundManager::MushroomRepair();
	//create an explosion effect at position
	ExplosionEffectFactory::CreateExplosionEffect(Pos.x, Pos.y);
}