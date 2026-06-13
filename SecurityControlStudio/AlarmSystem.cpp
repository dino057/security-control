#include "AlarmSystem.h"

#include "Door.h"
#include "EventLog.h"
#include "Person.h"
#include "Room.h"

#include <sstream>
#include <stdexcept>
#include <vector>

// Konstruktor używa tzw. "listy inicjalizacyjnej"
AlarmSystem::AlarmSystem(EventLog* log)
    : log(log), alarmActive(false) // Od razu zapamiętujemy nasz 'log' i upewniamy się, że alarm jest wyłączony
{
    if (log == nullptr)
    {
        throw std::invalid_argument("AlarmSystem: log pointer cannot be null");
    }
}

void AlarmSystem::triggerAlarm(Room* room, Person* person, int stepNumber)
{
    // Kolejne zabezpieczenie: jeśli system wykrył alarm, ale nie dostał informacji O TYM KTO (person)
    // ani GDZIE (room) to zrobił, to odpuszczamy i wychodzimy z funkcji (return).
    if (room == nullptr || person == nullptr) return;

    // Ważny warunek: reagujemy TYLKO wtedy, gdy alarm jeszcze nie wyje (!alarmActive).
    // Jeśli byśmy tego nie dodali, w jednym kroku wypisalibyśmy 10 razy "ALARM!", bo intruza widzi naraz kamera i czujnik.
    if (!alarmActive) // Zeby nie spamowac logow co ture
    {
        alarmActive = true; // Zmieniamy flagę - od teraz cały budynek wie o włamaniu.

        std::ostringstream alarmMessage;

        alarmMessage << "[Krok " << stepNumber << "] ALARM! Czujnik w pokoju "
                     << room->getName() << " wykryl intruza! Ochrona rozpoczyna poscig!";

        // Wyciągamy sklejony tekst (używając funkcji .str()) i wrzucamy go do głównego dziennika.
        log->add(alarmMessage.str());
    }
}

bool AlarmSystem::isAlarmActive() const
{
    return alarmActive;
}

void AlarmSystem::resetAlarm()
{
    // Funkcja gasząca alarm
    alarmActive = false;
}