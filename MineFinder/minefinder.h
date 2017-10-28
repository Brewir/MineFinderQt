#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_minefinder.h"

#include <QTimer>

class GameFrame;
class EndGame;
class SettingsWindow;

class MineFinder : public QMainWindow
{
    Q_OBJECT

public:
    MineFinder(QWidget *parent = Q_NULLPTR);

private:
    void connectSignals();
    void closeEvent(QCloseEvent *event);
    void adaptGeometry();

public slots:
    void startTimer();
    void stopTimer();
    void updateTimeDisplay();
    void resetCounter();
    void updateMineDisplay(int mineNumber);
    void openDialog(bool win);
    void redraw();


private:
    int timeCount;
    GameFrame* gamefr;
    Ui::MineFinder ui;
    QTimer timer;
    EndGame* endGame;
    SettingsWindow* settingWindow;
};
