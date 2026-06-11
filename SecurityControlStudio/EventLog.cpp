#include "EventLog.h"

#include <sstream>

EventLog::EventLog()
{
}

void EventLog::add(const std::string& message)
{
    entries.push_back(message);
}

void EventLog::clear()
{
    entries.clear();
}

std::string EventLog::toString() const
{
    std::ostringstream stream;

    for (std::size_t i = 0; i < entries.size(); ++i)
    {
        stream << entries[i] << '\n';
    }

    return stream.str();
}
