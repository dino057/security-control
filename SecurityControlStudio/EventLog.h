#ifndef EVENTLOG_H
#define EVENTLOG_H

#include <string>
#include <vector>

class EventLog
{
public:
    EventLog();

    void add(const std::string& message);
    void clear();
    std::string toString() const;

private:
    // vector to dynamiczna tablica z biblioteki standardowej.
    // Nie musimy jej zwalniac recznie, bo robi to sama w destruktorze.
    std::vector<std::string> entries;
};

#endif // EVENTLOG_H
