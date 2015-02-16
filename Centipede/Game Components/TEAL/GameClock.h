// GameClock
// Andre Berthiaume, April 2013

#ifndef _GameClock
#define _GameClock

#include "SFML\System\Clock.hpp"
#include "SFML\Window\Keyboard.hpp"

class GameClock
{
private:
	static const sf::Keyboard::Key FREEZEFRAME;
	static const sf::Keyboard::Key STEPONCE;

	sf::Clock frameclock;					// clock used to compute frame time intervals

	float TimeCurrentFrame;					// Total time since game started (not counting freeze frame)
	float TimePreviousFrame;				// Total time at the previous frame.
	float frametime;						// time interval since the last frame

	float ComputeFrameTime();				// Returns the time since the last frame (test for freeze frame)
	float FreezeFrameControl(float time);	// Processes freeze frame conditions

public:
	GameClock();								
	void ProcessTime();						

	float TotalTime();						
	float FrameTime();
};

#endif _GameClock