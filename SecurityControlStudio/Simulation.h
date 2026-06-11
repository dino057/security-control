#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include <vector>

class AlarmSystem;
class Building;
class Door;
class EventLog;
class Person;
class Room;
class Sensor;

class Simulation
{
public:
    Simulation();
    ~Simulation();

    bool loadFromFile(const std::string& fileName, std::string& errorMessage);
    void step();

    std::string getLogText() const;
    bool isLoaded() const;
    int getStepNumber() const;
    const std::vector<Room*>& getRooms() const;
    const std::vector<Door*>& getDoors() const;
    const std::vector<Person*>& getPeople() const;
    bool isFinished() const;

private:
    void clearDynamicObjects();
    void createSensorsForAllRooms();
    bool parseDoorsAndPeople(const std::string& fileName, std::string& errorMessage);
    void movePeople();
    void checkSensors();
    Room* getNextRoomTowards(Room* start, Room* target);

    // Te wskazniki pokazuja reczne zarzadzanie pamiecia.
    // Obiekty powstaja przez new w konstruktorze lub podczas wczytywania mapy,
    // a potem sa usuwane przez delete w destruktorze albo przy resecie.
    EventLog* eventLog;
    Building* building;
    AlarmSystem* alarmSystem;

    std::vector<Person*> people;
    std::vector<Door*> doors;
    std::vector<Sensor*> sensors;

    int stepNumber;
    bool loaded;
    bool finished;
};

#endif // SIMULATION_H
