// GameClock
// Andre Berthiaume, April 2013

#include "Game.h"
#include "GameClock.h"
#include "Tools.h"
#include "WindowManager.h"

const sf::Keyboard::Key GameClock::FREEZEFRAME = sf::Keyboard::F12;
const sf::Keyboard::Key GameClock::STEPONCE = sf::Keyboard::F11;


GameClock::GameClock()
{
	frameclock.restart();
	TimeCurrentFrame = 0;
	TimePreviousFrame = 0;
}

/// \brief Main time processing call.
void GameClock::ProcessTime()
{
	TimePreviousFrame = TimeCurrentFrame;
	TimeCurrentFrame += ComputeFrameTime();

	// Resets the interval clock to compute the next frame
	frameclock.restart();

	// frametime is computed once (for this frame) rather than computed at every call request;
	frametime = TimeCurrentFrame - TimePreviousFrame;
}


/// \brief Gets the total time.
///
/// \return The total number elapsed time since the game started.
float GameClock::TotalTime()
{
	return TimeCurrentFrame;
}

/// \brief The elapsed time since the last frame.
///
/// \return The elapsed time since the last frame.
float GameClock::FrameTime()
{
	return	frametime;
}

/// \brief Calculates the elapsed time since the last frame. 
///
/// \return Elapsed time since the laset frame or 1/60 second if stepping in freeze frame mode.
float GameClock::ComputeFrameTime()
{
	float time = frameclock.getElapsedTime().asSeconds();

	time = FreezeFrameControl(time);

	return time;
}

/// \brief Freeze frame control.
///
/// \param time The elasped time since the last frame.
///
/// \return normally returns the original time value. But in freeze frame more (F12) will return 1/60 each time F11 is pressed 
float GameClock::FreezeFrameControl(float time)
{
	static bool freeze = false;

	// Test for a freeze-frame mode request
	if ( sf::Keyboard::isKeyPressed( FREEZEFRAME )	// either a keypress to request freeze-frame
		  || freeze )								// or returning here after a single-frame request
	{
		// stall until the FREEZEFRAME key is release
		Tools::WaitForRelease( FREEZEFRAME );		

		freeze = true;
		bool onceonly = false;

		// Loops here unil either freeze frame mode is released or single frame requested
		while ( freeze && ! onceonly )
		{
			// Test for cancelling freeze frame (or game exit)
			if ( sf::Keyboard::isKeyPressed( FREEZEFRAME ) || sf::Keyboard::isKeyPressed( Game::KILLGAMEKEY )  )
			{
				freeze = false;
				onceonly = false;
				Tools::WaitForRelease( FREEZEFRAME );
			} 
			// Test for single frame request
			else if ( sf::Keyboard::isKeyPressed( STEPONCE ) )
			{
				onceonly = true;
 				Tools::WaitForRelease( STEPONCE );
			} 

			time = frametime; // Coming out of this loop, the (internal) time should be defaulted to last non-freeze mode frametime interval
		}
	}

	return time;
}