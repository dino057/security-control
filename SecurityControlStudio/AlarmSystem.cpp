#include "AlarmSystem.h"

#include "Door.h"
#include "EventLog.h"
#include "Person.h"
#include "Room.h"

#include <sstream>
#include <stdexcept>
#include <vector>

AlarmSystem::AlarmSystem(EventLog* log)
    : log(log), alarmActive(false)
{
    if (log == nullptr)
    {
        throw std::invalid_argument("AlarmSystem: log pointer cannot be null");
    }
}

void AlarmSystem::triggerAlarm(Room* room, Person* person, int stepNumber)
{
    if (room == nullptr || person == nullptr) return;

    if (!alarmActive) // Zeby nie spamowac logow co ture
    {
        alarmActive = true;
        std::ostringstream alarmMessage;
        alarmMessage << "[Krok " << stepNumber << "] ALARM! Kamera w "
                     << room->getName() << " wykryla intruza! Ochrona rozpoczyna poscig!";
        log->add(alarmMessage.str());
    }
}
bool AlarmSystem::isAlarmActive() const
{
    return alarmActive;
}

void AlarmSystem::resetAlarm()
{
    alarmActive = false;
}