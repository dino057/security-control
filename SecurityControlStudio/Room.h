/**
 * @file Room.h
 * @brief Definicja klasy Room, reprezentujacej pojedyncze pomieszczenie w budynku.
 */

#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>

// Forward declaration
class Door;
class Person;
class Sensor;

/**
 * @class Room
 * @brief Klasa reprezentujaca pokoj, przechowujaca informacje o tym, kto i co w nim jest.
 * * Pokoj wie, jakie ma drzwi, kto aktualnie w nim stoi oraz czy ma zamontowane
 * systemy bezpieczenstwa (kamera, czujnik).
 */
class Room
{
public:
    /**
     * @brief Konstruktor tworzacy nowy pokoj.
     * @param id Unikalny numer pokoju (np. 1).
     * @param name Nazwa pokoju (np. "Serwerownia").
     */
    Room(int id, const std::string& name);

    /**
     * @brief Destruktor pokoju.
     */
    ~Room();

    /**
     * @brief Zwraca unikalny numer (ID) pokoju.
     * @return Liczba calkowita reprezentujaca ID.
     */
    int getId() const;

    /**
     * @brief Zwraca nazwe pokoju.
     * @return Ciag znakow (std::string) z nazwa.
     */
    std::string getName() const;

    /**
     * @brief Przypisuje drzwi do tego pokoju.
     * @param door Wskaznik na drzwi, ktore maja byc polaczone z pokojem.
     */
    void addDoor(Door* door);

    /**
     * @brief Dodaje osobe do listy obecnych w pokoju.
     * @param person Wskaznik na wchodzaca osobe.
     */
    void addPerson(Person* person);

    /**
     * @brief Usuwa osobe z listy obecnych (gdy z niego wychodzi).
     * @param person Wskaznik na wychodzaca osobe.
     */
    void removePerson(Person* person);

    /**
     * @brief Zwraca liste wszystkich drzwi nalezacych do tego pokoju.
     * @return Stala referencja do wektora wskaznikow na drzwi.
     */
    const std::vector<Door*>& getDoors() const;

    /**
     * @brief Zwraca liste wszystkich osob aktualnie przebywajacych w pokoju.
     * @return Stala referencja do wektora wskaznikow na osoby.
     */
    const std::vector<Person*>& getPeople() const;

    /**
     * @brief Montuje czujnik w pokoju.
     * @param sensor Wskaznik na instalowany czujnik.
     */
    void setSensor(Sensor* sensor);

    /**
     * @brief Zwraca czujnik przypisany do tego pokoju.
     * @return Wskaznik na obiekt Sensor.
     */
    Sensor* getSensor() const;

    /**
     * @brief Ustawia, czy pokoj ma miec zainstalowana kamere.
     * @param value true jesli montujemy kamere, false jesli demontujemy.
     */
    void setCamera(bool value);

    /**
     * @brief Sprawdza, czy w pokoju dziala kamera.
     * @return true jesli kamera jest obecna.
     */
    bool hasCamera() const;

private:
    int id;
    std::string name;
    bool camera;

    // Trzymamy tutaj tylko wskaźniki (adresy w pamięci).
    // Pokój nie tworzy ludzi ani drzwi, on tylko robi za "kontener" na ich adresy.
    // Dlatego przy niszczeniu pokoju nie będziemy robić na nich "delete".
    std::vector<Door*> doors;
    std::vector<Person*> people;
    Sensor* sensor;
};

#endif // ROOM_H