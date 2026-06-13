/**
 * @file Sensor.h
 * @brief Definicja klasy Sensor, reprezentujacej czujnik obecnosci/autoryzacji w pokoju.
 */

#ifndef SENSOR_H
#define SENSOR_H

// Forward declaration
class AlarmSystem;
class Room;

/**
 * @class Sensor
 * @brief Skanuje pokoj i weryfikuje uprawnienia przebywajacych w nim osob.
 * * Czujnik jest przypisany do konkretnego pokoju i podlaczony do glownego
 * systemu alarmowego, ktory powiadamia w razie wykrycia intruza.
 */
class Sensor
{
public:
    /**
     * @brief Konstruktor tworzacy nowy czujnik.
     * @param room Wskaznik na pokoj, w ktorym montowany jest czujnik.
     * @param alarmSystem Wskaznik na glowny system alarmowy budynku.
     */
    Sensor(Room* room, AlarmSystem* alarmSystem);

    /**
     * @brief Skanuje wszystkie osoby w pokoju pod katem autoryzacji.
     * @param stepNumber Aktualny numer kroku symulacji (do logow).
     */
    void check(int stepNumber);

    /**
     * @brief Zwraca pokoj, w ktorym zamontowany jest czujnik.
     * @return Wskaznik na obiekt Room.
     */
    Room* getRoom() const;

private:
    // Czujnik tylko "obserwuje" pokoj i "komunikuje sie" z systemem alarmowym.
    // Poniewaz nie tworzy ich sam, nie musi (i nie moze) robic na nich delete w destruktorze.
    Room* room;
    AlarmSystem* alarmSystem;
};

#endif // SENSOR_H