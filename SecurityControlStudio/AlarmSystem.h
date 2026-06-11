#ifndef ALARMSYSTEM_H
#define ALARMSYSTEM_H

class EventLog;
class Person;
class Room;

class AlarmSystem
{
public:
    explicit AlarmSystem(EventLog* log);

    void triggerAlarm(Room* room, Person* person, int stepNumber);
    bool isAlarmActive() const;
    void resetAlarm();

private:
    EventLog* log;
    bool alarmActive;
};

#endif // ALARMSYSTEM_H