#include "Employee.h"

Employee::Employee(const std::string& name, const std::string& rfidId, Room* startRoom)
    : Person(name, startRoom),
      rfidId(rfidId)
{
}

bool Employee::hasAccess()
{
    // Pracownik ma karte RFID, wiec w tej prostej symulacji zawsze ma dostep.
    return true;
}

std::string Employee::getKind() const
{
    return "pracownik";
}

std::string Employee::getRfidId() const
{
    return rfidId;
}
