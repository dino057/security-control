#include "Intruder.h"
#include "Room.h"

Intruder::Intruder(const std::string& name, Room* startRoom, Room* targetRoom)
    : Person(name, startRoom),
    targetRoom(targetRoom),
    escapeRoom(startRoom), // Ustawiamy startRoom jako miejsce ucieczki
    hasStolen(false)
{
}

bool Intruder::hasAccess()
{
    // Udaje pracownika (zwraca true) dopóki nie zdobędzie danych.
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

Room* Intruder::getEscapeRoom() const
{
    return escapeRoom;
}

bool Intruder::isMissionAccomplished() const
{
    return hasStolen;
}

void Intruder::setMissionAccomplished()
{
    hasStolen = true;
    // Po kradzieży, celem może stać się ucieczka do punktu startowego
}

