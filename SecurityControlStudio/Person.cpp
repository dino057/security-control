#include "Person.h"

#include "Room.h"

Person::Person(const std::string& name, Room* startRoom)
    : name(name),
      currentRoom(startRoom)
{
    if (currentRoom != nullptr)
    {
        currentRoom->addPerson(this);
    }
}

Person::~Person()
{
    // Destruktor jest virtual, bo usuwamy obiekty przez wskaznik Person*.
    // Bez virtual destruktor klasy pochodnej moglby nie wykonac sie poprawnie.
}

std::string Person::getName() const
{
    return name;
}

Room* Person::getCurrentRoom() const
{
    return currentRoom;
}

void Person::move(Room* target)
{
    // Wymaganie projektu: move ma ignorowac nulle.
    // Gdy target == nullptr, po prostu zostajemy w miejscu.
    if (target == nullptr)
    {
        return;
    }

    if (target == currentRoom)
    {
        return;
    }

    if (currentRoom != nullptr)
    {
        currentRoom->removePerson(this);
    }

    target->addPerson(this);
    currentRoom = target;
}
