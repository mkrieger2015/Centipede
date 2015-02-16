#include "TEAL\CommonElements.h"
#include "MainScene.h"
#include "..\Blaster.h"
#include "..\Mushroom.h"
#include "..\PoisonMushroom.h"
#include "..\Bullet.h"
#include "..\Head.h"
#include "..\Body.h"
#include "..\Flea.h"
#include "..\Spider.h"
#include "..\Scorpion.h"
#include "..\GameManager.h"

void MainScene::Initialize()
{
	//main scene. does nothing except test for collisions and load the GameManager

	CollisionTestPair<Mushroom,Blaster>();
	CollisionTestPair<Bullet,Mushroom>();
	CollisionTestPair<Bullet,PoisonMushroom>();
	CollisionTestPair<Blaster,Head>();
	CollisionTestPair<Blaster,Body>();
	CollisionTestPair<Blaster,Spider>();
	CollisionTestPair<Blaster,Flea>();
	CollisionTestPair<Bullet,Head>();
	CollisionTestPair<Bullet,Body>();
	CollisionTestPair<Bullet,Flea>();
	CollisionTestPair<Bullet,Scorpion>();
	CollisionTestPair<Scorpion,Mushroom>();
	CollisionTestPair<Spider,Mushroom>();
	CollisionTestPair<Spider,Bullet>();

	GameManager::Begin();
}