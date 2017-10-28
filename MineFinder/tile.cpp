#include "tile.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>


Tile::Tile(QWidget *parent)
    : QStackedWidget(parent),
    value(0),    mined(false), flagged(false),
    buttonImage(":/image/Resources/Button.png"),
    flagButtonImage(":/image/Resources/Flag_button.png"),
    markButtonImage(":/image/Resources/Mark_button.png"),
    mineImage(":/image/Resources/Mine.png"),
    tileImages({QPixmap(":/image/Resources/Tile.png"),
        QPixmap(":/image/Resources/Tile1.png"),
        QPixmap(":/image/Resources/Tile2.png"),
        QPixmap(":/image/Resources/Tile3.png"),
        QPixmap(":/image/Resources/Tile4.png"),
        QPixmap(":/image/Resources/Tile5.png"),
        QPixmap(":/image/Resources/Tile6.png"),
        QPixmap(":/image/Resources/Tile7.png"),
        QPixmap(":/image/Resources/Tile8.png") })
{
    QPoint zero(0, 0);
    QSize tileSize(19, 19);
    setGeometry(QRect(zero, tileSize));

    QLabel* pushButton = new QLabel;
    pushButton->setGeometry(QRect(zero, tileSize));
    pushButton->setPixmap(buttonImage);
    addWidget(pushButton);

    QLabel* label = new QLabel;
    label->setGeometry(QRect(zero, tileSize));;
    label->setPixmap(tileImages[value]);
    addWidget(label);
    qDebug() << "Create new tile" ;
    setCurrentIndex(0);
}

Tile::~Tile()
{
    qDebug() << "delete new tile";
}

int Tile::getValue()
{
    return value;
}
void Tile::increment()
{
    static_cast<QLabel*>(widget(1))->setPixmap(mined ? mineImage : tileImages[++value]);
}

bool Tile::isMined()
{
    return mined;
}
void Tile::setMined(bool iMined)
{
    mined = iMined;
    static_cast<QLabel*>(widget(1))->setPixmap(mined ? mineImage : tileImages[value]);
}

bool Tile::isFlagged()
{
    return flagged;
}
void Tile::setFlagged(bool iFlagged)
{
    flagged = iFlagged;
    static_cast<QLabel*>(widget(0))->setPixmap(iFlagged ? flagButtonImage : buttonImage);
}

void Tile::reset()
{
    value = 0;
    mined = false;
    flagged = false;
    setCurrentIndex(0);
    static_cast<QLabel*>(widget(0))->setPixmap(buttonImage);
    static_cast<QLabel*>(widget(1))->setPixmap(tileImages[value]);

}