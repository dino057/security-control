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
    // Zabezpieczenie: czujnik nie moze wisiec w "powietrzu" ani nie byc podlaczony do systemu.
    // Jesli dostaniemy pusty wskaznik (nullptr), od razu przerywamy program rzucajac blad.
    if (room == nullptr || alarmSystem == nullptr)
    {
        throw std::invalid_argument("Sensor: room and alarmSystem cannot be null");
    }
}

void Sensor::check(int stepNumber)
{
    // Pobieramy liste wszystkich osob, ktore aktualnie stoja w obserwowanym pokoju.
    const std::vector<Person*>& people = room->getPeople();

    for (std::size_t i = 0; i < people.size(); ++i)
    {
        Person* person = people[i];

        // Znów używamy polimorfizmu:
        // Petla leci po ogolnych wskaznikach 'Person*'. Czujnik nie ma pojecia, czy patrzy na
        // pracownika, ochroniarza czy intruza. Po prostu w ciemno wywoluje 'hasAccess()'.
        // C++ samo w locie sprawdza pod spodem, z kim ma do czynienia, i wykonuje odpowiednia wersje kodu.
        if (person != nullptr && person->hasAccess() == false)
        {
            // Jesli hasAccess zwrocilo fałsz (czyli intruz wpadl), natychmiast odpalamy globalny alarm!
            alarmSystem->triggerAlarm(room, person, stepNumber);
        }
    }
}

Room* Sensor::getRoom() const
{
    return room;
}