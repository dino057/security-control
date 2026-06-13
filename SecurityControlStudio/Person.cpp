#include "Person.h"
#include "Room.h"

Person::Person(const std::string& name, Room* startRoom)
    : name(name),
    currentRoom(startRoom)
{
    // Jeśli pokój startowy faktycznie istnieje (nie jest pusty/nullem),
    // to Człowiek wie, w którym jest pokoju, a Pokój wie, kto w nim stoi.
    if (currentRoom != nullptr)
    {
        currentRoom->addPerson(this); // 'this' oznacza "mnie samego"
    }
}

Person::~Person()
{

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
    // Zabezpieczenie: jeśli system każe nam iść donikąd (nullptr), ignorujemy rozkaz.
    if (target == nullptr)
    {
        return;
    }

    // Zabezpieczenie: jeśli każą nam iść do pokoju, w którym już stoimy, nie robimy nic.
    if (target == currentRoom)
    {
        return;
    }

    // Jeśli wszystko jest OK, rozpoczynamy przejście.
    // 1. Najpierw wypisujemy się ze starego pokoju.
    if (currentRoom != nullptr)
    {
        currentRoom->removePerson(this);
    }

    // 2. Następnie dopisujemy się do listy gości w nowym pokoju.
    target->addPerson(this);

    // 3. Na koniec aktualizujemy sobie w głowie informację, gdzie teraz stoimy.
    currentRoom = target;
}