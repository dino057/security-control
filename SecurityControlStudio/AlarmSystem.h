/**
 * @file AlarmSystem.h
 * @brief Definicja klasy systemu alarmowego, odpowiedzialnego za reagowanie na zagrozenia.
 */

#ifndef ALARMSYSTEM_H
#define ALARMSYSTEM_H

// Poniżej to tzw. "Forward declarations" (deklaracje zapowiadające).
// Zamiast dołączać całe pliki #include "EventLog.h" itd., tylko obiecujemy kompilatorowi, że takie klasy istnieją.
class EventLog;
class Person;
class Room;

/**
 * @class AlarmSystem
 * @brief Zarzadza globalnym stanem alarmu w budynku.
 * * Klasa przechowuje informacje o tym, czy alarm jest w danym momencie aktywny,
 * oraz umozliwia jego wyzwolenie i zresetowanie.
 */
class AlarmSystem
{
public:
    /**
     * @brief Konstruktor tworzacy system alarmowy.
     * @param log Wskaznik na glowny dziennik zdarzen (EventLog), do ktorego wpisywane beda alarmy.
     */
    explicit AlarmSystem(EventLog* log);

    /**
     * @brief Uruchamia alarm w budynku.
     * @param room Pokoj, w ktorym wykryto zagrozenie.
     * @param person Osoba (intruz), ktora wywolala ten alarm.
     * @param stepNumber Aktualny krok symulacji (aby wpisac go do logu).
     */
    void triggerAlarm(Room* room, Person* person, int stepNumber);

    /**
     * @brief Sprawdza, czy alarm jest obecnie wlaczony.
     * @return true jesli alarm trwa, false jesli jest spokoj.
     */
    bool isAlarmActive() const;

    /**
     * @brief Wylacza alarm (wykorzystywane przy restarcie symulacji).
     */
    void resetAlarm();

private:
    EventLog* log;      // Zapamiętany wskaźnik na dziennik, żeby system alarmowy wiedział, gdzie ma pisać.
    bool alarmActive;   // Flaga "prawda/fałsz", która mówi całej symulacji czy drzwi mają się blokować.
};

#endif // ALARMSYSTEM_H