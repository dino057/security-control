#include "Intruder.h"
#include "Room.h"

Intruder::Intruder(const std::string& name, Room* startRoom, Room* targetRoom)
    : Person(name, startRoom),
    targetRoom(targetRoom),
    escapeRoom(startRoom),
    hasStolen(false) // Na start na pewno nie ma jeszcze łupu w kieszeni
{
}

bool Intruder::hasAccess()
{
    // To jest nasz system "Kamuflażu" - dopóki intruz nie dotknie łupu, udaje pracownika i system myśli, że jest czysty.
    if (!hasStolen)
    {
        return true;
    }

    // Kiedy intruz zwinie dane, hasStolen zmienia się na 'true'.
    // Od tego momentu za każdym razem na zapytanie "masz dostęp?" nasza klasa zaczyna zwracać fałsz,
    // co od razu odpala alarm.
    return false;
}

std::string Intruder::getKind() const
{
    // Czerwona flaga - nasza grafika z BuildingView po tym słowie wie,
    // żeby narysować złowrogą, czerwoną kropkę.
    return "intruz";
}

Room* Intruder::getTargetRoom() const
{
    return targetRoom;
}

Room* Intruder::getEscapeRoom() const
{
    return escapeRoom;
}

bool Intruder::isMissionAccomplished() const
{
    // Zwraca po prostu naszą zmienną. Jak prawda - uciekamy. Jak fałsz - szukamy łupu.
    return hasStolen;
}

void Intruder::setMissionAccomplished()
{
    // Kiedy Intruz wejdzie do pokoju z celem, ta intruz traci kamuflarz
    hasStolen = true;
}

void Intruder::setTargetRoom(Room* newTarget)
{
    //Metoda do naszej funkcji "Losowania łupu" przy restarcie gry
    targetRoom = newTarget;
}