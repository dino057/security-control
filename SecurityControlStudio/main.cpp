/**
 * @file main.cpp
 * @brief Punkt wejscia do programu (miejsce, gdzie aplikacja zaczyna swoje dzialanie).
 */

#include "mainwindow.h"
#include <QApplication>

/**
 * @brief Glowna funkcja programu w C++.
 * @param argc Liczba argumentow uruchomieniowych przekazanych z konsoli.
 * @param argv Tablica tekstow z argumentami.
 * @return Kod bledu (0 oznacza, ze program zamknal sie poprawnie).
 */
int main(int argc, char* argv[])
{
    QApplication application(argc, argv);

    // Tworzymy obiekt naszego glownego okna.
    MainWindow window;

    // A następnie kazemy mu sie wyswietlic na ekranie.
    window.show();

    // application.exec() odpala nieskonczona petle.
    // Od tego momentu program tu "wisi" i caly czas czeka na to, az uzytkownik cos kliknie.
    // Skonczy dzialac dopiero wtedy, gdy klikniemy czerwony krzyzyk zamykajacy aplikacje.
    return application.exec();
}