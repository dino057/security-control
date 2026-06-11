#include "Building.h"

#include "Room.h"

#include <fstream>
#include <sstream>

namespace
{
std::string trim(const std::string& text)
{
    std::size_t first = text.find_first_not_of(" \t\r\n");

    if (first == std::string::npos)
    {
        return "";
    }

    std::size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}
}

Building::Building()
{
}

Building::~Building()
{
    clear();
}

bool Building::loadFromFile(const std::string& fileName, std::string& errorMessage)
{
    clear();

    std::ifstream file(fileName);

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

        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        std::istringstream stream(line);
        std::string command;
        stream >> command;

        if (command == "ROOM")
        {
            int id = 0;
            stream >> id;

            std::string roomName;
            std::getline(stream, roomName);
            roomName = trim(roomName);

            if (id <= 0 || roomName.empty())
            {
                errorMessage = "Blad ROOM w linii " + std::to_string(lineNumber);
                clear();
                return false;
            }

            if (findRoomById(id) != nullptr)
            {
                errorMessage = "Powtorzone ID pokoju w linii " + std::to_string(lineNumber);
                clear();
                return false;
            }

            rooms.push_back(new Room(id, roomName));
        }
    }

    if (rooms.empty())
    {
        errorMessage = "Mapa nie zawiera zadnego pokoju ROOM.";
        return false;
    }

    return true;
}

void Building::clear()
{
    for (std::size_t i = 0; i < rooms.size(); ++i)
    {
        delete rooms[i];
    }

    rooms.clear();
}

Room* Building::findRoomById(int id) const
{
    for (std::size_t i = 0; i < rooms.size(); ++i)
    {
        if (rooms[i] != nullptr && rooms[i]->getId() == id)
        {
            return rooms[i];
        }
    }

    return nullptr;
}

const std::vector<Room*>& Building::getRooms() const
{
    return rooms;
}
