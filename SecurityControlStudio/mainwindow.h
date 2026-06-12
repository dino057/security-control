/**
 * @file mainwindow.h
 * @brief Definicja klasy MainWindow, glownego okna naszej aplikacji.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

// Forward declaration
class Simulation;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Klasa obslugujaca glowne okno programu (przyciski, logi, widok budynku).
 * * Dziedziczy po QMainWindow, co daje nam dostep do gotowych funkcji okna
 * (np. paska narzedzi, paska stanu na dole ekranu).
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor glownego okna.
     * @param parent Wskaznik na okno-rodzica (domyslnie nullptr, bo to nasze glowne, pierwsze okno).
     */
    explicit MainWindow(QWidget* parent = nullptr);

    /**
     * @brief Destruktor. Sprzata pamiec po zamknieciu okna.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Obsluguje klikniecie przycisku "Wybierz budynek".
     */
    void onStartClicked();

    /**
     * @brief Obsluguje klikniecie przycisku "Start symulacji" / "Zatrzymaj" / "Zagraj ponownie".
     */
    void onStepClicked();

private:
    /**
     * @brief Szuka domyslnego pliku mapy na dysku, jesli uzytkownik zadnego nie wybral.
     * @return Sciezka do pliku mapy.
     */
    QString findDefaultMapFile() const;

    /**
     * @brief Odswieza i przewija tekst w czarnym polu z logami na dole ekranu.
     */
    void refreshLog();

    // Wskaznik na interfejs uzytkownika.
    Ui::MainWindow* ui;

    // Wskaznik na silniksymulacji
    Simulation* simulation;

    // Zmienne potrzebne do automatycznego "klikania" symulacji:
    QTimer* autoTimer;       // Zegar, ktory wykonuje kolejny krok co ulamek sekundy
    bool isAutoRunning;      // Flaga pamietajaca, czy animacja akurat teraz "leci"
    QString currentMapFile;  // Zapamietana sciezka do mapy (niezbedne, zeby przycisk "Zagraj ponownie" wiedzial co wczytac)
};

#endif // MAINWINDOW_H