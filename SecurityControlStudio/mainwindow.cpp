#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "BuildingView.h"
#include "Simulation.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QStatusBar>
#include <QTextCursor>
#include <QStringList>
#include <QInputDialog>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    simulation(new Simulation()) // Tworzymy nasz silnik przy odpaleniu okna
{
    // setupUi to gotowa funkcja z biblioteki Qt. Ona "rozpakowuje" nasz układ guzików
    // z pliku .ui (stworzonego w Qt Designerze) i wrzuca je na to szare okno.
    ui->setupUi(this);

    // Nadajemy nazwy przyciskom po lewej na dole
    ui->pushButtonStart->setText("Wybierz budynek");
    ui->pushButtonStep->setText("Start symulacji");

    connect(ui->pushButtonStart, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(ui->pushButtonStep, &QPushButton::clicked, this, &MainWindow::onStepClicked);

    // Podłączamy nasz graficzny widok z mapą budynku pod silnik logiczny.
    ui->buildingView->setSimulation(simulation);

    // Początkowy tekst w czarnym oknie na dole
    ui->plainTextEditLog->setPlainText(
        "Security Control Studio\n"
        "1. Kliknij 'Wybierz budynek', aby wczytac uklad pomieszczen.\n"
        "2. Kliknij 'Start symulacji', aby wlaczyc automatyczne kroki.\n");

    // QTimer to nasz niewidzialny "klikacz".
    // Kiedy go włączymy, będzie sam, co ułamek sekundy, wywoływał funkcję onStepClicked.
    autoTimer = new QTimer(this);
    isAutoRunning = false;
    connect(autoTimer, &QTimer::timeout, this, &MainWindow::onStepClicked);
}

MainWindow::~MainWindow()
{
    delete simulation;
    // Interfejs (ui) też jest usuwany, bo wygenerował go Qt.
    delete ui;
}

void MainWindow::onStartClicked()
{
    // Jeśli symulacja aktualnie sobie chodzi w tle, a gracz chce wczytać nową mapę,
    // to najpierw twardo zatrzymujemy zegar.
    if (isAutoRunning) {
        autoTimer->stop();
        isAutoRunning = false;
        ui->pushButtonStep->setText("Start symulacji");
    }

    // Tworzymy małą listę wyboru map, żeby ułatwić życie (nie trzeba szukać plików po dysku)
    QStringList mapOptions;
    mapOptions << "1. Maly budynek (Podstawowy)" << "2. Duzy budynek (Siatka 4x4)";
    bool ok;
    // QInputDialog to małe, wyskakujące na środek ekranu okienko z rozwijaną listą
    QString choice = QInputDialog::getItem(this, "Wybór mapy", "Wybierz układ budynku do symulacji:", mapOptions, 0, false, &ok);

    // Jeśli ktoś kliknął krzyżyk albo "Anuluj" w tym małym okienku, to nic nie robimy
    if (!ok || choice.isEmpty()) return;

    // Ustalamy ścieżkę do wybranej mapy.
    // QCoreApplication::applicationDirPath() podaje nam folder, z którego odpaliliśmy symulację.
    QString mapFile;
    if (choice.startsWith("1")) mapFile = QCoreApplication::applicationDirPath() + "/mapa_mala.map";
    else mapFile = QCoreApplication::applicationDirPath() + "/mapa_duza.map";

    // Awaryjne rozwiązanie: gdyby Qt zwariowało ze ścieżkami, szukamy pliku "obok" pliku wykonywalnego
    if (!QFileInfo::exists(mapFile)) {
        if (choice.startsWith("1")) mapFile = "mapa_mala.map";
        else mapFile = "mapa_duza.map";
    }

    // Zapamiętujemy wybraną mapę, żeby guzik "Zagraj ponownie" wiedział, co odświeżyć
    currentMapFile = mapFile;
    std::string errorMessage;

    // Próbujemy wczytać mapę przez klasę Simulation (a ona deleguje to do Building).
    // Jeśli się nie uda, to wyskoczy błąd.
    if (!simulation->loadFromFile(mapFile.toStdString(), errorMessage))
    {
        refreshLog(); // Odświeżamy logi, żeby pokazać tekst z błędem z pliku tekstowego
        QMessageBox::critical(this, "Blad mapy", QString::fromStdString(errorMessage));
        return;
    }

    // Jeśli mapa wczytała się poprawnie:
    refreshLog();
    ui->buildingView->setSimulation(simulation); // Mówimy grafice, żeby narysowała nowe pokoje
    statusBar()->showMessage(QString("Mapa wczytana: ") + mapFile, 5000); // Mały napis w lewym dolnym rogu ekranu (na 5 sekund)
    ui->pushButtonStep->setText("Start symulacji"); // Guzik znów gotowy do odpalenia
}

void MainWindow::onStepClicked()
{
    // Zabezpieczenie przed głupimi kliknięciami
    if (!simulation->isLoaded())
    {
        QMessageBox::warning(this, "Brak mapy", "Najpierw wybierz budynek z listy po lewej stronie!");
        return;
    }

    // Kiedy symulacja się skończyła (wygrana/przegrana) guzik zmienił nazwę na "Zagraj ponownie".
    // Klikając w niego po zakończeniu gry wpadamy w ten warunek:
    if (simulation->isFinished())
    {
        std::string errorMessage;
        // Wczytujemy z pamięci jeszcze raz tę samą mapę, co resetuje ludzików na start
        simulation->loadFromFile(currentMapFile.toStdString(), errorMessage);
        ui->buildingView->setSimulation(simulation);
        refreshLog();

        // I od razu puszczamy autmatyczny start!
        isAutoRunning = true;
        ui->pushButtonStep->setText("Zatrzymaj symulacje");
        autoTimer->start(600); // Krok symulacji wywoływany automatycznie co 600 milisekund
        return;
    }

    // sender() sprawdza, czy to my kliknęliśmy guzik myszką, czy to nasz niewidzialny zegar "autoTimer" wywołał funkcję.
    // Jeśli my to kliknęliśmy:
    if (sender() == ui->pushButtonStep)
    {
        if (!isAutoRunning) // Jeśli stało, to zaczynamy bieg
        {
            isAutoRunning = true;
            ui->pushButtonStep->setText("Zatrzymaj symulacje");
            autoTimer->start(600);
        }
        else // Jeśli już biegło, to zatrzymujemy awaryjnie grę
        {
            isAutoRunning = false;
            autoTimer->stop();
            ui->pushButtonStep->setText("Wznow symulacje");
            return; // Zatrzymujemy natychmiast
        }
    }

    // -----------------------------------------------------
    // PONIŻEJ JEST SAMA LOGIKA POJEDYNCZEGO KROKU SYMULACJI
    // -----------------------------------------------------

    // 1. Zrób "zdjęcie" gdzie kto teraz stoi
    ui->buildingView->rememberPositionsBeforeStep();

    // 2. Wywołanie kroku (ludzie zmieniają pokoje w kodzie, kamery szukają, drzwi się blokują)
    simulation->step();

    // 3. Powiedz grafice, żeby przemieściła powoli kropki na ekranie ze starych zdjęć na nowe
    ui->buildingView->animateToCurrentPositions();

    // 4. Przepisz tekst z historii logów na ten czarny panel na dole
    refreshLog();

    // Sprawdzamy, czy symulacja nie skończyła się np. złapaniem złodzieja w tym kroku
    if (simulation->isFinished())
    {
        if (isAutoRunning)
        {
            // Zatrzymujemy automatyczne "klikanie"
            autoTimer->stop();
            isAutoRunning = false;
        }
        ui->pushButtonStep->setText("Zagraj ponownie"); // Zmieniamy nazwę na restart
        statusBar()->showMessage(QString("Symulacja zostala zakonczona."), 5000);
    }
    else
    {
        statusBar()->showMessage(QString("Wykonano krok symulacji"), 1000);
    }
}

// Funkcja poszukująca awaryjnej mapy na starym kodzie (gdyby zabrakło docelowych)
QString MainWindow::findDefaultMapFile() const
{
    QStringList candidates;
    candidates << QCoreApplication::applicationDirPath() + "/sample_building.map";
    candidates << QDir::currentPath() + "/sample_building.map";
    candidates << QCoreApplication::applicationDirPath() + "/../sample_building.map";

    for (int i = 0; i < candidates.size(); ++i)
    {
        if (QFileInfo::exists(candidates[i]))
        {
            return candidates[i];
        }
    }
    return candidates[0];
}

void MainWindow::refreshLog()
{
    // Pobieramy całego Stringa z logami i ustawiamy w polu
    ui->plainTextEditLog->setPlainText(QString::fromStdString(simulation->getLogText()));

    // Zmuszamy suwak po prawej stronie czarnego pola, żeby sam zjechał na sam dół (End).
    // Inaczej po zrobieniu 50 kroków gry widzielibyśmy tylko to, co było na początku.
    QTextCursor cursor = ui->plainTextEditLog->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->plainTextEditLog->setTextCursor(cursor);
}