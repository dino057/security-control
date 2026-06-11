#include "Guard.h"

Guard::Guard(const std::string& name, Room* startRoom)
    : Person(name, startRoom)
{
}

bool Guard::hasAccess()
{
    return true; // Ochroniarz zawsze ma pełny dostęp
}

std::string Guard::getKind() const
{
    return "ochroniarz";
}