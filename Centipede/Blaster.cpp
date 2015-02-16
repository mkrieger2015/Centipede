#include "Blaster.h"
#include "KeyboardInput.h"
#include "AIInput.h"
#include "Bullet.h"
#include "BulletFactory.h"
#include "Head.h"
#include "Body.h"
#include "Spider.h"
#include "Flea.h"
#include "CommonFunctionsAndValues.h"
#include "SoundManager.h"
#include "WaveManager.h"
#include "PlayerManager.h"
#include "KeyboardInputFactory.h"
#include "AIInputFactory.h"

extern const float blaster_SPEED;
extern const float y_upperbound;
extern const float y_lowerbound;
extern const float start_x;
extern const float start_y;
extern const float sprite_bumper;

Blaster::Blaster()
{

}

void Blaster::Initialize(bool mode)
{
	//movement is on
	movement_lock= 1;

	//load sprites and set animation
	bitmap = ResourceManager::GetTextureBitmap("Blaster");
	MainSprite = AnimatedSprite(  ResourceManager::GetTexture("Blaster"), 2, 1);
	MainSprite.SetAnimation(0,1,false,false);
	MainSprite.SetFrame(0);

	MainSprite.setOrigin( MainSprite.getTextureRect().width / 2.0f, (MainSprite.getTextureRect().height / 2.0f)-2);
	MainSprite.setScale(1,1);

	//load explosion sprite (death) and set animation)
	ExplosionSprite = AnimatedSprite( ResourceManager::GetTexture("Death"), 4, 2, 30);
	ExplosionSprite.SetAnimation(0,7,true,false);
	ExplosionSprite.Pause();

	ExplosionSprite.setOrigin( MainSprite.getTextureRect().width / 2.0f, (MainSprite.getTextureRect().height / 2.0f));
	ExplosionSprite.setScale(1,1);

	DisplaySprite= &MainSprite;
	
	Pos = sf::Vector2f(start_x,start_y);
	MainSprite.setPosition(Pos);

	SetCollider(MainSprite, bitmap, true);
	RegisterCollision<Blaster>(*this);
	
	SetDrawOrder(10000);

	//blaster can currently fire
	loaded= true;

	//determines whether blaster should either be ai controlled or player controlled
	if (mode == true)
	{
		control_mode= true;
		input= KeyboardInputFactory::CreateKeyboardInput(this);
	}
	else if (mode == false)
	{
		control_mode= false;
		input= AIInputFactory::CreateAIInput(this);
	}

	//there has not been a bullet fired yet
	active_bullet= NULL;
}

void Blaster::Destroy()
{
	//destroy input object
	if(input != NULL)
		input->MarkForDestroy();
	//destroy any live fire on screen
	if(active_bullet != NULL)
		active_bullet->MarkForDestroy();
	DeregisterCollision<Blaster>(*this);
}

void Blaster::Update()
{
	DisplaySprite->Update();
}

void Blaster::Up(int mult)
{
	//mult determines whether or not this event will actually move the player - see the AIInput for more information
	//store previous y
	yprev= Pos.y;
	//set new y position, if mult and movement_lock are both 1
	Pos.y -= blaster_SPEED*mult*movement_lock;
	//clamp to keep player in bounds
	Tools::Clamp<float>(Pos.y, y_upperbound, y_lowerbound);	
	MainSprite.setPosition(Pos);
}

void Blaster::Down(int mult)
{
	//mult determines whether or not this event will actually move the player - see the AIInput for more information
	//store previous y
	yprev= Pos.y;
	//set new y position, if mult and movement_lock are both 1
	Pos.y += blaster_SPEED*mult*movement_lock;
	//clamp to keep player in bounds
	Tools::Clamp<float>(Pos.y, y_upperbound, y_lowerbound);	
	MainSprite.setPosition(Pos);
}

void Blaster::Left(int mult)
{
	//mult determines whether or not this event will actually move the player - see the AIInput for more information
	//store previous x
	xprev= Pos.x;
	//set new x position, if mult and movement_lock are both 1
	Pos.x -= blaster_SPEED*mult*movement_lock;
	//clamp to keep player in bounds
	Tools::Clamp<float>(Pos.x, sprite_bumper, (WindowManager::MainWindow.getView().getSize().x)-sprite_bumper);
	MainSprite.setPosition(Pos);
}

void Blaster::Right(int mult)
{
	//mult determines whether or not this event will actually move the player - see the AIInput for more information
	//store previous x
	xprev= Pos.x;
	//set new x position, if mult and movement_lock are both 1
	Pos.x += blaster_SPEED*mult*movement_lock;
	//clamp to keep player in bounds
	Tools::Clamp<float>(Pos.x, sprite_bumper, (WindowManager::MainWindow.getView().getSize().x)-sprite_bumper);
	MainSprite.setPosition(Pos);
}

void Blaster::Fire()
{
	if(loaded == true)
	{
		//change the sprite from loaded to unloaded
		MainSprite.NextFrame();
		loaded= false;
		//fire the bullet, and store it in the active_bullet pointer
		active_bullet= BulletFactory::CreateBullet(Pos.x, Pos.y);
	}
}

void Blaster::Reload()
{
	//change the sprite from unloaded to loaded
	MainSprite.NextFrame();
	//clear active bullet
	active_bullet= NULL;
	loaded= true;
}

void Blaster::Draw()
{
	WindowManager::MainWindow.draw(*DisplaySprite);
}

void Blaster::Collision(Mushroom *other)
{
	//if blaster collides with a mushroom, simply move it to its previous position
	Pos.x= xprev;
	Pos.y= yprev;
}

void Blaster::Collision(Head *other)
{
	Death();
}

void Blaster::Collision(Body *other)
{
	Death();
}

void Blaster::Collision(Flea *other)
{
	Death();
}

void Blaster::Collision(Spider *other)
{
	Death();
}

void Blaster::Death()
{
	//if the player is not an AI, decrease the life count of the current player 
	if(control_mode == true)
		PlayerManager::DecreaseLives();

	//tell the wave manager the player is dead, and to stop all enemies on screen
	WaveManager::PlayerStatus(false);
	WaveManager::WaveStop();
	DeregisterCollision<Blaster>(*this);
	
	//movement is off
	movement_lock= 0;

	//play the explosion effect and sound
	ExplosionSprite.SetAnimation(0,7,true,false);
	ExplosionSprite.setPosition(Pos);
	DisplaySprite= &ExplosionSprite;
	SoundManager::BlasterKill();
	loaded= false;
	SetAlarm(1,1);
}

void Blaster::Respawn()
{
	RegisterCollision<Blaster>(*this);
	//tell the wave manager the player is alive again
	WaveManager::PlayerStatus(true);
	//movement is on
	movement_lock= 1;
	loaded= true;
	DisplaySprite= &MainSprite;
	//set to start position, and begin the wave once again
	Pos = sf::Vector2f(start_x,start_y);
	MainSprite.setPosition(Pos);
	WaveManager::BeginWave();
}

void Blaster::Alarm1()
{
	//begin repairing mushroom field
	GridManager::RepairMushroomField();
}

float Blaster::ReturnX()
{
	return Pos.x;
}

float Blaster::ReturnY()
{
	return Pos.y;
}