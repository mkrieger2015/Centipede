#ifndef _SoundObject
#define _SoundObject

#include "Game Components/TEAL/CommonElements.h"

//generic parent class for strategy pattern. not much to see

class SoundObject : public GameObject
{
public:
	virtual void BlasterPlay(){ };
	virtual void CentipedeBeatPlay(){ };
	virtual void CentipedeKillPlay(){ };
	virtual void CentipedeBeatStop(){ };
	virtual void FleaDropPlay(){ };
	virtual void FleaDropStop(){ };
	virtual void ScorpionPlay(){ };
	virtual void ScorpionStop(){ };
	virtual void SpiderPlay(){ };
	virtual void SpiderStop(){ };
	virtual void BlasterKill(){ };
	virtual void MushroomRepair(){ };
	virtual void OneUp(){ };
};

#endif