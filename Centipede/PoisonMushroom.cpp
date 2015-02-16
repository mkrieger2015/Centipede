#include "PoisonMushroom.h"
#include "CommonFunctionsAndValues.h"
#include "Bullet.h"
#include "GridManager.h"
#include "SoundManager.h"
#include "ScoreManager.h"
#include "ExplosionEffectFactory.h"


extern const int mushroom_health;

PoisonMushroom::PoisonMushroom()
{
	//read in health
	health= mushroom_health;

	//is poisoned
	is_poison= true;
	
	//load sprite, animation, and collider
	bitmap = ResourceManager::GetTextureBitmap("Mushroom");
	MainSprite = AnimatedSprite(  ResourceManager::GetTexture("Mushroom"), 4, 2);
	MainSprite.SetAnimation(4,7,false,false); 

	MainSprite.setOrigin(0,0);
	MainSprite.setScale(1,1);
	
	SetCollider(MainSprite, bitmap, true);
	
	SetDrawOrder(10);
}

void PoisonMushroom::Initialize(float xpos, float ypos)
{
	//reset animation
	MainSprite.SetAnimation(4,7,false,false); 
	Pos = sf::Vector2f(xpos,ypos);
	MainSprite.setPosition(Pos);
	RegisterCollision<PoisonMushroom>(*this);
	//if this initializer is used, the mushroom was not converted
	converted= false;
}

void PoisonMushroom::InitializeDamaged(float xpos, float ypos, int prev_health)
{
	//this initializer is for if the converted mushroom has damage
	Pos = sf::Vector2f(xpos,ypos);
	//cycle through frames for amount of health you have left
	for (int i = 0; i < (mushroom_health - prev_health); i++)
	{
		health--;
		MainSprite.NextFrame();
	}
	MainSprite.setPosition(Pos);
	RegisterCollision<PoisonMushroom>(*this);
}

void PoisonMushroom::Draw()
{
	WindowManager::MainWindow.draw(MainSprite);
}

void PoisonMushroom::Destroy()
{
	//only clear the gird position if this mushroom is not being destroyed as part of the repair process
	if(converted == false)
	{
		GridManager::ClearGridPosition(Pos.x,Pos.y);
	}
	health= 4;
	DeregisterCollision<PoisonMushroom>(*this);
}

void PoisonMushroom::Collision(Bullet *other)
{
	other->MarkForDestroy();
	if(health > 1)
	{
		//cycle through to next frame to show damage
		health--;
		MainSprite.NextFrame();
	}
	else if (health <= 1)
	{
		MarkForDestroy();
		//only award points if this mushroom is not being destroyed as part of the repair process
		if(converted == false)
		{
			ScoreManager::AddPoints(1);
		}
	}

}

void PoisonMushroom::Repair()
{
	SoundManager::MushroomRepair();
	//the mushroom has now been converted back to normal - therefore, the player should not gain points for it being destroyed
	converted= true;
	GridManager::ClearGridPosition(Pos.x,Pos.y);
	GridManager::SpawnMushroomAtPosition(Pos.x,Pos.y);
	//create an explosion effect at position
	ExplosionEffectFactory::CreateExplosionEffect(Pos.x, Pos.y);
	MarkForDestroy();
	
}