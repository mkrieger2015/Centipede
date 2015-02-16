#include "Bullet.h"
#include "CommonFunctionsAndValues.h"
#include "Blaster.h"
#include "Mushroom.h"
#include "BulletFactory.h"
#include "SoundManager.h"
#include "WaveManager.h"

extern const float bullet_SPEED;

Bullet::Bullet()
{
	//set sprite and collider
	bitmap = ResourceManager::GetTextureBitmap("Bullet");
	MainSprite = AnimatedSprite(  ResourceManager::GetTexture("Bullet"), 1, 1);
	MainSprite.SetAnimation(0,0,false,false); 

	MainSprite.setOrigin( MainSprite.getTextureRect().width / 2.0f, (MainSprite.getTextureRect().height / 2.0f));
	MainSprite.setScale(1,1);
	SetCollider(MainSprite, bitmap, true);
}

void Bullet::Initialize(Blaster *blast, float startPos_x, float startPos_y)
{
	//set up callback pointer to player
	player= blast;
	//set position 
	Pos.x= startPos_x;
	Pos.y= startPos_y;
	//play bullet sound
	SoundManager::BlasterPlay();
	//tell the wave manager a bullet has been created, so that it can register it and kill it if the player dies/wave stops
	WaveManager::BulletCreated(this);
	RegisterCollision<Bullet>(*this);
}

void Bullet::Update()
{
	//move every frame
	Pos.y -= bullet_SPEED;

	//if the bullet goes off screen, destroy it
	if (Pos.y < - MainSprite.getTextureRect().height ) 
	{
		MarkForDestroy(); 
	}

	MainSprite.setPosition(Pos);
}

void Bullet::Draw()
{
	WindowManager::MainWindow.draw(MainSprite);
}

void Bullet::Destroy()
{
	//callback to the player to reload
	player->Reload();
	DeregisterCollision<Bullet>(*this);
	//tell the wave manager to deregister this bullet
	WaveManager::BulletDestroyed();
}
