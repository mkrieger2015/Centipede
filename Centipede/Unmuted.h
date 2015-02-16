#ifndef _Unmuted
#define _Unmuted

#include "SoundObject.h"

class Unmuted : public SoundObject
{

public:
	Unmuted();
	virtual void BlasterPlay();
	virtual void CentipedeBeatPlay();
	virtual void CentipedeBeatStop();
	virtual void CentipedeKillPlay();
	virtual void FleaDropPlay();
	virtual void FleaDropStop();
	virtual void ScorpionPlay();
	virtual void ScorpionStop();
	virtual void SpiderPlay();
	virtual void SpiderStop();
	virtual void BlasterKill();
	virtual void MushroomRepair();
	virtual void OneUp();

private:
	bool beat_playing;
	sf::Sound Blaster_Fire;
	sf::Sound Centipede_Beat;
	sf::Sound Centipede_Kill;
	sf::Sound Flea_Drop;
	sf::Sound Scorpion_Loop;
	sf::Sound Spider_Loop;
	sf::Sound Blaster_Death;
	sf::Sound Mushroom_Explosion;
	sf::Sound OneUp_Noise;

};

#endif