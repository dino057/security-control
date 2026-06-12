/**
 * @file Employee.h
 * @brief Definicja klasy Employee, reprezentujacej zwyklego pracownika z dostepem.
 */

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "Person.h"

/**
 * @class Employee
 * @brief Reprezentuje autoryzowanego pracownika przebywajacego w budynku.
 * * Klasa dziedziczy po abstrakcyjnej klasie Person. Pracownik posiada
 * identyfikator RFID, ktory zapewnia mu staly, legalny dostep do pomieszczen.
 */
class Employee : public Person
{
public:
    /**
     * @brief Konstruktor tworzacy nowego pracownika.
     * @param name Imie i nazwisko (lub pseudonim) pracownika.
     * @param rfidId Unikalny numer identyfikatora zblizeniowego (karty RFID).
     * @param startRoom Wskaznik na pokoj, w ktorym pracownik zaczyna swoja zmiane.
     */
    Employee(const std::string& name, const std::string& rfidId, Room* startRoom);

    /**
     * @brief Sprawdza, czy pracownik ma prawo przebywac w budynku.
     * @return Zawsze true, poniewaz pracownik posiada legalna karte RFID.
     */
    bool hasAccess() override;

    /**
     * @brief Zwraca typ postaci (jej zawod/role).
     * @return Ciag znakow "pracownik".
     */
    std::string getKind() const override;

    /**
     * @brief Pobiera numer karty RFID pracownika.
     * @return Ciag znakow reprezentujacy numer karty.
     */
    std::string getRfidId() const;

private:
    std::string rfidId; // Indywidualna karta dostępowa pracownika
};

#endif // EMPLOYEE_H