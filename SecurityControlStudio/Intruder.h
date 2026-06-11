#ifndef INTRUDER_H
#define INTRUDER_H

#include "Person.h"

class Intruder : public Person
{
public:
    Intruder(const std::string& name, Room* startRoom, Room* targetRoom);

    bool hasAccess() override;
    std::string getKind() const override;

    Room* getTargetRoom() const;
    bool isMissionAccomplished() const;
    void setMissionAccomplished();

private:
    Room* targetRoom;
    bool hasStolen;
};

#endif // INTRUDER_H