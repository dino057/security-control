/**
 * @file EventLog.h
 * @brief Definicja klasy EventLog, sluzacej do gromadzenia logow (historii) symulacji.
 */

#ifndef EVENTLOG_H
#define EVENTLOG_H

#include <string>
#include <vector>

/**
 * @class EventLog
 * @brief Dziennik zdarzen przechowujacy komunikaty tekstowe.
 * * Klasa zbiera informacje o wszystkich akcjach w budynku (ruchy postaci,
 * wlaczenie alarmow, zhakowanie drzwi) i pozwala na ich seryjne odczytanie.
 */
class EventLog
{
public:
    /**
     * @brief Konstruktor domyslny. Tworzy pusty dziennik zdarzen.
     */
    EventLog();

    /**
     * @brief Dodaje nowa wiadomosc na koniec dziennika.
     * @param message Tresc komunikatu do zapisania.
     */
    void add(const std::string& message);

    /**
     * @brief Czysci cala historie zdarzen (uzywane przy restarcie symulacji).
     */
    void clear();

    /**
     * @brief Zwraca wszystkie zapisane komunikaty polaczone w jeden dlugi tekst.
     * @return Ciag znakow (std::string) zawierajacy cala historie, przedzielona enterami.
     */
    std::string toString() const;

private:
    std::vector<std::string> entries;
};

#endif // EVENTLOG_H