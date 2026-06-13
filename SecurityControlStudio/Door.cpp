#include "Door.h"

#include <stdexcept>

Door::Door(Room* firstRoom, Room* secondRoom)
    : firstRoom(firstRoom),
    secondRoom(secondRoom),
    locked(false)
{
    // Zabezpieczenie przed "drzwiami donikąd".
    // nullptr oznacza pusty wskaźnik, czyli brak obiektu pod danym adresem w pamięci.
    if (firstRoom == nullptr || secondRoom == nullptr)
    {
        // Rzucamy wyjątkiem, co zatrzyma program ze stosownym komunikatem błędu.
        throw std::invalid_argument("Door: room pointer cannot be null");
    }
}

// Zwykłe "gettery" (metody typu get). Służą tylko do tego, żeby inne klasy
// mogły podglądnąć prywatne zmienne tych drzwi.
Room* Door::getFirstRoom() const
{
    return firstRoom;
}

Room* Door::getSecondRoom() const
{
    return secondRoom;
}

// Algorytm FBS
Room* Door::getOtherRoom(Room* room) const
{
    // Jeśli stoję w "pierwszym", to idę do "drugiego"
    if (room == firstRoom)
    {
        return secondRoom;
    }

    // A jeśli stoję w "drugim", to idę do "pierwszego"
    if (room == secondRoom)
    {
        return firstRoom;
    }

    // Jeśli podaliśmy pokój, który w ogóle nie jest połączony z tymi drzwiami:
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