/**
 * @file Building.h
 * @brief Definicja klasy Building, reprezentujacej budynek (zbior pokojow).
 */

#ifndef BUILDING_H
#define BUILDING_H

#include <string>
#include <vector>

// Forward declaration
class Room;

/**
 * @class Building
 * @brief Zarzadza budynkiem, wczytuje pokoje z pliku i zwalnia ich pamiec.
 * * Klasa pełni role tzw. "wlasciciela" (owner) obiektow Room.
 */
class Building
{
public:
    /**
     * @brief Konstruktor domyslny.
     */
    Building();

    /**
     * @brief Destruktor. Zwalnia pamiec zajmowana przez pokoje.
     */
    ~Building();

    /**
     * @brief Wczytuje uklad pokojow z podanego pliku konfiguracyjnego (.map).
     * @param fileName Nazwa pliku do wczytania.
     * @param errorMessage Referencja do stringa, do ktorego zostanie wpisany ewentualny blad.
     * @return true jesli wczytano pomyslnie, false w razie bledu.
     */
    bool loadFromFile(const std::string& fileName, std::string& errorMessage);

    /**
     * @brief Usuwa wszystkie pokoje i czysci budynek.
     */
    void clear();

    /**
     * @brief Wyszukuje pokoj po jego unikalnym numerze ID.
     * @param id Numer ID pokoju (np. 1 dla Wejscia).
     * @return Wskaznik na znaleziony pokoj lub nullptr, jesli taki nie istnieje.
     */
    Room* findRoomById(int id) const;

    /**
     * @brief Zwraca liste wszystkich pokojow w budynku.
     * @return Stala referencja do wektora wskaznikow na pokoje.
     */
    const std::vector<Room*>& getRooms() const;

private:
    // Ten wektor trzyma wskaźniki (adresy) do pokojów.
    // Skoro Building tworzy te pokoje za pomocą operatora 'new' (w loadFromFile),
    // to musi je potem usunąć za pomocą 'delete' (w metodzie clear/destruktorze).
    std::vector<Room*> rooms;
};

#endif // BUILDING_H