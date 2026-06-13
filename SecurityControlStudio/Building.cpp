#include "Building.h"
#include "Room.h"

#include <fstream>
#include <sstream>

namespace
{
/**
     * @brief Usuwa biale znaki (spacje, taby, entery) z poczatku i konca tekstu.
     */
std::string trim(const std::string& text)
{
    // Szukamy pierwszego znaku, który NIE jest spacją, tabulatorem ani enterem
    std::size_t first = text.find_first_not_of(" \t\r\n");

    // Jeśli text.npos to znaczy, że "nie znaleziono" - czyli cały tekst to same spacje
    if (first == std::string::npos)
    {
        return "";
    }

    // Szukamy ostatniego "normalnego" znaku
    std::size_t last = text.find_last_not_of(" \t\r\n");

    // Wycinamy tylko to, co jest w środku
    return text.substr(first, last - first + 1);
}
}

Building::Building()
{
}

Building::~Building()
{
    // Destruktor wywoływany jest, gdy program się zamyka.
    // Wywołujemy naszą metodę clear(), żeby posprzątać pamięć (unikamy Memory Leaks).
    clear();
}

bool Building::loadFromFile(const std::string& fileName, std::string& errorMessage)
{
    clear(); // Na wszelki wypadek czyścimy stary budynek, gdybyśmy wczytywali nową mapę w trakcie gry

    std::ifstream file(fileName); // Otwieramy plik do czytania

    if (!file.is_open())
    {
        errorMessage = "Nie mozna otworzyc pliku mapy: " + fileName;
        return false;
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line))
    {
        ++lineNumber;
        line = trim(line);

        // Ignorujemy puste linijki i nasze komentarze zaczynające się od '#'
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        std::istringstream stream(line);
        std::string command;
        stream >> command; // Wyciągamy pierwsze słowo (np. "ROOM", "DOOR", "CAMERA")

        if (command == "ROOM")
        {
            int id = 0;
            stream >> id; // Wyciągamy drugie słowo jako liczbę (ID pokoju)

            std::string roomName;
            std::getline(stream, roomName); // Reszta tej linijki to już nazwa pokoju (np. "Biura Zarzadu")
            roomName = trim(roomName);

            // Zabezpieczenia, gdyby ktoś w pliku mapy napisał głupoty
            if (id <= 0 || roomName.empty())
            {
                errorMessage = "Blad ROOM w linii " + std::to_string(lineNumber);
                clear();
                return false;
            }

            // Sprawdzamy czy pokoju o takim ID już przypadkiem nie wczytaliśmy
            if (findRoomById(id) != nullptr)
            {
                errorMessage = "Powtorzone ID pokoju w linii " + std::to_string(lineNumber);
                clear();
                return false;
            }

            // TUTAJ TWORZYMY OBIEKT: używamy słówka 'new', czyli alokujemy go w pamięci operacyjnej komputera.
            rooms.push_back(new Room(id, roomName));
        }
    }

    // Jak przeszliśmy cały plik, a nie ma ani jednego pokoju, to mapa jest zepsuta
    if (rooms.empty())
    {
        errorMessage = "Mapa nie zawiera zadnego pokoju ROOM.";
        return false;
    }

    return true;
}

void Building::clear()
{
    // Pętla usuwa same OBIEKTY pokojów z pamięci komputera.
    for (std::size_t i = 0; i < rooms.size(); ++i)
    {
        delete rooms[i];
    }

    // A to usuwa tylko "przegródki" na wskaźniki z naszego wektora, żeby był znowu pusty (size = 0).
    rooms.clear();
}

Room* Building::findRoomById(int id) const
{
    // Pętla szukająca - leci przez wszystkie pokoje i jak znajdzie pasujące ID, to je zwraca.
    for (std::size_t i = 0; i < rooms.size(); ++i)
    {
        if (rooms[i] != nullptr && rooms[i]->getId() == id)
        {
            return rooms[i];
        }
    }

    return nullptr; // Jak nie znalazł, zwraca pustkę (nullptr)
}

const std::vector<Room*>& Building::getRooms() const
{
    return rooms;
}