#ifndef BUILDINGVIEW_H
#define BUILDINGVIEW_H

#include <QMap>
#include <QPointF>
#include <QRectF>
#include <QTimer>
#include <QWidget>

class QPainter;
class Person;
class Room;
class Simulation;

class BuildingView : public QWidget
{
    Q_OBJECT

public:
    explicit BuildingView(QWidget* parent = nullptr);

    void setSimulation(Simulation* simulation);
    void rememberPositionsBeforeStep();
    void animateToCurrentPositions();

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onAnimationTimer();

private:
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

    // Ten wskaznik nie jest wlascicielem symulacji.
    // MainWindow tworzy i usuwa Simulation, a BuildingView tylko ja oglada.
    Simulation* simulation;

    // Zapamietujemy pokoje sprzed klikniecia Krok, zeby kropki osob
    // mogly plynnie przejsc do nowych pokojow.
    QMap<const Person*, const Room*> oldPersonRooms;

    QTimer animationTimer;
    double animationProgress;
};

#endif // BUILDINGVIEW_H
