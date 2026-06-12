/**
 * @file Person.h
 * @brief Definicja abstrakcyjnej klasy bazowej Person.
 */

#ifndef PERSON_H
#define PERSON_H

#include <string>

class Room;

/**
 * @class Person
 * @brief Klasa bazowa reprezentujaca dowolna osobe w budynku.
 * * Jest to klasa abstrakcyjna. Zarzadza podstawowymi informacjami (imie, obecny pokoj)
 * oraz umozliwia fizyczne przemieszczanie sie miedzy pomieszczeniami.
 */
class Person
{
public:
    /**
     * @brief Konstruktor inicjalizujacy podstawowe dane osoby.
     * @param name Imie lub pseudonim.
     * @param startRoom Wskaznik na pokoj poczatkowy.
     */
    Person(const std::string& name, Room* startRoom);

    /**
     * @brief Wirtualny destruktor.
     * * Zapewnia poprawne usuwanie obiektow klas pochodnych.
     */
    virtual ~Person();

    /**
     * @brief Zwraca imie osoby.
     * @return Ciag znakow (std::string) z imieniem.
     */
    std::string getName() const;

    /**
     * @brief Zwraca aktualny pokoj, w ktorym znajduje sie osoba.
     * @return Wskaznik na obiekt Room.
     */
    Room* getCurrentRoom() const;

    /**
     * @brief Metoda czysto wirtualna sprawdzajaca autoryzacje postaci.
     * @return true/false w zaleznosci od implementacji w klasie pochodnej.
     */
    virtual bool hasAccess() = 0;

    /**
     * @brief Metoda czysto wirtualna zwracajaca typ postaci.
     * @return Nazwa roli (np. "pracownik", "intruz").
     */
    virtual std::string getKind() const = 0;

    /**
     * @brief Przemieszcza osobe z obecnego pokoju do nowego.
     * @param target Pokoj docelowy.
     */
    virtual void move(Room* target);

protected:
    std::string name;

    // Zapamiętany wskaźnik - wiemy, w jakim pokoju aktualnie stoimy.
    Room* currentRoom;
};

#endif // PERSON_H