// AlarmManager.h
// Andre Berthiaume, 2012
//
// All Game time and alarm event processing

#ifndef _AlarmManager
#define _AlarmManager

#include "AlarmEventTimeline.h"

// forward declarations
class GameObject;

class AlarmManager
{
private:
	static const int ALARMCOUNT = 5;
	AlarmEventTimeline eventTimelines[ALARMCOUNT];

	static void CallAlarm0(GameObject* go);
	static void CallAlarm1(GameObject* go);
	static void CallAlarm2(GameObject* go);
	static void CallAlarm3(GameObject* go);
	static void CallAlarm4(GameObject* go);

public:
	AlarmManager();

	void ProcessTimeEvents();
	
	void SetAlarm(GameObject& go, int AlarmNumber, float time);
	float GetAlarm(GameObject& go, int AlarmNumber);
	void AddTime(GameObject& go, int AlarmNumber, float time);
	bool AlarmActive(GameObject& go, int AlarmNumber);
	void AlarmCancel(GameObject& go, int AlarmNumber);

	void ClearAllAlarms(GameObject &go);
};

#endif _AlarmManager