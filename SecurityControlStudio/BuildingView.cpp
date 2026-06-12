#include "BuildingView.h"
#include "Door.h"
#include "Person.h"
#include "Room.h"
#include "Simulation.h"
#include "Intruder.h"

#include <QPainter>
#include <QPaintEvent>
#include <cmath>
#include <vector>

BuildingView::BuildingView(QWidget* parent) : QWidget(parent), simulation(nullptr), animationProgress(1.0)
{
    setMinimumSize(520, 260); // Zabezpieczamy okno, żeby nikt go za bardzo nie ścisnął

    // Ustawiamy zegar, który będzie tykał co 30 milisekund i pchał animację do przodu.
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

    // Przed każdym krokiem logiki w Simulation.cpp, zapisujemy "zdjęcie", w jakim pokoju kto stoi.
    const std::vector<Person*>& people = simulation->getPeople();
    for (std::size_t i = 0; i < people.size(); ++i)
    {
        if (people[i] != nullptr) oldPersonRooms.insert(people[i], people[i]->getCurrentRoom());
    }
}

void BuildingView::animateToCurrentPositions()
{
    if (oldPersonRooms.isEmpty()) { update(); return; }

    // Resetujemy postęp animacji na 0% i odpalamy zegar.
    animationProgress = 0.0;
    animationTimer.start();
    update();
}

void BuildingView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event); // Mówimy kompilatorowi, żeby nie narzekał, że nie używamy zmiennej 'event'

    // QPainter to nasz wirtualny pędzel w Qt. Przekazujemy 'this', żeby rysował po naszym oknie.
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // Włącza wygładzanie krawędzi (żeby kółka nie były kanciaste)
    painter.fillRect(rect(), QColor(245, 247, 250)); // Malujemy tło okna

    if (simulation == nullptr || !simulation->isLoaded())
    {
        drawEmptyMessage(painter);
        return;
    }

    // Wyliczamy gdzie leżą pokoje i rysujemy wszystko warstwa po warstwie
    QMap<const Room*, QRectF> roomRects = calculateRoomRects();
    drawDoors(painter, roomRects);
    drawRooms(painter, roomRects);
    drawPeople(painter, roomRects);
    drawLegend(painter);
}

void BuildingView::onAnimationTimer()
{
    // Co 30 milisekund przesuwamy postęp o 8% (0.08)
    animationProgress += 0.08;
    if (animationProgress >= 1.0)
    {
        // Jak dobijemy do 100%, to zatrzymujemy zegar.
        animationProgress = 1.0;
        animationTimer.stop();
        oldPersonRooms.clear();
    }
    update(); // Z każdym "tyknięciem" zlecamy przerysowanie okna
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
    int topLegendSpace = 55;
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

        // Obliczamy fizyczne miejsce (x, y) na ekranie dla konkretnego pokoju
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
    // Komunikat powitalny
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

        // Drzwi rysujemy jako linię łączącą środki dwóch pokojów
        QPointF first = roomCenter(door->getFirstRoom(), roomRects);
        QPointF second = roomCenter(door->getSecondRoom(), roomRects);

        QPen pen;
        // Jeśli drzwi są zablokowane - czerwona, gruba, przerywana linia. Jak nie - szara ciągła.
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

        // Biały prostokąt pokoju z zaokrąglonymi brzegami
        painter.setPen(QPen(QColor(49, 56, 70), 2));
        painter.setBrush(QColor(255, 255, 255));
        painter.drawRoundedRect(roomRect, 8, 8);

        // Podpisujemy pokój (Numer + Nazwa)
        painter.setPen(QColor(30, 35, 45));
        painter.setFont(QFont("Arial", 10, QFont::Bold));
        painter.drawText(roomRect.adjusted(8, 8, -8, -8),
                         Qt::AlignTop | Qt::AlignHCenter,
                         QString::number(room->getId()) + ". " + QString::fromStdString(room->getName()));

        // Jeśli pokój ma kamerę, malujemy małe niebieskie "K" w prawym górnym rogu
        if (room->hasCamera())
        {
            painter.setPen(QPen(QColor(255, 255, 255), 1));
            painter.setBrush(QColor(41, 128, 185));
            painter.drawEllipse(QRectF(roomRect.right() - 18, roomRect.top() + 6, 12, 12));
            painter.setFont(QFont("Arial", 7, QFont::Bold));
            painter.drawText(QRectF(roomRect.right() - 18, roomRect.top() + 6, 12, 12), Qt::AlignCenter, "K");
        }
    }

    if (simulation != nullptr)
    {
        const std::vector<Person*>& allPeople = simulation->getPeople();
        for (std::size_t i = 0; i < allPeople.size(); ++i)
        {
            if (allPeople[i] != nullptr && allPeople[i]->getKind() == "intruz")
            {
                Intruder* intruder = dynamic_cast<Intruder*>(allPeople[i]);
                if (intruder != nullptr && !intruder->isMissionAccomplished())
                {
                    Room* lootRoom = intruder->getTargetRoom();
                    if (lootRoom != nullptr && roomRects.contains(lootRoom))
                    {
                        // Jeśli włamywacz jeszcze nie ukradł łupu, rysujemy złotego dolara nad celem
                        QRectF roomRect = roomRects[lootRoom];
                        QPointF lootPos = roomRect.center() + QPointF(0, -20);

                        painter.save();
                        painter.setBrush(QBrush(QColor(255, 215, 0)));
                        painter.setPen(QPen(Qt::black, 1.5));
                        painter.drawEllipse(lootPos, 11, 11);

                        painter.setPen(Qt::black);
                        painter.setFont(QFont("Arial", 10, QFont::Bold));
                        painter.drawText(QRectF(lootPos.x() - 11, lootPos.y() - 11, 22, 22), Qt::AlignCenter, "$");
                        painter.restore();
                    }
                }
            }
        }
    }
}

QPointF BuildingView::personPoint(const Room* room, int localIndex, const QMap<const Room*, QRectF>& roomRects) const
{
    QRectF roomRect = roomRects.value(room);
    QPointF center = roomRect.center();

    // jeśli w jednym pokoju stoi kilka osób, rozstawiamy ich w szyku (siatce),
    // żeby kropki nie nakładały się w 100% na siebie.
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

        // Liczymy ilu ludzi już narysowaliśmy w tym pokoju (żeby ich nie nałożyć na siebie)
        const Room* currentRoom = person->getCurrentRoom();
        int localIndex = peopleInRoomCount.value(currentRoom, 0);
        peopleInRoomCount[currentRoom] = localIndex + 1;

        QPointF targetPoint = personPoint(currentRoom, localIndex, roomRects);
        QPointF drawPoint = targetPoint;

        // Jeśli trwa animacja, wyliczamy gdzie ludzik powienien być pomiędzy starym a nowym pokojem
        if (animationProgress < 1.0 && oldPersonRooms.contains(person))
        {
            const Room* oldRoom = oldPersonRooms.value(person);
            int oldLocalIndex = 0;
            QPointF startPoint = personPoint(oldRoom, oldLocalIndex, roomRects);

            // Interpolacja liniowa (matematyczne przejście płynnie z punktu A do B)
            drawPoint = QPointF(startPoint.x() + (targetPoint.x() - startPoint.x()) * animationProgress,
                                startPoint.y() + (targetPoint.y() - startPoint.y()) * animationProgress);
        }

        QColor personColor;
        // Wyciągamy z imienia postaci tylko pierwszą literkę (np. 'Z' dla Zbycha) i robimy ją dużą
        QString initial = QString::fromStdString(person->getName()).left(1).toUpper();
        bool isThiefWithLoot = false;

        // Ustalamy kolory w zależności od zawodu (zielony, niebieski, czerwony)
        if (person->getKind() == "intruz")
        {
            personColor = QColor(205, 48, 48);
            Intruder* intr = static_cast<Intruder*>(person);
            if (intr->isMissionAccomplished())
            {
                // Jak intruz ma łup, zamiast literki imienia ma symbol dolara
                isThiefWithLoot = true;
                initial = "$";
            }
        }
        else if (person->getKind() == "ochroniarz")
        {
            personColor = QColor(41, 128, 185);
        }
        else
        {
            personColor = QColor(37, 138, 87);
        }

        // Jeśli to złodziej z łupem, rysujemy mu złotą obwódkę! W przeciwnym razie zwykłą białą.
        if (isThiefWithLoot)
        {
            painter.setPen(QPen(QColor(241, 196, 15), 3));
        }
        else
        {
            painter.setPen(QPen(QColor(255, 255, 255), 2));
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
    // Kod rysujący legendę na górze ekranu (po prostu naklejamy na sztywno znaczki w konkretnych miejscach X, Y)
    painter.setFont(QFont("Arial", 9));

    int y1 = 12;
    painter.setPen(Qt::NoPen); painter.setBrush(QColor(37, 138, 87));
    painter.drawEllipse(QPointF(20, y1), 6, 6);
    painter.setPen(QColor(40, 45, 55)); painter.drawText(32, y1 + 4, "pracownik");

    painter.setPen(Qt::NoPen); painter.setBrush(QColor(41, 128, 185));
    painter.drawEllipse(QPointF(110, y1), 6, 6);
    painter.setPen(QColor(40, 45, 55)); painter.drawText(122, y1 + 4, "ochroniarz");

    painter.setPen(Qt::NoPen); painter.setBrush(QColor(205, 48, 48));
    painter.drawEllipse(QPointF(200, y1), 6, 6);
    painter.setPen(QColor(40, 45, 55)); painter.drawText(212, y1 + 4, "intruz");

    painter.setPen(QPen(QColor(241, 196, 15), 2)); painter.setBrush(QColor(205, 48, 48));
    painter.drawEllipse(QPointF(270, y1), 6, 6);
    painter.setPen(QColor(255, 255, 255)); painter.setFont(QFont("Arial", 7, QFont::Bold));
    painter.drawText(QRectF(264, y1 - 6, 12, 12), Qt::AlignCenter, "$");
    painter.setFont(QFont("Arial", 9));
    painter.setPen(QColor(40, 45, 55)); painter.drawText(282, y1 + 4, "z lupem");

    int y2 = 30;
    painter.setPen(QPen(QColor(255, 255, 255), 1)); painter.setBrush(QColor(41, 128, 185));
    painter.drawEllipse(QRectF(14, y2 - 6, 12, 12));
    painter.setFont(QFont("Arial", 7, QFont::Bold)); painter.drawText(QRectF(14, y2 - 6, 12, 12), Qt::AlignCenter, "K");
    painter.setFont(QFont("Arial", 9));
    painter.setPen(QColor(40, 45, 55)); painter.drawText(32, y2 + 4, "kamera");

    painter.setPen(QPen(QColor(190, 42, 42), 3, Qt::DashLine));
    painter.drawLine(110, y2, 140, y2);
    painter.setPen(QColor(40, 45, 55)); painter.drawText(148, y2 + 4, "drzwi zablokowane");
}