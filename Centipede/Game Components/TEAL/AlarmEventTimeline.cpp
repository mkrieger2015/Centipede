// AlarmEventTimeline.cpp
// Andre Berthiaume, 2012
//
// Processes alarm events for GameObbjects

#include "Game.h"
#include "AlarmManager.h"

using namespace std;

void AlarmEventTimeline::SetOperation( EventOperation f)
{
	EventOp = f;
}

void AlarmEventTimeline::SetAlarm(GameObject &go, float time)
{
	TimeEventRequesters::iterator it;

	it = registeredGameObjects.find(&go);
	if (it != registeredGameObjects.end() )
	{
		// Erase the old timeline entry
		pair<Timeline::iterator, Timeline::iterator> tlitpair = requestedAlarms.equal_range(it->second );

		if (tlitpair.first != tlitpair.second) // This test needs to be confirmed...
		{

			Timeline::iterator tlit = tlitpair.first;
			while( tlit != tlitpair.second && tlit->second != &go)
				tlit++;
			requestedAlarms.erase(tlit);
		}

		// adjust the new time in the registration list
		it->second = Game::TotalTime() + time;

		// Update the timeline ordering correspondingly
		requestedAlarms.insert( pair<float, GameObject*>(it->second , &go) );
	}
	else
	{	// new entry
		registeredGameObjects[&go] = Game::TotalTime() + time;
		requestedAlarms.insert( pair<float, GameObject*>(Game::TotalTime() + time , &go) );
	}
}

void AlarmEventTimeline::AlarmCancel(GameObject &go)
{
	TimeEventRequesters::iterator it;

	it = registeredGameObjects.find(&go);
	if (it != registeredGameObjects.end() )
	{
		// Erase the old timeline entry
		pair<Timeline::iterator, Timeline::iterator> tlitpair = requestedAlarms.equal_range(it->second );

		if (tlitpair.first != tlitpair.second) // This test needs to be confirmed...
		{

			Timeline::iterator tlit = tlitpair.first;
			while( tlit != tlitpair.second && tlit->second != &go)
				tlit++;
			requestedAlarms.erase(tlit);
		}

		registeredGameObjects.erase(it);
	}
}

void AlarmEventTimeline::AddTime(GameObject& go, float time)
{
	TimeEventRequesters::iterator it;

	it = registeredGameObjects.find(&go);
	if (it != registeredGameObjects.end() )
	{
		// adjust the new time in the registration list
		it->second += time;

		// Erase the old timeline entry
		pair<Timeline::iterator, Timeline::iterator> tlitpair = requestedAlarms.equal_range(Game::TotalTime() + time);
		Timeline::iterator tlit = tlitpair.first;
		while( tlit != tlitpair.second && tlit->second != &go)
			tlit++;
		requestedAlarms.erase(tlit);

		// Update the timeline ordering correspondingly
		requestedAlarms.insert( pair<float, GameObject*>(it->second , &go) );
	}
	else // go is not registered, so consider this 'add' as a new registration instead
		SetAlarm(go, time);
}

float AlarmEventTimeline::GetAlarm(GameObject& go)
{
	TimeEventRequesters::iterator it;

	it = registeredGameObjects.find(&go);
	if (it != registeredGameObjects.end() )
	{
		return  it->second - Game::TotalTime();
	}
	else
		return -1;
}

bool AlarmEventTimeline::AlarmActive(GameObject& go)
{
	// true if find(go) != end()
	return registeredGameObjects.find(&go) != registeredGameObjects.end();
}

void AlarmEventTimeline::ProcessAlarmEvents()
{
	Timeline::iterator it = requestedAlarms.begin(); // Get first timeline event
	GameObject *gotemp;

	// from the top of the list, process each alarm event that has passed
	while (it != requestedAlarms.end() && it->first <= Game::TotalTime() )
	{
		// Important: since an alarm event may itself set an alarm and change the map we are looping in,
		// we need to clean up the map _before_ performing the callback!
		gotemp = it->second;
		registeredGameObjects.erase(it->second);	// Deregister the object
		requestedAlarms.erase(it);					// remove the timeline event	
		(*EventOp)( gotemp );						// perform callback (which might reset the alarm and modify this map)

		it = requestedAlarms.begin();				// get new earliest timeline event
	}

}

void AlarmEventTimeline::ClearAllAlarms(GameObject &go)
{
	TimeEventRequesters::iterator it = registeredGameObjects.find(&go);

	if ( it != registeredGameObjects.end() )
	{
		std::pair< Timeline::iterator, Timeline::iterator> p = requestedAlarms.equal_range(it->second);

		Timeline::iterator it2 = p.first;
		while ( it2 != requestedAlarms.end() && it2 != p.second)
		{
			if ( it2->second == &go)
			{
				requestedAlarms.erase( it2 );
				registeredGameObjects.erase(it);
				break;
			}
			it2++;
		}
	}
}


