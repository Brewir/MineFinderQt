#include "settingswindow.h"
#include <QSettings>

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    setting.gt = Beginer;
    setting.col = 9;
    setting.row = 9;
    setting.mine = 10;
    connect(this, SIGNAL(accepted()), this, SLOT(updateSettings()));
}

SettingsWindow::~SettingsWindow()
{
}

void SettingsWindow::setRadioButton()
{
    switch (setting.gt)
    {
    case Beginer:
        ui.beginerButton->toggle();
        break;
    case Intermediate:
        ui.intermediateButton->toggle();
        break;
    case Expert:
        ui.expertButton->toggle();
        break;
    default:
        ui.customButton->toggle();
        break;
    }

     ui.columnEdit->setText(QString::number(setting.col));
     ui.rowEdit->setText(QString::number(setting.row));
     ui.mineEdit->setText(QString::number(setting.mine));
}

void SettingsWindow::readSettings()
{
    QSettings settings("Brewir", "MineFinder");
    setting.gt = (GameType)settings.value("game_type").toInt();
    setting.col = settings.value("columns").toInt();
    setting.row = settings.value("rows").toInt();
    setting.mine = settings.value("mines").toInt();
    setRadioButton();
}
void SettingsWindow::writeSettings()
{
    QSettings settings("Brewir", "MineFinder");
    settings.setValue("game_type", (int)setting.gt);
    settings.setValue("columns", setting.col);
    settings.setValue("rows", setting.row);
    settings.setValue("mines", setting.mine);
}

void SettingsWindow::updateSettings()
{
    if (ui.beginerButton->isChecked())
    {
        setting.gt = Beginer;
        setting.col = 9;
        setting.row = 9;
        setting.mine = 10;
    }
    else if (ui.intermediateButton->isChecked())
    {
        setting.gt = Intermediate;
        setting.col = 16;
        setting.row = 16;
        setting.mine = 40;
    }
    else if (ui.expertButton->isChecked())
    {
        setting.gt = Expert;
        setting.col = 30;
        setting.row = 16;
        setting.mine = 99;
    }
    else if (ui.customButton->isChecked())
    {
        setting.gt = Custom;
        setting.col = ui.columnEdit->text().toInt();
        setting.row = ui.rowEdit->text().toInt();
        setting.mine = ui.mineEdit->text().toInt();
        if (setting.mine > setting.col * setting.row * 85 / 100)
        {
            setting.mine = setting.col * setting.row * 85 / 100;
        }
    }
    emit settingsUpdated();
}
