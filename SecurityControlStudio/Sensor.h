#ifndef SENSOR_H
#define SENSOR_H

class AlarmSystem;
class Room;

class Sensor
{
public:
    Sensor(Room* room, AlarmSystem* alarmSystem);

    void check(int stepNumber);
    Room* getRoom() const;

private:
    // Czujnik zna pokoj, ktory obserwuje, oraz system alarmowy.
    // Nie jest wlascicielem ani pokoju, ani alarmu, dlatego nie robi delete.
    Room* room;
    AlarmSystem* alarmSystem;
};

#endif // SENSOR_H
