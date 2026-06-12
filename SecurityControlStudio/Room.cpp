#include "Room.h"
#include <algorithm>

Room::Room(int id, const std::string& name)
    : id(id),
    name(name),
    sensor(nullptr), // Na starcie pokój nie ma czujnika (jest pustym wskaźnikiem)
    camera(false)    // Domyślnie na mapie żaden pokój nie ma kamery
{
}

Room::~Room()
{

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
    // Zabezpieczenie przed podaniem pustego wskaźnika.
    if (door == nullptr)
    {
        return;
    }

    // std::find szuka w naszym wektorze 'doors' podanych drzwi.
    // Jeśli ich nie znalazł, funkcja zwraca wartość '.end()'.
    // Tylko wtedy dodajemy drzwi (żeby nie było duplikatów).
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

    // To samo co przy drzwiach - upewniamy się, że ta sama osoba
    // nie zostanie dodana do pokoju dwa razy.
    if (std::find(people.begin(), people.end(), person) == people.end())
    {
        people.push_back(person);
    }
}

void Room::removePerson(Person* person)
{
    // Szukamy osoby, która chce wyjść z pokoju. Zapisujemy wynik do tzw. iteratora ('found').
    // Iterator to taki wskaźnik, który pokazuje na konkretne miejsce wewnątrz wektora.
    std::vector<Person*>::iterator found = std::find(people.begin(), people.end(), person);

    // Jeśli 'found' NIE pokazuje na koniec wektora, to znaczy, że kogoś takiego znaleźliśmy.
    if (found != people.end())
    {
        // erase usuwa ten konkretny element z wektora i zsuwa resztę.
        people.erase(found);
    }
    // Jeśli osoby tu nie było, po prostu nic nie robimy.
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
    this->sensor = sensor; // Używamy 'this', żeby odróżnić zmienną klasy od argumentu funkcji
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