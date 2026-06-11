#ifndef BUILDING_H
#define BUILDING_H

#include <string>
#include <vector>

class Room;

class Building
{
public:
    Building();
    ~Building();

    bool loadFromFile(const std::string& fileName, std::string& errorMessage);
    void clear();

    Room* findRoomById(int id) const;
    const std::vector<Room*>& getRooms() const;

private:
    // Building jest wlascicielem pokojow, dlatego w destruktorze robi delete.
    std::vector<Room*> rooms;
};

#endif // BUILDING_H
