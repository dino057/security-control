#include "Guard.h"

// Podobnie jak u Pracownika - zanim powstanie Ochroniarz, musimy odpalić
// konstruktor klasy-matki (Person), żeby przypisała mu imię i pokój startowy.
Guard::Guard(const std::string& name, Room* startRoom)
    : Person(name, startRoom)
{

}

bool Guard::hasAccess()
{
    // Tutaj znów użyliśmy polimorfizmu i ochorna może wejsć wszędzie.
    return true;
}

std::string Guard::getKind() const
{
    // Zwracamy słowo "ochroniarz".
    // Dzięki temu w klasie BuildingView program wie, żeby narysować dla tej postaci NIEBIESKĄ kropkę.
    // Z kolei klasa Simulation używa tego słowa, żeby wiedzieć, kto ma włączyć pościg.
    return "ochroniarz";
}