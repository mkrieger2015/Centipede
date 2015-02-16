// LoadAllResources.cpp
// Andre Berthiaume, June 2012
//
// The user fills the method with all resources that need loading

#include "ResourceManager.h"
#include "../MainScene.h"

void ResourceManager::LoadAllResources()
{
	SetStartScene( new MainScene() );
	AddTexture("Blaster", "blaster_sheet.png");
	AddTexture("Bullet", "bullet.png");
	AddTexture("Mushroom", "mushroom.png");
	AddTexture("c_head_left_walk","c_head_left_walk.png");
	AddTexture("c_body_left_walk","c_body_left_walk.png");
	AddTexture("Flea","flea.png");
	AddTexture("Scorpion","scorpion.png");
	AddTexture("Spider","spider_and_score.png");
	AddTexture("Death","death.png");
	AddTexture("Explosion","explosion.png");
	AddSound("fire1","fire1.wav");
	AddSound("newBeat","newBeat.wav");
	AddSound("kill","kill.wav");
	AddSound("spider","spider.wav");
	AddSound("flea","flea.wav");
	AddSound("scorpion","scorpion.wav");
	AddSound("death","death.wav");
	AddSound("bonus","bonus.wav");
	AddSound("oneup","1up.wav");

	//AddFont("PointFont", "MotorwerkOblique.ttf" );
	AddTexture("Alphabet", "FONT.png");
}

