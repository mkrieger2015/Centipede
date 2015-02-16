#include "SoundManager.h"
#include "Unmuted.h"
#include "Muted.h"

int SoundManager::selected_object= 0;
SoundManager* SoundManager::self= NULL;

void SoundManager::Instantiate()
{
	//creates one unmuted and one muted object (strategy pattern), and sets the initially selected one to 
	Instance();
	Instance().unmuted_object= new Unmuted();
	Instance().muted_object= new Muted();
	Instance().current_object= Instance().muted_object;
	Instance().selected_object= 1;
}

void SoundManager::SwapObjects()
{
	//swaps selected object depending on mode. like all the other swappers
	if(Instance().selected_object == 1)
	{
		Instance().current_object= Instance().unmuted_object;
		Instance().selected_object= 2;
	}
	else if (Instance().selected_object == 2)
	{
		Instance().current_object= Instance().muted_object;
		Instance().selected_object= 1;
	}
}


//these are all just accessor functions
void SoundManager::BlasterPlay()
{
	Instance().current_object->BlasterPlay();
}

void SoundManager::CentipedeBeatPlay()
{
	Instance().current_object->CentipedeBeatPlay();
}

void SoundManager::CentipedeKillPlay()
{
	Instance().current_object->CentipedeKillPlay();
}

void SoundManager::CentipedeBeatStop()
{
	Instance().current_object->CentipedeBeatStop();
}

void SoundManager::FleaDropPlay()
{
	Instance().current_object->FleaDropPlay();
}

void SoundManager::FleaDropStop()
{
	Instance().current_object->FleaDropStop();
}

void SoundManager::ScorpionPlay()
{
	Instance().current_object->ScorpionPlay();
}

void SoundManager::ScorpionStop()
{
	Instance().current_object->ScorpionStop();
}

void SoundManager::SpiderPlay()
{
	Instance().current_object->SpiderPlay();
}

void SoundManager::SpiderStop()
{
	Instance().current_object->SpiderStop();
}

void SoundManager::BlasterKill()
{
	Instance().current_object->BlasterKill();
}

void SoundManager::MushroomRepair()
{
	Instance().current_object->MushroomRepair();
}

void SoundManager::DeleteAllSoundObjects()
{
	//destroys both sound objects
	Instance().unmuted_object->MarkForDestroy();
	Instance().muted_object->MarkForDestroy();
}

void SoundManager::OneUp()
{
	Instance().current_object->OneUp();
}