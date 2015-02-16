// AlarmManager.cpp
// Andre Berthiaume, 2012
//
// All Game time and alarm event processing

#include "AlarmManager.h"
#include "GameObject.h"

AlarmManager::AlarmManager()
{
	eventTimelines[0].SetOperation( CallAlarm0 );
	eventTimelines[1].SetOperation( CallAlarm1 );
	eventTimelines[2].SetOperation( CallAlarm2 );
	eventTimelines[3].SetOperation( CallAlarm3 );
	eventTimelines[4].SetOperation( CallAlarm4 );
}

void AlarmManager::CallAlarm0(GameObject* go){ go->Alarm0();}
void AlarmManager::CallAlarm1(GameObject* go){ go->Alarm1();}
void AlarmManager::CallAlarm2(GameObject* go){ go->Alarm2();}
void AlarmManager::CallAlarm3(GameObject* go){ go->Alarm3();}
void AlarmManager::CallAlarm4(GameObject* go){ go->Alarm4();}

void AlarmManager::ProcessTimeEvents()
{
	// Processing alarm events
	for(int i = 0; i < ALARMCOUNT; i++)
		eventTimelines[i].ProcessAlarmEvents();
}

void AlarmManager::ClearAllAlarms(GameObject &go)
{
	for(int i = 0; i < ALARMCOUNT; i++)
		eventTimelines[i].ClearAllAlarms(go);
}

void AlarmManager::SetAlarm(GameObject& go, int AlarmNumber, float time){eventTimelines[ AlarmNumber ].SetAlarm(go, time); } 
float AlarmManager::GetAlarm(GameObject& go, int AlarmNumber){ return eventTimelines[ AlarmNumber ].GetAlarm(go); }
void AlarmManager::AddTime(GameObject& go, int AlarmNumber, float time){ eventTimelines[ AlarmNumber ].AddTime(go, time); }
bool AlarmManager::AlarmActive(GameObject& go, int AlarmNumber){ return eventTimelines[ AlarmNumber ].AlarmActive(go);}
void AlarmManager::AlarmCancel(GameObject& go, int AlarmNumber){ eventTimelines[ AlarmNumber ].AlarmCancel(go); }
