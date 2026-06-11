#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "Person.h"

class Employee : public Person
{
public:
    Employee(const std::string& name, const std::string& rfidId, Room* startRoom);

    bool hasAccess() override;
    std::string getKind() const override;
    std::string getRfidId() const;

private:
    std::string rfidId;
};

#endif // EMPLOYEE_H
