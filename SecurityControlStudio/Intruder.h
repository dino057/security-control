#ifndef INTRUDER_H
#define INTRUDER_H

#include "Person.h"
#include <string>

class Room;

class Intruder : public Person
{
public:
    Intruder(const std::string& name, Room* startRoom, Room* targetRoom);

    // Metody nadpisujące z klasy Person
    bool hasAccess() override;
    std::string getKind() const override;

    // Metody specyficzne dla Intrudera
    Room* getTargetRoom() const;
    Room* getEscapeRoom() const;
    void setTargetRoom(Room* newTarget);
    bool isMissionAccomplished() const;
    void setMissionAccomplished();

private:
    Room* targetRoom;
    Room* escapeRoom; // DODAJ TĘ LINIĘ
    bool hasStolen;
};

#endif // INTRUDER_H