#pragma once

#include <QDialog>
#include "ui_settingswindow.h"

enum GameType : int
{
    Beginer,
    Intermediate,
    Expert,
    Custom
};
struct Settings
{
    GameType gt;
    int col;
    int row;
    int mine;
};

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    SettingsWindow(QWidget *parent = Q_NULLPTR);
    ~SettingsWindow();

    void setRadioButton();
    void readSettings();
    void writeSettings();

private slots:
    void updateSettings();
signals:
    void settingsUpdated();

public:
    Settings setting;

private:
    Ui::SettingsWindow ui;
};
