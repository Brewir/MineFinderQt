#pragma once

#include <QStackedWidget>

class QWidget;

class Tile : public QStackedWidget
{
    Q_OBJECT

public:
    Tile(QWidget *parent = 0);
    ~Tile();

    int getValue();
    void increment();

    bool isMined();
    void setMined(bool iMined = true);

    bool isFlagged();
    void setFlagged(bool iFlagged = true);

    void reset();

private:
    int value;
    bool mined;
    bool flagged;

    QPixmap buttonImage;
    QPixmap flagButtonImage;
    QPixmap markButtonImage;
    QPixmap mineImage;

    std::vector<QPixmap> tileImages;
};
