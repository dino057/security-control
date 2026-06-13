/**
 * @file Door.h
 * @brief Definicja klasy Door, reprezentujacej polaczenie miedzy dwoma pokojami.
 */

#ifndef DOOR_H
#define DOOR_H

// Forward declaration
class Room;

/**
 * @class Door
 * @brief Reprezentuje fizyczne drzwi laczace dwa pokoje w budynku.
 * * Drzwi moga zostac zablokowane (np. podczas alarmu), co uniemozliwia
 * standardowe przejscie przez nie.
 */
class Door
{
public:
    /**
     * @brief Konstruktor tworzacy drzwi pomiedzy dwoma pokojami.
     * @param firstRoom Wskaznik na pierwszy pokoj.
     * @param secondRoom Wskaznik na drugi pokoj.
     */
    Door(Room* firstRoom, Room* secondRoom);

    /**
     * @brief Zwraca wskaznik na pierwszy z pokojow.
     * @return Wskaznik na Room.
     */
    Room* getFirstRoom() const;

    /**
     * @brief Zwraca wskaznik na drugi z pokojow.
     * @return Wskaznik na Room.
     */
    Room* getSecondRoom() const;

    /**
     * @brief Zwraca sasiadujacy pokoj po drugiej stronie drzwi.
     * @param room Pokoj, w ktorym obecnie sie znajdujemy.
     * @return Wskaznik na pokoj po drugiej stronie, lub nullptr jesli podany pokoj nie laczy sie z tymi drzwiami.
     */
    Room* getOtherRoom(Room* room) const;

    /**
     * @brief Sprawdza, czy drzwi sa obecnie zablokowane.
     * @return true jesli zamek jest zablokowany, false jesli mozna przejsc.
     */
    bool isLocked() const;

    /**
     * @brief Blokuje zamek w drzwiach (uruchamiane np. przez system alarmowy).
     */
    void lock();

    /**
     * @brief Odblokowuje zamek w drzwiach.
     */
    void unlock();

private:
    // Door przechowuje tylko ADRESY pokojow.
    Room* firstRoom;
    Room* secondRoom;

    bool locked; // Flaga trzymająca stan zamka (zamknięte/otwarte)
};

#endif // DOOR_H