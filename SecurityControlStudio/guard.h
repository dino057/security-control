#ifndef GUARD_H
#define GUARD_H

#include "Person.h"

class Guard : public Person
{
public:
    Guard(const std::string& name, Room* startRoom);

    bool hasAccess() override;
    std::string getKind() const override;
};

#endif // GUARD_H