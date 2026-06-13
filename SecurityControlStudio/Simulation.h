/**
 * @file Simulation.h
 * @brief Definicja klasy Simulation, glownego silnika napedzajacego logike gry.
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include <vector>

// Zapowiedzi klas.
class AlarmSystem;
class Building;
class Door;
class EventLog;
class Person;
class Room;
class Sensor;

/**
 * @class Simulation
 * @brief Glowny menedzer symulacji. Zarzadza czasem (krokami), postaciami i budynkiem.
 * * Klasa laczy wszystkie elementy w calosc. Odpowiada za wczytywanie mapy,
 * usuwanie dynamicznych obiektow z pamieci, oraz za glowna petle logiki gry (metoda step).
 */
class Simulation
{
public:
    /**
     * @brief Konstruktor. Przygotowuje puste systemy i logi.
     */
    Simulation();

    /**
     * @brief Destruktor. Zwalnia cala pamiec z przypisanych obiektow.
     */
    ~Simulation();

    /**
     * @brief Wczytuje mape, drzwi i postacie z pliku konfiguracyjnego.
     * @param fileName Sciezka do pliku z mapa (np. mapa_mala.map).
     * @param errorMessage Referencja do stringa, by zwrocic komunikat o bledzie.
     * @return true jesli wczytano sukcesem, false jesli wystapil blad.
     */
    bool loadFromFile(const std::string& fileName, std::string& errorMessage);

    /**
     * @brief Wykonuje pojedynczy krok symulacji (ruch postaci, czujniki, ucieczki).
     */
    void step();

    /**
     * @brief Pobiera cala historie wydarzen z dziennika.
     * @return Zlepiony tekst ze wszystkimi logami.
     */
    std::string getLogText() const;

    /**
     * @brief Sprawdza, czy mapa zostala juz pomyslnie wczytana.
     * @return true/false.
     */
    bool isLoaded() const;

    /**
     * @brief Sprawdza, czy symulacja dobiegla konca (wygrana/przegrana).
     * @return true jesli gra jest zakonczona.
     */
    bool isFinished() const;

    /**
     * @brief Pobiera aktualny numer kroku.
     * @return Liczba calkowita oznaczajaca ture.
     */
    int getStepNumber() const;

    /**
     * @brief Zwraca liste wszystkich pokojow.
     */
    const std::vector<Room*>& getRooms() const;

    /**
     * @brief Zwraca liste wszystkich drzwi.
     */
    const std::vector<Door*>& getDoors() const;

    /**
     * @brief Zwraca liste wszystkich zyjacych postaci w budynku.
     */
    const std::vector<Person*>& getPeople() const;

private:
    void clearDynamicObjects();
    void createSensorsForAllRooms();
    bool parseDoorsAndPeople(const std::string& fileName, std::string& errorMessage);
    void movePeople();
    void checkSensors();

    /**
     * @brief Zaawansowany algorytm (BFS) szukajacy najkrotszej drogi do celu.
     * @param start Pokoj poczatkowy.
     * @param target Pokoj docelowy.
     * @param avoidGuards Flaga (domyslnie false). Jesli true, algorytm unika pokojow z ochrona.
     * @return Wskaznik na kolejny pokoj, do ktorego trzeba wejsc, zeby zblizyc sie do celu.
     */
    Room* getNextRoomTowards(Room* start, Room* target, bool avoidGuards = false);

    // Glowne moduly systemu. Simulation jest wlascicielem eventLog i alarmSystem.
    EventLog* eventLog;
    Building* building;
    AlarmSystem* alarmSystem;

    // Simulation jest wlascicielem ludzi, drzwi i czujnikow (robi na nich 'delete').
    std::vector<Person*> people;
    std::vector<Door*> doors;
    std::vector<Sensor*> sensors;

    int stepNumber;
    bool loaded;
    bool finished;
};

#endif // SIMULATION_H