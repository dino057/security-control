#ifndef PERSON_H
#define PERSON_H

#include <string>

class Room;

class Person
{
public:
    Person(const std::string& name, Room* startRoom);
    virtual ~Person();

    std::string getName() const;
    Room* getCurrentRoom() const;

    // Metoda czysto wirtualna: "= 0" oznacza, ze Person jest klasa abstrakcyjna.
    // Nie da sie utworzyc zwyklej Person, trzeba utworzyc Employee albo Intruder.
    virtual bool hasAccess() = 0;

    // Druga metoda wirtualna pozwala ladnie wypisac typ osoby w logach.
    virtual std::string getKind() const = 0;

    virtual void move(Room* target);

protected:
    std::string name;

    // Wskaznik na aktualny pokoj. Person nie robi delete na pokoju,
    // bo pokoje sa wlasnoscia klasy Building.
    Room* currentRoom;
};

#endif // PERSON_H
