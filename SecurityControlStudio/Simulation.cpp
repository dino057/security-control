#include "Simulation.h"
#include "AlarmSystem.h"
#include "Building.h"
#include "Door.h"
#include "Employee.h"
#include "EventLog.h"
#include "Intruder.h"
#include "Guard.h"
#include "Person.h"
#include "Room.h"
#include "Sensor.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <map>
#include <algorithm>

namespace
{
std::string trim(const std::string& text)
{
    std::size_t first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    std::size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}
}

Simulation::Simulation()
    : eventLog(new EventLog()),
    building(new Building()),
    alarmSystem(new AlarmSystem(eventLog)),
    stepNumber(0),
    loaded(false),
    finished(false)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

Simulation::~Simulation()
{
    clearDynamicObjects();
    delete alarmSystem;
    delete building;
    delete eventLog;
}

bool Simulation::isFinished() const
{
    return finished;
}

bool Simulation::loadFromFile(const std::string& fileName, std::string& errorMessage)
{
    clearDynamicObjects();
    eventLog->clear();

    if (!building->loadFromFile(fileName, errorMessage))
    {
        eventLog->add("[blad] " + errorMessage);
        return false;
    }

    try
    {
        createSensorsForAllRooms();

        if (!parseDoorsAndPeople(fileName, errorMessage))
        {
            clearDynamicObjects();
            eventLog->add("[blad] " + errorMessage);
            return false;
        }
    }
    catch (const std::exception& exception)
    {
        errorMessage = exception.what();
        clearDynamicObjects();
        eventLog->add("[blad] " + errorMessage);
        return false;
    }

    loaded = true;
    eventLog->add("[start] Wczytano mape: " + fileName);
    eventLog->add("[start] Liczba osob: " + std::to_string(people.size()));
    return true;
}

void Simulation::step()
{
    if (!loaded || finished) return;

    ++stepNumber;
    eventLog->add("\n[krok " + std::to_string(stepNumber) + "] --- poczatek kroku ---");

    movePeople();
    checkSensors();

    bool alarm = alarmSystem->isAlarmActive();
    for (std::size_t i = 0; i < people.size(); ++i)
    {
        if (people[i] == nullptr) continue;

        if (alarm && people[i]->getKind() == "ochroniarz")
        {
            Room* r = people[i]->getCurrentRoom();
            const std::vector<Person*>& inRoom = r->getPeople();
            for (std::size_t j = 0; j < inRoom.size(); ++j)
            {
                if (inRoom[j]->getKind() == "intruz")
                {
                    eventLog->add("\n[SUKCES OCHRONY] Ochroniarz " + people[i]->getName() + " ujal intruza " + inRoom[j]->getName() + " w pokoju " + std::to_string(r->getId()) + "!");
                    finished = true;
                    return;
                }
            }
        }

        if (people[i]->getKind() == "intruz")
        {
            Intruder* intr = static_cast<Intruder*>(people[i]);
            if (intr->getCurrentRoom()->getId() == 1 && intr->isMissionAccomplished())
            {
                eventLog->add("\n[PORAZKA OCHRONY] Intruz " + intr->getName() + " UCIEKL Z LUPEM przez Wejscie!");
                finished = true;
                return;
            }
        }
    }
}

std::string Simulation::getLogText() const
{
    return eventLog->toString();
}

bool Simulation::isLoaded() const
{
    return loaded;
}

int Simulation::getStepNumber() const
{
    return stepNumber;
}

const std::vector<Room*>& Simulation::getRooms() const
{
    return building->getRooms();
}

const std::vector<Door*>& Simulation::getDoors() const
{
    return doors;
}

const std::vector<Person*>& Simulation::getPeople() const
{
    return people;
}

void Simulation::clearDynamicObjects()
{
    for (std::size_t i = 0; i < sensors.size(); ++i) delete sensors[i];
    sensors.clear();

    for (std::size_t i = 0; i < doors.size(); ++i) delete doors[i];
    doors.clear();

    for (std::size_t i = 0; i < people.size(); ++i) delete people[i];
    people.clear();

    building->clear();
    alarmSystem->resetAlarm();

    stepNumber = 0;
    loaded = false;
    finished = false;
}

void Simulation::createSensorsForAllRooms()
{
    const std::vector<Room*>& rooms = building->getRooms();
    for (std::size_t i = 0; i < rooms.size(); ++i)
    {
        Sensor* sensor = new Sensor(rooms[i], alarmSystem);
        sensors.push_back(sensor);
        rooms[i]->setSensor(sensor);
    }
}

bool Simulation::parseDoorsAndPeople(const std::string& fileName, std::string& errorMessage)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        errorMessage = "Nie mozna ponownie otworzyc pliku mapy: " + fileName;
        return false;
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line))
    {
        ++lineNumber;
        line = trim(line);

        if (line.empty() || line[0] == '#') continue;

        std::istringstream stream(line);
        std::string command;
        stream >> command;

        if (command == "ROOM") continue;

        if (command == "DOOR")
        {
            int firstId = 0, secondId = 0;
            stream >> firstId >> secondId;
            Room* firstRoom = building->findRoomById(firstId);
            Room* secondRoom = building->findRoomById(secondId);

            if (firstRoom == nullptr || secondRoom == nullptr)
            {
                errorMessage = "Drzwi DOOR wskazuja nieistniejacy pokoj w linii " + std::to_string(lineNumber);
                return false;
            }

            Door* door = new Door(firstRoom, secondRoom);
            doors.push_back(door);
            firstRoom->addDoor(door);
            secondRoom->addDoor(door);
            continue;
        }

        if (command == "CAMERA")
        {
            int roomId = 0;
            stream >> roomId;
            Room* room = building->findRoomById(roomId);
            if (room != nullptr) room->setCamera(true);
            continue;
        }

        if (command == "PERSON")
        {
            std::string kind, name;
            stream >> kind >> name;

            if (kind == "EMPLOYEE")
            {
                std::string rfidId, dummyRoom;
                stream >> rfidId >> dummyRoom;

                const std::vector<Room*>& allRooms = building->getRooms();
                int randomIndex = std::rand() % allRooms.size();
                Room* room = allRooms[randomIndex];

                if (room == nullptr || name.empty() || rfidId.empty())
                {
                    errorMessage = "Bledny pracownik w linii " + std::to_string(lineNumber);
                    return false;
                }
                people.push_back(new Employee(name, rfidId, room));
                continue;
            }

            if (kind == "INTRUDER")
            {
                int startRoomId = 0, targetRoomId = 0;
                stream >> startRoomId >> targetRoomId;

                Room* startRoom = building->findRoomById(1);
                Room* targetRoom = building->findRoomById(targetRoomId);

                if (startRoom == nullptr || targetRoom == nullptr || name.empty())
                {
                    errorMessage = "Bledny intruz w linii " + std::to_string(lineNumber);
                    return false;
                }
                people.push_back(new Intruder(name, startRoom, targetRoom));
                continue;
            }

            if (kind == "GUARD")
            {
                int roomId = 0;
                stream >> roomId;
                Room* room = building->findRoomById(roomId);
                if (room == nullptr || name.empty())
                {
                    errorMessage = "Bledny ochroniarz w linii " + std::to_string(lineNumber);
                    return false;
                }
                people.push_back(new Guard(name, room));
                continue;
            }

            errorMessage = "Nieznany typ osoby w linii " + std::to_string(lineNumber);
            return false;
        }
    }
    return true;
}

void Simulation::movePeople()
{
    bool alarm = alarmSystem->isAlarmActive();

    Room* intruderRoom = nullptr;
    Intruder* intruderObj = nullptr;
    for (std::size_t i = 0; i < people.size(); ++i)
    {
        if (people[i] != nullptr && people[i]->getKind() == "intruz")
        {
            intruderObj = static_cast<Intruder*>(people[i]);
            intruderRoom = intruderObj->getCurrentRoom();
            break;
        }
    }

    for (std::size_t i = 0; i < people.size(); ++i)
    {
        Person* person = people[i];
        if (person == nullptr || person->getCurrentRoom() == nullptr) continue;

        Room* currentRoom = person->getCurrentRoom();
        Room* globalTarget = nullptr;

        if (person->getKind() == "pracownik")
        {
            if (alarm)
            {
                globalTarget = building->findRoomById(1);
            }
            else
            {
                const std::vector<Door*>& roomDoors = currentRoom->getDoors();
                if (!roomDoors.empty())
                {
                    int r = std::rand() % roomDoors.size();
                    Door* chosenDoor = roomDoors[r];
                    if (!chosenDoor->isLocked())
                    {
                        Room* targetRoom = chosenDoor->getOtherRoom(currentRoom);
                        int oldId = currentRoom->getId();
                        person->move(targetRoom);
                        eventLog->add("[krok " + std::to_string(stepNumber) + "] (pracownik) " + person->getName()
                                      + " spaceruje z pokoju " + std::to_string(oldId) + " do " + std::to_string(targetRoom->getId()) + ".");
                    }
                }
                continue;
            }
        }
        else if (person->getKind() == "intruz")
        {
            if (!intruderObj->isMissionAccomplished())
            {
                globalTarget = intruderObj->getTargetRoom();
                if (currentRoom == globalTarget)
                {
                    intruderObj->setMissionAccomplished();
                    eventLog->add("[krok " + std::to_string(stepNumber) + "] INTRUZ: " + person->getName() + " UKRADL DANE Z CELU! Teraz ucieka do wyjscia.");
                    globalTarget = building->findRoomById(1);
                }
            }
            else
            {
                globalTarget = building->findRoomById(1);
            }
        }
        else if (person->getKind() == "ochroniarz")
        {
            if (alarm && intruderRoom != nullptr)
            {
                globalTarget = intruderRoom;
            }
        }

        if (globalTarget != nullptr && globalTarget != currentRoom)
        {
            Room* nextStep = nullptr;

            // SZTUCZNA INTELIGENCJA INTRUZA: Probuje ominac straznikow!
            if (person->getKind() == "intruz" && alarm)
            {
                nextStep = getNextRoomTowards(currentRoom, globalTarget, true); // tryb bezpieczny (bez strażników)
                if (nextStep == nullptr)
                {
                    nextStep = getNextRoomTowards(currentRoom, globalTarget, false); // desperacka ucieczka
                }
            }
            else
            {
                nextStep = getNextRoomTowards(currentRoom, globalTarget, false);
            }

            if (nextStep != nullptr)
            {
                bool canPass = true;
                if (person->getKind() == "intruz")
                {
                    const std::vector<Door*>& roomDoors = currentRoom->getDoors();
                    for (std::size_t d = 0; d < roomDoors.size(); ++d)
                    {
                        if (roomDoors[d]->getOtherRoom(currentRoom) == nextStep && roomDoors[d]->isLocked())
                        {
                            if (std::rand() % 100 < 40)
                            {
                                canPass = true;
                                eventLog->add("[krok " + std::to_string(stepNumber) + "] INTRUZ " + person->getName() + " zhakowal zamek i omija blokade do pokoju " + std::to_string(nextStep->getId()) + "!");
                            }
                            else
                            {
                                canPass = false;
                            }
                            break;
                        }
                    }
                }

                if (canPass)
                {
                    int oldId = currentRoom->getId();
                    person->move(nextStep);
                    eventLog->add("[krok " + std::to_string(stepNumber) + "] (" + person->getKind() + ") " + person->getName()
                                  + " przemieszcza sie z " + std::to_string(oldId) + " do " + std::to_string(nextStep->getId()) + ".");
                }
                else
                {
                    eventLog->add("[krok " + std::to_string(stepNumber) + "] Intruz " + person->getName() + " natrafil na ZABLOKOWANE drzwi do pokoju " + std::to_string(nextStep->getId()) + "!");
                }
            }
        }
    }
}

void Simulation::checkSensors()
{
    bool alarmBefore = alarmSystem->isAlarmActive();
    const std::vector<Room*>& rooms = building->getRooms();

    for (std::size_t i = 0; i < rooms.size(); ++i)
    {
        Room* r = rooms[i];
        if (r == nullptr) continue;

        Sensor* s = r->getSensor();
        if (s != nullptr) s->check(stepNumber);

        bool hasGuard = false;
        Person* localIntruder = nullptr;
        const std::vector<Person*>& peopleInRoom = r->getPeople();

        for (std::size_t p = 0; p < peopleInRoom.size(); ++p)
        {
            if (peopleInRoom[p]->getKind() == "ochroniarz") hasGuard = true;
            if (peopleInRoom[p]->getKind() == "intruz") localIntruder = peopleInRoom[p];
        }

        if (r->hasCamera() && localIntruder != nullptr && !alarmSystem->isAlarmActive())
        {
            if (std::rand() % 100 < 30)
            {
                eventLog->add("\n[krok " + std::to_string(stepNumber) + "] KAMERA w pokoju " + std::to_string(r->getId()) + " (" + r->getName() + ") rozpoznala intruza!");
                alarmSystem->triggerAlarm(r, localIntruder, stepNumber);
            }
        }

        if (hasGuard && localIntruder != nullptr && !alarmSystem->isAlarmActive())
        {
            eventLog->add("\n[krok " + std::to_string(stepNumber) + "] UWAGA! Ochroniarz przypadkowo wpadl na intruza w pokoju " + std::to_string(r->getId()) + "!");
            alarmSystem->triggerAlarm(r, localIntruder, stepNumber);
        }
    }

    if (!alarmBefore && alarmSystem->isAlarmActive())
    {
        eventLog->add("\n[krok " + std::to_string(stepNumber) + "] SYSTEM: Zarzadzono calkowita blokade wszystkich drzwi w budynku!");
        for (std::size_t i = 0; i < doors.size(); ++i)
        {
            if (doors[i] != nullptr) doors[i]->lock();
        }
    }
}

Room* Simulation::getNextRoomTowards(Room* start, Room* target, bool avoidGuards)
{
    if (start == nullptr || target == nullptr || start == target) return nullptr;

    std::queue<Room*> q;
    std::map<Room*, Room*> parent;
    std::vector<Room*> visited;

    q.push(start);
    visited.push_back(start);

    bool found = false;
    while (!q.empty())
    {
        Room* current = q.front();
        q.pop();

        if (current == target)
        {
            found = true;
            break;
        }

        std::vector<Door*> roomDoors = current->getDoors();

        for (std::size_t i = 0; i < roomDoors.size(); ++i) {
            std::size_t r = std::rand() % roomDoors.size();
            std::swap(roomDoors[i], roomDoors[r]);
        }

        for (std::size_t i = 0; i < roomDoors.size(); ++i)
        {
            Room* next = roomDoors[i]->getOtherRoom(current);

            // Jesli szukamy bezpiecznej drogi, sprawdzamy czy w nastepnym pokoju jest ochroniarz
            bool hasGuard = false;
            if (avoidGuards && next != target && next != nullptr)
            {
                const std::vector<Person*>& peeps = next->getPeople();
                for (std::size_t p = 0; p < peeps.size(); ++p)
                {
                    if (peeps[p]->getKind() == "ochroniarz")
                    {
                        hasGuard = true;
                        break;
                    }
                }
            }

            if (next != nullptr && !hasGuard && std::find(visited.begin(), visited.end(), next) == visited.end())
            {
                visited.push_back(next);
                parent[next] = current;
                q.push(next);
            }
        }
    }

    if (!found) return nullptr;

    Room* curr = target;
    while (parent[curr] != start)
    {
        parent[curr];
        curr = parent[curr];
    }
    return curr;
}