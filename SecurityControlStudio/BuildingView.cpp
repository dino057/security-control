#include "BuildingView.h"
#include "Door.h"
#include "Person.h"
#include "Room.h"
#include "Simulation.h"
#include "Intruder.h" // POTRZEBNE DO SPRAWDZENIA STATUSU KRADZIEZY

#include <QPainter>
#include <QPaintEvent>
#include <cmath>
#include <vector>

BuildingView::BuildingView(QWidget* parent) : QWidget(parent), simulation(nullptr), animationProgress(1.0)
{
    setMinimumSize(520, 260);
    animationTimer.setInterval(30);
    connect(&animationTimer, &QTimer::timeout, this, &BuildingView::onAnimationTimer);
}

void BuildingView::setSimulation(Simulation* simulation)
{
    this->simulation = simulation;
    oldPersonRooms.clear();
    animationProgress = 1.0;
    animationTimer.stop();
    update();
}

void BuildingView::rememberPositionsBeforeStep()
{
    oldPersonRooms.clear();
    if (simulation == nullptr) return;
    const std::vector<Person*>& people = simulation->getPeople();
    for (std::size_t i = 0; i < people.size(); ++i)
    {
        if (people[i] != nullptr) oldPersonRooms.insert(people[i], people[i]->getCurrentRoom());
    }
}

void BuildingView::animateToCurrentPositions()
{
    if (oldPersonRooms.isEmpty()) { update(); return; }
    animationProgress = 0.0;
    animationTimer.start();
    update();
}

void BuildingView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QColor(245, 247, 250));

    if (simulation == nullptr || !simulation->isLoaded())
    {
        drawEmptyMessage(painter);
        return;
    }

    QMap<const Room*, QRectF> roomRects = calculateRoomRects();
    drawDoors(painter, roomRects);
    drawRooms(painter, roomRects);
    drawPeople(painter, roomRects);
    drawLegend(painter);
}

void BuildingView::onAnimationTimer()
{
    animationProgress += 0.08;
    if (animationProgress >= 1.0)
    {
        animationProgress = 1.0;
        animationTimer.stop();
        oldPersonRooms.clear();
    }
    update();
}

QMap<const Room*, QRectF> BuildingView::calculateRoomRects() const
{
    QMap<const Room*, QRectF> result;
    if (simulation == nullptr) return result;
    const std::vector<Room*>& rooms = simulation->getRooms();
    int count = static_cast<int>(rooms.size());
    if (count == 0) return result;

    int columns = static_cast<int>(std::ceil(std::sqrt(static_cast<double>(count))));
    int rows = static_cast<int>(std::ceil(static_cast<double>(count) / columns));

    int margin = 26;
    int topLegendSpace = 55; // Powiekszone miejsce na dwurzedowa legende!
    double availableWidth = width() - 2.0 * margin;
    double availableHeight = height() - 2.0 * margin - topLegendSpace;
    double cellWidth = availableWidth / columns;
    double cellHeight = availableHeight / rows;

    for (int i = 0; i < count; ++i)
    {
        int row = i / columns;
        int column = i % columns;
        double roomWidth = cellWidth * 0.72;
        double roomHeight = cellHeight * 0.62;
        double x = margin + column * cellWidth + (cellWidth - roomWidth) / 2.0;
        double y = margin + topLegendSpace + row * cellHeight + (cellHeight - roomHeight) / 2.0;
        result.insert(rooms[i], QRectF(x, y, roomWidth, roomHeight));
    }
    return result;
}

QPointF BuildingView::roomCenter(const Room* room, const QMap<const Room*, QRectF>& roomRects) const
{
    if (!roomRects.contains(room)) return QPointF();
    return roomRects.value(room).center();
}

void BuildingView::drawEmptyMessage(QPainter& painter)
{
    painter.setPen(QColor(80, 86, 97));
    painter.setFont(QFont("Arial", 13));
    painter.drawText(rect(), Qt::AlignCenter, "Kliknij Wybierz budynek, aby pokazac uklad budynku.");
}

void BuildingView::drawDoors(QPainter& painter, const QMap<const Room*, QRectF>& roomRects)
{
    const std::vector<Door*>& doors = simulation->getDoors();
    for (std::size_t i = 0; i < doors.size(); ++i)
    {
        Door* door = doors[i];
        if (door == nullptr) continue;
        QPointF first = roomCenter(door->getFirstRoom(), roomRects);
        QPointF second = roomCenter(door->getSecondRoom(), roomRects);
        QPen pen;
        if (door->isLocked()) pen = QPen(QColor(190, 42, 42), 5, Qt::DashLine);
        else pen = QPen(QColor(105, 113, 128), 4, Qt::SolidLine);
        painter.setPen(pen);
        painter.drawLine(first, second);
    }
}

void BuildingView::drawRooms(QPainter& painter, const QMap<const Room*, QRectF>& roomRects)
{
    const std::vector<Room*>& rooms = simulation->getRooms();
    for (std::size_t i = 0; i < rooms.size(); ++i)
    {
        Room* room = rooms[i];
        QRectF roomRect = roomRects.value(room);

        painter.setPen(QPen(QColor(49, 56, 70), 2));
        painter.setBrush(QColor(255, 255, 255));
        painter.drawRoundedRect(roomRect, 8, 8);

        painter.setPen(QColor(30, 35, 45));
        painter.setFont(QFont("Arial", 10, QFont::Bold));
        painter.drawText(roomRect.adjusted(8, 8, -8, -8),
                         Qt::AlignTop | Qt::AlignHCenter,
                         QString::number(room->getId()) + ". " + QString::fromStdString(room->getName()));

        if (room->hasCamera())
        {
            painter.setPen(QPen(QColor(255, 255, 255), 1));
            painter.setBrush(QColor(41, 128, 185));
            painter.drawEllipse(QRectF(roomRect.right() - 18, roomRect.top() + 6, 12, 12));
            painter.setFont(QFont("Arial", 7, QFont::Bold));
            painter.drawText(QRectF(roomRect.right() - 18, roomRect.top() + 6, 12, 12), Qt::AlignCenter, "K");
        }
    }
}

QPointF BuildingView::personPoint(const Room* room, int localIndex, const QMap<const Room*, QRectF>& roomRects) const
{
    QRectF roomRect = roomRects.value(room);
    QPointF center = roomRect.center();

    int column = localIndex % 4;
    int row = localIndex / 4;

    double dx = (column - 1.5) * 24.0;
    double dy = (row - 0.5) * 24.0;

    return QPointF(center.x() + dx, center.y() + dy);
}

void BuildingView::drawPeople(QPainter& painter, const QMap<const Room*, QRectF>& roomRects)
{
    const std::vector<Person*>& people = simulation->getPeople();
    QMap<const Room*, int> peopleInRoomCount;

    for (std::size_t i = 0; i < people.size(); ++i)
    {
        Person* person = people[i];
        if (person == nullptr || person->getCurrentRoom() == nullptr) continue;

        const Room* currentRoom = person->getCurrentRoom();
        int localIndex = peopleInRoomCount.value(currentRoom, 0);
        peopleInRoomCount[currentRoom] = localIndex + 1;

        QPointF targetPoint = personPoint(currentRoom, localIndex, roomRects);
        QPointF drawPoint = targetPoint;

        if (animationProgress < 1.0 && oldPersonRooms.contains(person))
        {
            const Room* oldRoom = oldPersonRooms.value(person);
            int oldLocalIndex = 0;
            QPointF startPoint = personPoint(oldRoom, oldLocalIndex, roomRects);

            drawPoint = QPointF(startPoint.x() + (targetPoint.x() - startPoint.x()) * animationProgress,
                                startPoint.y() + (targetPoint.y() - startPoint.y()) * animationProgress);
        }

        // --- ZARZADZANIE KOLORAMI I ZNACZNIKAMI ---
        QColor personColor;
        QString initial = QString::fromStdString(person->getName()).left(1).toUpper();
        bool isThiefWithLoot = false;

        if (person->getKind() == "intruz")
        {
            personColor = QColor(205, 48, 48); // ZAWSZE Czerwony intruz
            Intruder* intr = static_cast<Intruder*>(person);
            if (intr->isMissionAccomplished())
            {
                isThiefWithLoot = true;
                initial = "$"; // Zmienia inicjal na znak kradziezy
            }
        }
        else if (person->getKind() == "ochroniarz")
        {
            personColor = QColor(41, 128, 185); // Ochroniarz Niebieski
        }
        else
        {
            personColor = QColor(37, 138, 87); // Pracownik Zielony
        }

        if (isThiefWithLoot)
        {
            painter.setPen(QPen(QColor(241, 196, 15), 3)); // Zlota obwodka lupu
        }
        else
        {
            painter.setPen(QPen(QColor(255, 255, 255), 2)); // Zwykla biala obwodka
        }

        painter.setBrush(personColor);
        painter.drawEllipse(drawPoint, 12, 12);

        painter.setPen(QColor(255, 255, 255));
        painter.setFont(QFont("Arial", 9, QFont::Bold));
        painter.drawText(QRectF(drawPoint.x() - 12, drawPoint.y() - 12, 24, 24), Qt::AlignCenter, initial);
    }
}

void BuildingView::drawLegend(QPainter& painter)
{
    painter.setFont(QFont("Arial", 9));

    // WIERSZ 1
    int y1 = 12;
    // Pracownik
    painter.setPen(Qt::NoPen); painter.setBrush(QColor(37, 138, 87));
    painter.drawEllipse(QPointF(20, y1), 6, 6);
    painter.setPen(QColor(40, 45, 55)); painter.drawText(32, y1 + 4, "pracownik");

    // Ochroniarz
    painter.setPen(Qt::NoPen); painter.setBrush(QColor(41, 128, 185));
    painter.drawEllipse(QPointF(110, y1), 6, 6);
    painter.setPen(QColor(40, 45, 55)); painter.drawText(122, y1 + 4, "ochroniarz");

    // Intruz
    painter.setPen(Qt::NoPen); painter.setBrush(QColor(205, 48, 48));
    painter.drawEllipse(QPointF(200, y1), 6, 6);
    painter.setPen(QColor(40, 45, 55)); painter.drawText(212, y1 + 4, "intruz");

    // Intruz z lupem
    painter.setPen(QPen(QColor(241, 196, 15), 2)); painter.setBrush(QColor(205, 48, 48));
    painter.drawEllipse(QPointF(270, y1), 6, 6);
    painter.setPen(QColor(255, 255, 255)); painter.setFont(QFont("Arial", 7, QFont::Bold));
    painter.drawText(QRectF(264, y1 - 6, 12, 12), Qt::AlignCenter, "$");
    painter.setFont(QFont("Arial", 9));
    painter.setPen(QColor(40, 45, 55)); painter.drawText(282, y1 + 4, "z lupem");

    // WIERSZ 2
    int y2 = 30;
    // Kamera
    painter.setPen(QPen(QColor(255, 255, 255), 1)); painter.setBrush(QColor(41, 128, 185));
    painter.drawEllipse(QRectF(14, y2 - 6, 12, 12));
    painter.setFont(QFont("Arial", 7, QFont::Bold)); painter.drawText(QRectF(14, y2 - 6, 12, 12), Qt::AlignCenter, "K");
    painter.setFont(QFont("Arial", 9));
    painter.setPen(QColor(40, 45, 55)); painter.drawText(32, y2 + 4, "kamera");

    // Drzwi
    painter.setPen(QPen(QColor(190, 42, 42), 3, Qt::DashLine));
    painter.drawLine(110, y2, 140, y2);
    painter.setPen(QColor(40, 45, 55)); painter.drawText(148, y2 + 4, "drzwi zablokowane");
}