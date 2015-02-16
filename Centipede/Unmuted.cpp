#include "Unmuted.h"

Unmuted::Unmuted()
{
	//load all sounds
	Blaster_Fire.setBuffer( ResourceManager::GetSound("fire1")  );
	Centipede_Beat.setBuffer( ResourceManager::GetSound("newBeat") );
	Centipede_Kill.setBuffer( ResourceManager::GetSound("kill") );
	Flea_Drop.setBuffer( ResourceManager::GetSound("flea") );
	Scorpion_Loop.setBuffer( ResourceManager::GetSound("scorpion") );
	Spider_Loop.setBuffer( ResourceManager::GetSound("spider") );
	Blaster_Death.setBuffer( ResourceManager::GetSound("death") );
	Mushroom_Explosion.setBuffer( ResourceManager::GetSound("bonus") );
	OneUp_Noise.setBuffer( ResourceManager::GetSound("oneup") );
	beat_playing= false;
}

void Unmuted::BlasterPlay()
{
	Blaster_Fire.play();
}

void Unmuted::CentipedeBeatPlay()
{
	//check if the beat is already playing to avoid multiple loops at once
	if(beat_playing == false)
	{
		beat_playing= true;
		Centipede_Beat.setLoop(true);
		Centipede_Beat.play();
	}
}

void Unmuted::CentipedeBeatStop()
{
	if(beat_playing== true)
	{
		beat_playing= false;
		Centipede_Beat.stop();
	}
}

void Unmuted::CentipedeKillPlay()
{
	Centipede_Kill.play();
}

void Unmuted::FleaDropPlay()
{
	Flea_Drop.setLoop(false);
	Flea_Drop.play();
}

void Unmuted::FleaDropStop()
{
	Flea_Drop.stop();
}

void Unmuted::ScorpionPlay()
{
	Scorpion_Loop.setLoop(true);
	Scorpion_Loop.play();
}

void Unmuted::ScorpionStop()
{
	Scorpion_Loop.stop();
}

void Unmuted::SpiderPlay()
{
	Spider_Loop.setLoop(true);
	Spider_Loop.play();
}

void Unmuted::SpiderStop()
{
	Spider_Loop.stop();
}

void Unmuted::BlasterKill()
{
	Blaster_Death.play();
}

void Unmuted::MushroomRepair()
{
	Mushroom_Explosion.play();
}

void Unmuted::OneUp()
{
	OneUp_Noise.play();
}