#ifndef _SoundManager
#define _SoundManager

#include "Game Components\TEAL\CommonElements.h"

class SoundObject;

class SoundManager : public GameObject
{
private: 
	static SoundManager *self;

	SoundManager(){ };
	SoundManager(const SoundManager&){ };
	SoundManager& operator= (const SoundManager&){ };

	~SoundManager(){ };
	
	static SoundManager& Instance()
	{
		if (!self)
			self= new SoundManager();
		return *self;
	};

	SoundObject *current_object;
	SoundObject *unmuted_object;
	SoundObject *muted_object;
	
	static int selected_object;

public:
	static void Instantiate();
	static void SwapObjects();

	static void BlasterPlay();
	static void CentipedeBeatPlay();
	static void CentipedeBeatStop();
	static void CentipedeKillPlay();
	static void FleaDropPlay();
	static void FleaDropStop();
	static void ScorpionPlay();
	static void ScorpionStop();
	static void SpiderPlay();
	static void SpiderStop();
	static void BlasterKill();
	static void MushroomRepair();
	static void OneUp();

	static void DeleteAllSoundObjects();
};

#endif