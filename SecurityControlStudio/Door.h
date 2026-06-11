#ifndef DOOR_H
#define DOOR_H

class Room;

class Door
{
public:
    Door(Room* firstRoom, Room* secondRoom);

    Room* getFirstRoom() const;
    Room* getSecondRoom() const;
    Room* getOtherRoom(Room* room) const;

    bool isLocked() const;
    void lock();
    void unlock();

private:
    // Door nie tworzy pokojow i ich nie usuwa.
    // Przechowuje tylko adresy dwoch juz istniejacych pokojow.
    Room* firstRoom;
    Room* secondRoom;
    bool locked;
};

#endif // DOOR_H
