#include "minefinder.h"

#include <sstream>
#include <iomanip>
#include <QDebug>
#include <QCloseEvent>

#include "gameframe.h"
#include "endgame.h"
#include "settingswindow.h"


QString toDisplay(int entry, int length, char fill = '0')
{
    std::stringstream ss;
    ss << std::setw(length) << std::setfill(fill) << entry;
    return ss.str().c_str();
}

MineFinder::MineFinder(QWidget *parent)
    : QMainWindow(parent), timeCount(0), gamefr(0)
{
    ui.setupUi(this);
    settingWindow = new SettingsWindow(this);
    settingWindow->readSettings();

    gamefr = new GameFrame(settingWindow->setting.col, settingWindow->setting.row, settingWindow->setting.mine, ui.intermediate);

    adaptGeometry();

    ui.timeCounter->display(toDisplay(timeCount, ui.timeCounter->digitCount()));
    ui.mineCounter->display(toDisplay(settingWindow->setting.mine, ui.timeCounter->digitCount()));

    endGame = new EndGame(this);
    
    connectSignals();
}

void MineFinder::updateTimeDisplay()
{
    ui.timeCounter->display(toDisplay(++timeCount, ui.timeCounter->digitCount()));
}

void MineFinder::resetCounter()
{
    timeCount = 0;
    ui.timeCounter->display(toDisplay(timeCount, ui.timeCounter->digitCount()));
}

void MineFinder::startTimer()
{
    timer.start(1000);
}

void MineFinder::stopTimer()
{
    timer.stop();
}

void MineFinder::updateMineDisplay(int mineNumber)
{
    ui.mineCounter->display(toDisplay(mineNumber, ui.timeCounter->digitCount()));
}

void MineFinder::openDialog(bool win)
{
    
    endGame->setText(QString(win ? "gagne" : "perdu"));
    endGame->open();
}

void MineFinder::redraw()
{
    gamefr->resize(settingWindow->setting.col, settingWindow->setting.row, settingWindow->setting.mine);
    adaptGeometry();
}
void MineFinder::adaptGeometry()
{
    int interX = ui.intermediate->x();
    int interY = ui.intermediate->y();
    ui.intermediate->setGeometry(QRect(interX, interY, gamefr->width() + 2, gamefr->height() + 2));
    int winWidth = interX * 2 + ui.intermediate->width();
    int winHeight = interX + interY + ui.menubar->height() + ui.intermediate->height();
    resize(winWidth, winHeight);

    int timePos = (winWidth / 3) - (ui.timeCounter->width() / 2);
    int countPosY = interY / 2 - ui.timeCounter->height() / 2;
    int minePos = 2 * (winWidth / 3) - (ui.mineCounter->width() / 2);

    ui.timeCounter->setGeometry(QRect(timePos, countPosY, ui.timeCounter->width(), ui.timeCounter->height()));
    ui.mineCounter->setGeometry(QRect(minePos, countPosY, ui.mineCounter->width(), ui.mineCounter->height()));
}
void MineFinder::connectSignals()
{
    connect(settingWindow, SIGNAL(settingsUpdated()), this, SLOT(resetCounter()));
    connect(settingWindow, SIGNAL(settingsUpdated()), this, SLOT(stopTimer()));
    connect(settingWindow, SIGNAL(settingsUpdated()), this, SLOT(redraw()));
    connect(ui.actionOptions, SIGNAL(triggered()), settingWindow, SLOT(open()));
    connect(gamefr, SIGNAL(mineUpdate(int)), this, SLOT(updateMineDisplay(int)));
    connect(gamefr, SIGNAL(startGame()), this, SLOT(startTimer()));
    connect(gamefr, SIGNAL(endGame(bool)), this, SLOT(openDialog(bool)));
    connect(gamefr, SIGNAL(endGame()), this, SLOT(stopTimer()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateTimeDisplay()));
    connect(ui.actionNouveau_Jeu, SIGNAL(triggered()), gamefr, SLOT(resetMap()));
    connect(ui.actionNouveau_Jeu, SIGNAL(triggered()), this, SLOT(resetCounter()));
    connect(ui.actionNouveau_Jeu, SIGNAL(triggered()), this, SLOT(stopTimer()));
    connect(ui.actionQuitter, SIGNAL(triggered()), this, SLOT(close()));
    connect(endGame, SIGNAL(rejected()), this, SLOT(close()));
    connect(endGame, SIGNAL(accepted()), gamefr, SLOT(resetMap()));
    connect(endGame, SIGNAL(accepted()), this, SLOT(resetCounter()));
}


void MineFinder::closeEvent(QCloseEvent *event)
{
    settingWindow->writeSettings();
    event->accept();
}