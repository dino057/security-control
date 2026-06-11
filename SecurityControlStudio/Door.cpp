#include "Door.h"

#include <stdexcept>

Door::Door(Room* firstRoom, Room* secondRoom)
    : firstRoom(firstRoom),
      secondRoom(secondRoom),
      locked(false)
{
    // Wyjatek chroni nas przed bledem typu "drzwi do niczego".
    // nullptr oznacza pusty wskaznik, czyli brak obiektu pod adresem.
    if (firstRoom == nullptr || secondRoom == nullptr)
    {
        throw std::invalid_argument("Door: room pointer cannot be null");
    }
}

Room* Door::getFirstRoom() const
{
    return firstRoom;
}

Room* Door::getSecondRoom() const
{
    return secondRoom;
}

Room* Door::getOtherRoom(Room* room) const
{
    if (room == firstRoom)
    {
        return secondRoom;
    }

    if (room == secondRoom)
    {
        return firstRoom;
    }

    return nullptr;
}

bool Door::isLocked() const
{
    return locked;
}

void Door::lock()
{
    locked = true;
}

void Door::unlock()
{
    locked = false;
}
