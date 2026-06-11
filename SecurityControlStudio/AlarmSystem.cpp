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
    if (room == nullptr || person == nullptr)
    {
        return;
    }

    alarmActive = true;

    std::ostringstream alarmMessage;
    alarmMessage << "[krok " << stepNumber << "] ALARM: czujnik w pokoju "
                 << room->getId() << " (" << room->getName() << ") wykryl osobe bez dostepu: "
                 << person->getName() << " (" << person->getKind() << ")";
    log->add(alarmMessage.str());

    const std::vector<Door*>& doors = room->getDoors();

    if (doors.empty())
    {
        log->add("[krok " + std::to_string(stepNumber) + "] Pokoj nie ma drzwi do zablokowania.");
        return;
    }

    for (std::size_t i = 0; i < doors.size(); ++i)
    {
        Door* door = doors[i];

        if (door != nullptr && !door->isLocked())
        {
            door->lock();

            Room* a = door->getFirstRoom();
            Room* b = door->getSecondRoom();

            std::ostringstream lockMessage;
            lockMessage << "[krok " << stepNumber << "] Zablokowano drzwi "
                        << a->getId() << " <-> " << b->getId();
            log->add(lockMessage.str());
        }
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