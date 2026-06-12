/**
 * @file BuildingView.h
 * @brief Definicja klasy widoku, odpowiadajacej za graficzne wyswietlanie symulacji.
 */

#ifndef BUILDINGVIEW_H
#define BUILDINGVIEW_H

#include <QMap>
#include <QPointF>
#include <QRectF>
#include <QTimer>
#include <QWidget>

// Forward declaration
class QPainter;
class Person;
class Room;
class Simulation;

/**
 * @class BuildingView
 * @brief Klasa dziedziczaca po QWidget, sluzaca do rysowania budynku.
 * * Odpowiada za wyswietlanie pokojow, drzwi, postaci (kropek) oraz ich animacji.
 */
class BuildingView : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor widoku budynku.
     * @param parent Wskaznik na widzet-rodzica (domyslnie nullptr).
     */
    explicit BuildingView(QWidget* parent = nullptr);

    /**
     * @brief Podlacza obiekt symulacji pod ten widok graficzny.
     * @param simulation Wskaznik na aktualna symulacje.
     */
    void setSimulation(Simulation* simulation);

    /**
     * @brief Zapisuje w pamieci pozycje (pokoje) postaci przed wykonaniem kroku.
     * * Dzieki temu mozemy plynnie zaanimowac ich ruch do nowych pokojow.
     */
    void rememberPositionsBeforeStep();

    /**
     * @brief Rozpoczyna plynna animacje ruchu od starych do nowych pozycji.
     */
    void animateToCurrentPositions();

protected:
    /**
     * @brief Glowna metoda rysujaca Qt. Wywolywana automatycznie, gdy okno musi sie odswiezyc.
     * @param event Informacje o zdarzeniu rysowania.
     */
    void paintEvent(QPaintEvent* event) override;

private slots:
    // Slot to specjalna funkcja w Qt, ktora moze byc wywolana przez jakies zdarzenie (tu: tykniecie zegara).
    void onAnimationTimer();

private:
    // Prywatne metody pomocnicze do przeliczania pozycji i rysowania
    QMap<const Room*, QRectF> calculateRoomRects() const;
    QPointF roomCenter(const Room* room, const QMap<const Room*, QRectF>& roomRects) const;
    QPointF personPoint(const Room* room,
                        int personIndex,
                        const QMap<const Room*, QRectF>& roomRects) const;

    void drawEmptyMessage(QPainter& painter);
    void drawDoors(QPainter& painter, const QMap<const Room*, QRectF>& roomRects);
    void drawRooms(QPainter& painter, const QMap<const Room*, QRectF>& roomRects);
    void drawPeople(QPainter& painter, const QMap<const Room*, QRectF>& roomRects);
    void drawLegend(QPainter& painter);

    Simulation* simulation; // Wskaznik na mozgi naszej operacji (logike symulacji)

    // Mapa pamietajaca, gdzie kto stal przed kliknieciem "Start" (potrzebne do animacji)
    QMap<const Person*, const Room*> oldPersonRooms;

    QTimer animationTimer;     // Zegar odliczajacy klatki animacji
    double animationProgress;  // Postep animacji (od 0.0 do 1.0)
};

#endif // BUILDINGVIEW_H