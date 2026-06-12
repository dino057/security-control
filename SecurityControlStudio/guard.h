/**
 * @file Guard.h
 * @brief Definicja klasy Guard, reprezentujacej ochroniarza w budynku.
 */

#ifndef GUARD_H
#define GUARD_H

#include "Person.h"

/**
 * @class Guard
 * @brief Reprezentuje straznika patrolujacego budynek.
 * * Klasa dziedziczy po abstrakcyjnej klasie Person. Ochroniarze maja
 * specjalne zachowanie w symulacji - gonia intruzow po wlaczeniu alarmu.
 */
class Guard : public Person
{
public:
    /**
     * @brief Konstruktor tworzacy nowego ochroniarza.
     * @param name Imie i nazwisko (lub pseudonim) straznika.
     * @param startRoom Wskaznik na pokoj, w ktorym straznik zaczyna swoj patrol.
     */
    Guard(const std::string& name, Room* startRoom);

    /**
     * @brief Sprawdza, czy straznik ma prawo przebywac w budynku.
     * @return Zawsze true, poniewaz ochrona ma pelny autoryzowany dostep.
     */
    bool hasAccess() override;

    /**
     * @brief Zwraca typ postaci (jej zawod/role).
     * @return Ciag znakow "ochroniarz".
     */
    std::string getKind() const override;
};

#endif // GUARD_H