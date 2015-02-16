#ifndef _WaveManager
#define _WaveManager

#include "Game Components\TEAL\CommonElements.h"
#include "FleaFactory.h"
#include "ScorpionFactory.h"
#include "SpiderFactory.h"
#include "GridManager.h"
#include "SoundManager.h"
#include "Blaster.h"
#include "Centipede.h"
#include <list>
#include <queue>
#include <fstream>
#include <iostream>
#include <string>

class HeadFactory;
class Spider;
class Scorpion;
class Flea;
class Bullet;

class WaveManager : public GameObject
{
private:
	static WaveManager *self;

	WaveManager(){ };
	WaveManager(const WaveManager&){ };
	WaveManager& operator= (const WaveManager&){ };

	~WaveManager(){ };
	
	static WaveManager& Instance()
	{
		if (!self)
			self= new WaveManager();
		return *self;
	};

	struct wave_data
	{
		int wave;
		float centispeed;
		float headspeed;
		int centilength;
		int extraheads;
		float spiderspeed;
		int spidertimemin;
		int spidertimemax;
		int scorpiontimemin;
		int scorpiontimemax;
	};

	std::queue<wave_data> master_wave_queue;
	std::queue<wave_data> p1_wave_queue;
	std::queue<wave_data> p2_wave_queue;
	std::queue<wave_data> ai_wave_queue;
	
	wave_data* loaded_wave;
	wave_data p1_wave;
	wave_data p2_wave;
	wave_data ai_wave;

	virtual void Alarm0();
	virtual void Alarm1();
	virtual void Alarm2();
	virtual void Alarm3();
	bool is_flea_spawned;
	//true = player 1, false = player 2
	bool active_player;

	std::list<Centipede*> centipede_list;
	Spider* spider_pointer;
	Flea* flea_pointer;
	Scorpion* scorpion_pointer;
	Bullet* bullet_pointer;

	int current_wave;
	float centipede_speed;
	float head_speed;
	int centipede_length;
	int extra_heads;
	float spider_speed;
	int spider_time_min;
	int spider_time_max;
	int scorpion_time_min;
	int scorpion_time_max;
	int temp_extra_heads;

	bool player_alive;
	bool head_primed;
	
public:
	static void Instantiate();
	static void SwapPlayer();
	static bool ReturnActivePlayer();
	static void ResetSpiderTimer();
	static void ResetScorpionTimer();
	static void SpawnFlea();
	static void SpawnCentipede(float xpos,float ypos,bool initial_direction,int segments);
	static void FleaCreated(Flea *in_flea_pointer);
	static void FleaDestroyed();
	static void SpiderCreated(Spider *in_spider_pointer);
	static void SpiderDestroyed();
	static void ScorpionCreated(Scorpion *in_scorpion_pointer);
	static void ScorpionDestroyed();
	static void CentipedeCreated(Centipede *in_centipede_pointer);
	static void CentipedeDestroyed(Centipede *in_centipede_pointer);
	static void CentipedeShot();
	static void BulletCreated(Bullet *in_bullet_pointer);
	static void BulletDestroyed();
	static void WaveStop();
	static void ResetWave();
	static void BeginWave();
	static void NextWave();
	static void LoadWaveData();
	static float ReturnCentipedeSpeed();
	static float ReturnHeadSpeed();
	static float ReturnSpiderSpeed();
	static void PlayerStatus(bool dead_alive);
	static void SwapWaves();
	static void ResetAllWaveTimers();
	static void PrimeExtraHead();
	static void ReleaseExtraHead();
	static bool CheckFleaScorpionWave();
};

#endif _WaveManager