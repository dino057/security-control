#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

class Simulation;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onStartClicked();
    void onStepClicked();

private:
    QString findDefaultMapFile() const;
    void refreshLog();

    Ui::MainWindow* ui;
    Simulation* simulation;

    QTimer* autoTimer;
    bool isAutoRunning;
    QString currentMapFile;
};

#endif // MAINWINDOW_H