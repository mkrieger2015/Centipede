#include "ExplosionEffect.h"

void ExplosionEffect::Initialize(float xpos, float ypos)
{
	//think of this object like a particle
	//set sprite and animation
	life_iterator= 0;
	Pos.x= xpos;
	Pos.y= ypos;
	MainSprite= AnimatedSprite( ResourceManager::GetTexture("Explosion"), 6, 1);
	MainSprite.SetAnimation(0,7,true,false);

	MainSprite.setOrigin(0,0);
	MainSprite.setScale(1,1);

	MainSprite.setPosition(Pos);
	SetDrawOrder(1000);
}

void ExplosionEffect::Draw()
{
	WindowManager::MainWindow.draw(MainSprite);
}

void ExplosionEffect::Update()
{
	//after six frames have played, destroy this instance
	MainSprite.Update();
	life_iterator++;
	if(life_iterator >= 6)
		MarkForDestroy();
}
