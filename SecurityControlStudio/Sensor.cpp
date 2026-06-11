#include "Sensor.h"

#include "AlarmSystem.h"
#include "Person.h"
#include "Room.h"

#include <stdexcept>
#include <vector>

Sensor::Sensor(Room* room, AlarmSystem* alarmSystem)
    : room(room),
      alarmSystem(alarmSystem)
{
    if (room == nullptr || alarmSystem == nullptr)
    {
        throw std::invalid_argument("Sensor: room and alarmSystem cannot be null");
    }
}

void Sensor::check(int stepNumber)
{
    const std::vector<Person*>& people = room->getPeople();

    for (std::size_t i = 0; i < people.size(); ++i)
    {
        Person* person = people[i];

        // Polimorfizm: wywolujemy hasAccess() przez wskaznik Person*.
        // Dla Employee wykona sie Employee::hasAccess(), a dla Intruder
        // wykona sie Intruder::hasAccess(). To jest sens metod virtual.
        if (person != nullptr && person->hasAccess() == false)
        {
            alarmSystem->triggerAlarm(room, person, stepNumber);
        }
    }
}

Room* Sensor::getRoom() const
{
    return room;
}
