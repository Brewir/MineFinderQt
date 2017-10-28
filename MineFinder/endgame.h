#pragma once

#include <QDialog>
#include "ui_endgame.h"

class EndGame : public QDialog
{
    Q_OBJECT

public:
    EndGame(QWidget *parent = Q_NULLPTR);
    ~EndGame();

    void setText(QString result);

private:
    Ui::EndGame ui;
};
