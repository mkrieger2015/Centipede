#include "Flea.h"
#include "Bullet.h"
#include "GridManager.h"
#include "SoundManager.h"
#include "WaveManager.h"
#include "ScoreManager.h"
#include "CommonFunctionsAndValues.h"
#include "ExplosionEffectFactory.h"

extern const float flea_SPEED;
extern const float mushroom_chance;
extern const float y_initial;
extern const int window_width;
extern const int window_height;
extern const int flea_points;


Flea::Flea()
{
	//set sprite, collider, and animation
	bitmap = ResourceManager::GetTextureBitmap("Flea");
	MainSprite = AnimatedSprite(  ResourceManager::GetTexture("Flea"), 2, 2, 15);
	MainSprite.SetAnimation(0,1,true,true); 
	MainSprite.setOrigin(0,0);
	MainSprite.setScale(1,1);
	MainSprite.setPosition(Pos);
	SetCollider(MainSprite, bitmap, true);
}

void Flea::Update()
{
	//update y position (speed_multiplier is for after the flea is shot once)
	Pos.y+= flea_SPEED*speed_multiplier;
	//roll a chance to spawn a mushroom on your way down
	float mushroom_roll;
	mushroom_roll= (rand() % 100);
	if (mushroom_roll <= mushroom_chance)
		GridManager::SpawnMushroomAtPosition(Pos.x,Pos.y);

	MainSprite.Update();
	MainSprite.setPosition(Pos);
	
	//destroy if off the screen
	if(Pos.y > window_height)
		MarkForDestroy();
}

void Flea::Draw()
{
	WindowManager::MainWindow.draw(MainSprite);
}

void Flea::Destroy()
{
	DeregisterCollision<Flea>(*this);
	//deregister with the wave manager
	WaveManager::FleaDestroyed();
	//tell the wave manager to check and see if it should spawn another flea
	WaveManager::SpawnFlea();
	
}

void Flea::Initialize()
{
	float x_initial;
	//set life and speed multiplier
	life= 2;
	speed_multiplier= 1;
	//play flea sound
	SoundManager::FleaDropPlay();
	//randomize x position to a random columns
	x_initial=  (rand() % (window_width-1));
	//snap this position to an exact column position (so that mushroom spawning is not misaligned)
	x_initial= GridManager::ConvertFloatToGridUnits(x_initial);
	x_initial= GridManager::ConvertGridUnitsToFloat(x_initial);
	//set position
	Pos.x= x_initial;
	Pos.y= y_initial;
	MainSprite.setPosition(Pos);
	MainSprite.Play();
	RegisterCollision<Flea>(*this);
	//register this flea with the wave manager, so it can stop when the wave pauses/player dies
	WaveManager::FleaCreated(this);
}

void Flea::Collision(Bullet *other)
{
	other->MarkForDestroy();
	life--;
	//if the flea took its first hit, double the speed
	if(life == 1)
		speed_multiplier= 2;
	else if(life == 0)
	{
		ScoreManager::AddPoints(flea_points);
		//create an explosion effect at position (re-adjusted for sprite centering)
		ExplosionEffectFactory::CreateExplosionEffect(Pos.x, Pos.y);
		MarkForDestroy();
	}
}

void Flea::WaveStop()
{
	//stop movement
	speed_multiplier= 0;
	//stop sprite animation
	MainSprite.Pause();
	//stop flea sound
	SoundManager::FleaDropStop();
}