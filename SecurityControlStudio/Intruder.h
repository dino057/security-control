/**
 * @file Intruder.h
 * @brief Definicja klasy Intruder, reprezentujacej wlamywacza w symulacji.
 */

#ifndef INTRUDER_H
#define INTRUDER_H

#include "Person.h"
#include <string>

class Room;

/**
 * @class Intruder
 * @brief Reprezentuje intruza probujacego wykrasc dane z wyznaczonego pokoju.
 * * Klasa dziedziczy po abstrakcyjnej klasie Person. Posiada unikalna mechanike kamuflazu
 * (traci uprawnienia po dokonaniu kradziezy) oraz cel do osiagniecia.
 */
class Intruder : public Person
{
public:
    /**
     * @brief Konstruktor tworzacy intruza.
     * @param name Imie/pseudonim intruza.
     * @param startRoom Pokoj startowy (zazwyczaj Wejscie), z ktorego intruz bedzie tez uciekal.
     * @param targetRoom Pokoj docelowy, w ktorym ukryty jest lup.
     */
    Intruder(const std::string& name, Room* startRoom, Room* targetRoom);

    /**
     * @brief Sprawdza, czy intruz ma autoryzacje (dzialajacy kamuflaz).
     * @return true jesli jeszcze nie ukradl lupu, false jesli juz to zrobil (alarm!).
     */
    bool hasAccess() override;

    /**
     * @brief Zwraca typ postaci (jej zawod/role).
     * @return Ciag znakow "intruz".
     */
    std::string getKind() const override;

    /**
     * @brief Zwraca aktualny cel intruza (pokoj z lupem).
     * @return Wskaznik na docelowy pokoj.
     */
    Room* getTargetRoom() const;

    /**
     * @brief Zwraca pokoj, przez ktory intruz moze uciec z budynku.
     * @return Wskaznik na pokoj ewakuacyjny.
     */
    Room* getEscapeRoom() const;

    /**
     * @brief Pozwala na dynamiczna zmiane celu intruza (np. podczas losowania).
     * @param newTarget Wskaznik na nowy pokoj z lupem.
     */
    void setTargetRoom(Room* newTarget);

    /**
     * @brief Sprawdza status misji kradziezy.
     * @return true jesli lup zostal skradziony, false jesli intruz wciaz go szuka.
     */
    bool isMissionAccomplished() const;

    /**
     * @brief Zmienia status misji na wykonana, co wywoluje utrate kamuflazu.
     */
    void setMissionAccomplished();

private:
    Room* targetRoom;   // Pokój, do którego zmierzamy po łup
    Room* escapeRoom;   // Pokój ewakuacyjny (zazwyczaj ten sam, w którym zaczynamy)
    bool hasStolen;     // Flaga trzymająca status kamuflażu i kradzieży
};

#endif // INTRUDER_H