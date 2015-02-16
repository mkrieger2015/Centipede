#include "GameManager.h"
#include "Blaster.h"
#include "Mushroom.h"
#include "PoisonMushroom.h"
#include "MushroomFactory.h"
#include "PoisonMushroomFactory.h"
#include "Bullet.h"
#include "Head.h"
#include "Body.h"
#include "Flea.h"
#include "Spider.h"
#include "Scorpion.h"
#include "SpawningGrid.h"
#include "SoundManager.h"
#include "BulletFactory.h"
#include "Unmuted.h"
#include "HeadFactory.h"
#include "BodyFactory.h"
#include "FleaFactory.h"
#include "ScorpionFactory.h"
#include "SpiderFactory.h"
#include "BlasterFactory.h"
#include "KeyboardInputFactory.h"
#include "AIInputFactory.h"
#include "ExplosionEffectFactory.h"
#include "ScoreManager.h"
#include "GridManager.h"
#include "WaveManager.h"
#include "PlayerManager.h"
#include "HUDManager.h"
#include "ModeSelect.h"
#include "Game Components/TEAL/AlarmManager.h"

GameManager* GameManager::self= NULL;

void GameManager::Begin()
{
	//this function sets everything up for a game to begin properly

	//the initial game type is zero, since the player has not selected 1p or 2p mode yet
	Instance().game_type= 0;

	//instantiate (e.g. load defaults) for all singletons and multitons
	HUDManager::Instantiate();
	PlayerManager::Instantiate();
	GridManager::Instantiate();
	SoundManager::Instantiate();
	WaveManager::Instantiate();
	WaveManager::LoadWaveData();
	ScoreManager::Instantiate();

	//load the high score data
	ScoreManager::LoadHighScores();

	//begin the first wave
	WaveManager::BeginWave();

	//set the window background color
	WindowManager::SetBackgroundColor( sf::Color(0,0,0,255) );

	//populate the mushroom field
	GridManager::SpawnMushrooms();

	//display high score table and title screen text
	HUDManager::TitleScreenOn();

	//create the object which lets the player choose 1p or 2p mode
	new ModeSelect;
}

void GameManager::ResetGame()
{
	//this flushes all caches, queues, etc. and essentially creates a blank slate for the game to run again
	PlayerManager::DeleteAllPlayers();
	GridManager::DeleteAllGrids();
	SoundManager::DeleteAllSoundObjects();
	WaveManager::ResetWave();
	WaveManager::ResetAllWaveTimers();
	HUDManager::Delete();
	HeadFactory::Empty();
	BodyFactory::Empty();
	SpiderFactory::Empty();
	FleaFactory::Empty();
	ScorpionFactory::Empty();
	BulletFactory::Empty();
	BlasterFactory::Empty();
	MushroomFactory::Empty();
	PoisonMushroomFactory::Empty();
	KeyboardInputFactory::Empty();
	AIInputFactory::Empty();
	ExplosionEffectFactory::Empty();

	//run Begin to start the new game
	Instance().Begin();
}

void GameManager::OnePlayerGame()
{
	//sets the game type(many objects and managers refer to this)
	Instance().game_type= 1;
	//disables title screen
	HUDManager::TitleScreenOff();
	//loads the player 1 grid (see SwapGrids for more info)
	GridManager::SwapGrids();
	//sets the currently selected player to player 1 (see SwapPlayers for more info)
	PlayerManager::SwapPlayers();
	//sets the points to be edited to player 1's points (see SwapPlayerPoints for more info)
	ScoreManager::SwapPlayerPoints();
	//unmutes the sound
	SoundManager::SwapObjects();
	//resets the current wave to clear all enemies from the screen
	WaveManager::ResetWave();
	//sets the currently selected set of waves to player 1's set (see SwapWaves for more info)
	WaveManager::SwapWaves();
	//begins the wave
	WaveManager::BeginWave();
	
}

void GameManager::TwoPlayerGame()
{
	//sets the game type(many objects and managers refer to this)
	Instance().game_type= 2;
	//disables title screen
	HUDManager::TitleScreenOff();
	//loads the player 1 grid (see SwapGrids for more info)
	GridManager::SwapGrids();
	//sets the currently selected player to player 1 (see SwapPlayers for more info)
	PlayerManager::SwapPlayers();
	//sets the points to be edited to player 1's points (see SwapPlayerPoints for more info)
	ScoreManager::SwapPlayerPoints();
	//unmutes the sound
	SoundManager::SwapObjects();
	//resets the current wave to clear all enemies from the screen
	WaveManager::ResetWave();
	//sets the currently selected set of waves to player 1's set (see SwapWaves for more info)
	WaveManager::SwapWaves();
	//begins the wave
	WaveManager::BeginWave();
	
}

int GameManager::ReturnGameType()
{
	return Instance().game_type;
}

void GameManager::SwapPlayers()
{
	//swaps everything - see respective swap functions for more info
	HUDManager::SwapPlayerHUDs();
	//this lets the wave manager know the player is alive again, since the player just died in order for this swap to take place
	WaveManager::PlayerStatus(true);
	GridManager::SwapGrids();
	PlayerManager::SwapPlayers();
	ScoreManager::SwapPlayerPoints();
	WaveManager::ResetWave();
	WaveManager::SwapWaves();
	WaveManager::BeginWave();
}
