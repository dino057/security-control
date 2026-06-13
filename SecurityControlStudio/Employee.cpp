#include "Employee.h"

// Dziedziczenie po klasie Person
Employee::Employee(const std::string& name, const std::string& rfidId, Room* startRoom)
    : Person(name, startRoom),
    rfidId(rfidId)
{

}

bool Employee::hasAccess()
{
    // Ponieważ to jest uczciwy pracownik, który ma kartę RFID (nie to co Intruz!),
    // to za każdym razem, gdy czujnik zapyta go o dostęp, on odpowiada "Prawda".
    // Tutaj mamy przykład zastosowania polimorfizmu.
    return true;
}

std::string Employee::getKind() const
{
    // Funkcja pomocnicza, żeby nasz silnik symulacji (Simulation.cpp) i interfejs (BuildingView.cpp)
    // wiedział, czy ma do czynienia z zielonym pracownikiem, czy z niebieskim strażnikiem.
    return "pracownik";
}

std::string Employee::getRfidId() const
{
    // Zwykły getter, zwraca po prostu to, co pracownik ma ukryte w prywatnej zmiennej.
    return rfidId;
}