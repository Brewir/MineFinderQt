#include "endgame.h"

EndGame::EndGame(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

EndGame::~EndGame()
{
}

void EndGame::setText(QString result)
{
    ui.result->setText(result);
}
