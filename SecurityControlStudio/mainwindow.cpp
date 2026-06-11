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
    simulation(new Simulation())
{
    ui->setupUi(this);

    ui->pushButtonStart->setText("Wybierz budynek");
    ui->pushButtonStep->setText("Start symulacji");

    connect(ui->pushButtonStart, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(ui->pushButtonStep, &QPushButton::clicked, this, &MainWindow::onStepClicked);

    ui->buildingView->setSimulation(simulation);

    ui->plainTextEditLog->setPlainText(
        "Security Control Studio\n"
        "1. Kliknij 'Wybierz budynek', aby wczytac uklad pomieszczen.\n"
        "2. Kliknij 'Start symulacji', aby wlaczyc automatyczne kroki.\n");

    autoTimer = new QTimer(this);
    isAutoRunning = false;
    connect(autoTimer, &QTimer::timeout, this, &MainWindow::onStepClicked);
}

MainWindow::~MainWindow()
{
    delete simulation;
    delete ui;
}

void MainWindow::onStartClicked()
{
    if (isAutoRunning) {
        autoTimer->stop();
        isAutoRunning = false;
        ui->pushButtonStep->setText("Start symulacji");
    }

    QStringList mapOptions;
    mapOptions << "1. Maly budynek (Podstawowy)" << "2. Duzy budynek (Siatka 4x4)";
    bool ok;
    QString choice = QInputDialog::getItem(this, "Wybór mapy", "Wybierz układ budynku do symulacji:", mapOptions, 0, false, &ok);

    if (!ok || choice.isEmpty()) return;

    QString mapFile;
    if (choice.startsWith("1")) mapFile = QCoreApplication::applicationDirPath() + "/mapa_mala.map";
    else mapFile = QCoreApplication::applicationDirPath() + "/mapa_duza.map";

    if (!QFileInfo::exists(mapFile)) {
        if (choice.startsWith("1")) mapFile = "mapa_mala.map";
        else mapFile = "mapa_duza.map";
    }

    currentMapFile = mapFile;
    std::string errorMessage;

    if (!simulation->loadFromFile(mapFile.toStdString(), errorMessage))
    {
        refreshLog();
        QMessageBox::critical(this, "Blad mapy", QString::fromStdString(errorMessage));
        return;
    }

    refreshLog();
    ui->buildingView->setSimulation(simulation);
    statusBar()->showMessage(QString("Mapa wczytana: ") + mapFile, 5000);
    ui->pushButtonStep->setText("Start symulacji");
}

void MainWindow::onStepClicked()
{
    if (!simulation->isLoaded())
    {
        QMessageBox::warning(this, "Brak mapy", "Najpierw wybierz budynek z listy po lewej stronie!");
        return;
    }

    if (simulation->isFinished())
    {
        std::string errorMessage;
        simulation->loadFromFile(currentMapFile.toStdString(), errorMessage);
        ui->buildingView->setSimulation(simulation);
        refreshLog();

        isAutoRunning = true;
        ui->pushButtonStep->setText("Zatrzymaj symulacje");
        autoTimer->start(600);
        return;
    }

    if (sender() == ui->pushButtonStep)
    {
        if (!isAutoRunning)
        {
            isAutoRunning = true;
            ui->pushButtonStep->setText("Zatrzymaj symulacje");
            autoTimer->start(600);
        }
        else
        {
            isAutoRunning = false;
            autoTimer->stop();
            ui->pushButtonStep->setText("Wznow symulacje");
            return;
        }
    }

    ui->buildingView->rememberPositionsBeforeStep();
    simulation->step();
    ui->buildingView->animateToCurrentPositions();
    refreshLog();

    if (simulation->isFinished())
    {
        if (isAutoRunning)
        {
            autoTimer->stop();
            isAutoRunning = false;
        }
        ui->pushButtonStep->setText("Zagraj ponownie");
        statusBar()->showMessage(QString("Symulacja zostala zakonczona."), 5000);
    }
    else
    {
        statusBar()->showMessage(QString("Wykonano krok symulacji"), 1000);
    }
}

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
    ui->plainTextEditLog->setPlainText(QString::fromStdString(simulation->getLogText()));
    QTextCursor cursor = ui->plainTextEditLog->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->plainTextEditLog->setTextCursor(cursor);
}