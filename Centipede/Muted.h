//#ifndef _Muted
//#define _Muted

#include "SoundObject.h"

class Muted : public SoundObject
{
	//all events are empty - this is the muted class, so it does nothing

public:
	Muted(){ };
	virtual void BlasterPlay(){ };
	virtual void CentipedeBeatPlay(){ };
	virtual void CentipedeBeatStop(){ };
	virtual void CentipedeKillPlay(){ };
	virtual void FleaDropPlay(){ };
	virtual void FleaDropStop(){ };
	virtual void ScorpionPlay(){ };
	virtual void ScorpionStop(){ };
	virtual void SpiderPlay(){ };
	virtual void SpiderStop(){ };
	virtual void BlasterKill(){ };
	virtual void MushroomRepair(){ };
	virtual void OneUp(){ };

private:


};

//#endif