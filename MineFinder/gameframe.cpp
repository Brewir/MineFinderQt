#include "gameframe.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QMouseEvent>
#include <QGuiApplication>
#include "tile.h"

#include <QDebug>

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>


template <class function>
void for_3x3(int index, int col, int row, function fn)
{
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            int nx = (index % col) + i;
            int ny = (index / col) + j;
            if (0 <= nx && nx < col && 0 <= ny && ny < row)
            {
                fn(ny * col + nx);
            }
        }
    }
}

GameFrame::GameFrame(int iwidth, int iheight, int count, QWidget * parent):
    QFrame(parent), col(iwidth), row(iheight), mineCount(count), mineFound(0),
    emptyTiles(col * row - mineCount), emptyClosedTiles(emptyTiles), map(col*row)
{
    setGeometry(QRect(1, 1, 19 * col, 19 * row));
    setLayout(new QGridLayout(this));
    layout()->setSpacing(0);
    layout()->setContentsMargins(0, 0, 0, 0);
    int counter = 0;
    std::generate(map.begin(), map.end(), [this, &counter]()
    {
        Tile* tile = new Tile(this);
        static_cast<QGridLayout*>(layout())->addWidget(tile, counter / col, counter % col);
        ++counter;
        return tile;
    });

    srand(time(NULL));
}

GameFrame::~GameFrame()
{
}

void GameFrame::resize(int iwidth, int iheight, int count)
{
    std::for_each(map.begin(), map.end(), [this](Tile* tile) { delete tile; });
    col = iwidth;
    row = iheight;
    mineCount = count;
    emptyTiles = col * row - mineCount;
    map.resize(col*row);

    setGeometry(QRect(1, 1, 19 * col, 19 * row));
    delete layout();
    setLayout(new QGridLayout(this));
    layout()->setSpacing(0);
    layout()->setContentsMargins(0, 0, 0, 0);
    int counter = 0;
    std::generate(map.begin(), map.end(), [this, &counter]()
    {
        Tile* tile = new Tile(this);
        static_cast<QGridLayout*>(layout())->addWidget(tile, counter / col, counter % col);
        ++counter;
        return tile;
    });

    resetMap();
}

void GameFrame::testconnection()
{
    qDebug() << "button clicked";
}

void GameFrame::generateMap(std::vector<int>& forbidden)
{
    int counter = 0;
    while (counter < mineCount)
    {
        int minePos = rand() % (col*row);
        if (count(forbidden.begin(), forbidden.end(), minePos) > 0)
        {
            continue;
        }
        forbidden.push_back(minePos);
        map[minePos]->setMined(true);
        for_3x3(minePos, col, row, [this](int iter) {map[iter]->increment(); });
        ++counter;
    }
}

void GameFrame::resetMap()
{
    std::for_each(map.begin(), map.end(), [this](Tile* tile) { tile->reset(); });
    emptyClosedTiles = emptyTiles;
    mineFound = 0;
    emit mineUpdate(mineCount - mineFound);
}

void GameFrame::openTile(int index)
{
    if (map[index]->currentIndex() == 1) { return; }
    map[index]->setCurrentIndex(1);
    qDebug() << "tile opened" << index;
    if (map[index]->isMined())
    {
        reveal();
        static_cast<QLabel*>(map[index]->widget(1))->setPixmap(QPixmap(":/image/Resources/Open_mine.png"));
        emit endGame(false);
        return;
    }

    if (emptyClosedTiles == emptyTiles)
    {
        emit startGame();
        std::vector<int> forbiddenSlots;
        for_3x3(index, col, row, [&forbiddenSlots](int iter) {forbiddenSlots.push_back(iter); });
        generateMap(forbiddenSlots);
    }
    if (--emptyClosedTiles <= 0)
    {
        emit endGame(true);
    }
    if (map[index]->getValue() == 0)
    {
        for_3x3(index, col, row, [this](int iter) { openTile(iter);  });
    }
    qDebug() << "remaining tiles : " << emptyClosedTiles;
}

void GameFrame::mousePressEvent(QMouseEvent * e)
{
    Tile* tile = map[col * (e->pos().y() / 19) + e->pos().x() / 19];
    if (e->button() == Qt::RightButton && tile->currentIndex() == 0)
    {
        tile->setFlagged(!tile->isFlagged());
        mineFound += tile->isFlagged() ? 1 : -1;
        emit mineUpdate(mineCount - mineFound);
    }
}
void GameFrame::mouseReleaseEvent(QMouseEvent * e)
{
    Tile* tile = map[col * (e->pos().y() / 19) + e->pos().x() / 19];
    if (e->button() == Qt::LeftButton && tile->currentIndex() == 0 && !tile->isFlagged())
    {
        openTile(col * (e->pos().y() / 19) + e->pos().x() / 19);
    }
    if (e->button() == Qt::LeftButton && tile->currentIndex() == 1 && QGuiApplication::keyboardModifiers() != Qt::NoModifier)
    {
        popAround(col * (e->pos().y() / 19) + e->pos().x() / 19);
    }
}
void GameFrame::mouseDoubleClickEvent(QMouseEvent * e)
{
    qDebug() << "mouseDoubleClickEvent : ";
    qDebug() << "button used : " << e->button();
    qDebug() << "position : " << e->pos();
    Tile* tile = map[col * (e->pos().y() / 19) + e->pos().x() / 19];
    if (e->button() == Qt::RightButton && tile->currentIndex() == 0)
    {
        tile->setFlagged(!tile->isFlagged());
        mineFound += tile->isFlagged() ? 1 : -1;
        emit mineUpdate(mineCount - mineFound);
    }
    if (e->button() == Qt::LeftButton && tile->currentIndex() == 1)
    {
        popAround(col * (e->pos().y() / 19) + e->pos().x() / 19);
    }
    // pop around
}
void GameFrame::popAround(int index)
{
    int counter = 0;
    for_3x3(index, col, row, [this, &counter](int iter) { if (map[iter]->isFlagged()) ++counter; });
    if (counter == map[index]->getValue())
    {
        for_3x3(index, col, row, [this](int iter) { if(!map[iter]->isFlagged()) openTile(iter); });
    }
}
void GameFrame::reveal()
{
    for (Tile* tile : map)
    {
        qDebug() << "reveal : " << tile->isFlagged()  << tile->isMined();
        if (tile->isFlagged() && !tile->isMined())
        {
            static_cast<QLabel*>(tile->widget(0))->setPixmap(QPixmap(":/image/Resources/Wrong_flag_button.png"));
        }
        else if (tile->isMined() && !tile->isFlagged())
        {
            qDebug() << "revealed minr : " << tile->isFlagged() << tile->isMined();
            //static_cast<QLabel*>(tile->widget(1))->setPixmap(QPixmap(":/image/Resources/Mine.png"));
            tile->setCurrentIndex(1);
        }
    }
}