#include "Room.h"

#include <algorithm>

Room::Room(int id, const std::string& name)
    : id(id),
    name(name),
    sensor(nullptr),
    camera(false) // Domyslnie pokoj nie ma kamery
{
}


Room::~Room()
{
    // Tutaj celowo nie ma delete dla drzwi, ludzi ani czujnika.
    // Room nie jest ich wlascicielem. Gdyby kilka klas robilo delete
    // na tym samym wskazniku, program moglby sie wysypac.
}

int Room::getId() const
{
    return id;
}

std::string Room::getName() const
{
    return name;
}

void Room::addDoor(Door* door)
{
    if (door == nullptr)
    {
        return;
    }

    if (std::find(doors.begin(), doors.end(), door) == doors.end())
    {
        doors.push_back(door);
    }
}

void Room::addPerson(Person* person)
{
    if (person == nullptr)
    {
        return;
    }

    if (std::find(people.begin(), people.end(), person) == people.end())
    {
        people.push_back(person);
    }
}

void Room::removePerson(Person* person)
{
    // erase usuwa element z wektora, ale tylko wtedy, gdy go znajdziemy.
    // Jesli osoby nie ma w pokoju, metoda nic nie robi. To jest celowe,
    // bo wymaganie projektu mowi, ze brak osoby ma byc ignorowany.
    std::vector<Person*>::iterator found = std::find(people.begin(), people.end(), person);

    if (found != people.end())
    {
        people.erase(found);
    }
}

const std::vector<Door*>& Room::getDoors() const
{
    return doors;
}

const std::vector<Person*>& Room::getPeople() const
{
    return people;
}

void Room::setSensor(Sensor* sensor)
{
    this->sensor = sensor;
}

Sensor* Room::getSensor() const
{
    return sensor;
}

void Room::setCamera(bool value)
{
    camera = value;
}

bool Room::hasCamera() const
{
    return camera;
}