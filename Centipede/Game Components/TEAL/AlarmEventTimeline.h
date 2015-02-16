// AlarmEventTimeline.h
// Andre Berthiaume, 2012
//
// Processes alarm events for GameObbjects

#ifndef _AlarmEventTimeline
#define _AlarmEventTimeline

#include <map>

// Forward declarations 
class GameObject;

class AlarmEventTimeline
{
public:
	typedef void (*EventOperation)(GameObject*);

private:
	typedef std::map< GameObject*, float > TimeEventRequesters;
	typedef std::multimap< float, GameObject* > Timeline;

	EventOperation EventOp;
	TimeEventRequesters registeredGameObjects;
	Timeline requestedAlarms;

public:
	AlarmEventTimeline(){};
	void SetOperation( EventOperation f);
	void SetAlarm(GameObject &go, float time);
	float GetAlarm(GameObject &go);
	void AddTime(GameObject &go, float);
	bool AlarmActive(GameObject &go);
	void AlarmCancel(GameObject &go);
	void ProcessAlarmEvents();

	void ClearAllAlarms(GameObject &go);
};


#endif _AlarmEventTimeline