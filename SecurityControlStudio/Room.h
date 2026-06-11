#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>

// Forward declarations, czyli "zapowiedzi" klas.
// W naglowku wystarczy wiedziec, ze takie klasy istnieja,
// bo przechowujemy tylko wskazniki: Door*, Person*, Sensor*.
// Pelne definicje klas sa potrzebne dopiero w plikach .cpp.
class Door;
class Person;
class Sensor;

class Room
{
public:
    Room(int id, const std::string& name);
    ~Room();

    int getId() const;
    std::string getName() const;

    void addDoor(Door* door);
    void addPerson(Person* person);
    void removePerson(Person* person);

    const std::vector<Door*>& getDoors() const;
    const std::vector<Person*>& getPeople() const;

    void setSensor(Sensor* sensor);
    Sensor* getSensor() const;
    void setCamera(bool value);
    bool hasCamera() const;

private:
    int id;
    std::string name;
    bool camera;

    // Room nie jest wlascicielem tych obiektow.
    // To znaczy: Room tylko pamieta adresy, ale nie robi delete.
    // Delete robi Simulation, a Building usuwa tylko same pokoje.
    std::vector<Door*> doors;
    std::vector<Person*> people;
    Sensor* sensor;
};

#endif // ROOM_H
