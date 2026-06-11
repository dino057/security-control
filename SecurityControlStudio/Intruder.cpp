#include "Intruder.h"

Intruder::Intruder(const std::string& name, Room* startRoom, Room* targetRoom)
    : Person(name, startRoom),
    targetRoom(targetRoom),
    hasStolen(false) // Na poczatku intruz ma idealny kamuflaz
{
}

bool Intruder::hasAccess()
{
    // Udaje pracownika (zwraca true) dopoki nie zdobedzie danych.
    // Gdy tylko dokona kradziezy (hasStolen == true), traci kamuflaz
    // i system alarmowy w serwerowni natychmiast go wykrywa!
    if (!hasStolen)
    {
        return true;
    }
    return false;
}

std::string Intruder::getKind() const
{
    return "intruz";
}

Room* Intruder::getTargetRoom() const
{
    return targetRoom;
}

bool Intruder::isMissionAccomplished() const
{
    return hasStolen;
}

void Intruder::setMissionAccomplished()
{
    hasStolen = true;
}