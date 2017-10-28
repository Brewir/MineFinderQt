#pragma once

#include <QFrame>

class QWidget;
class QStackedWidget;
class Tile;

class GameFrame : public QFrame
{
    Q_OBJECT

public:
    GameFrame(int width, int height, int count, QWidget *parent = 0);
    ~GameFrame();

    void GameFrame::resize(int iwidth, int iheight, int count);

private:
    void generateMap(std::vector<int>& forbidden);
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);
    void mouseDoubleClickEvent(QMouseEvent * e);

    void popAround(int index);

    void reveal();

public slots:
    void testconnection();
    void resetMap();
    void openTile(int index);


signals:
    void mineUpdate(int mines);
    void startGame();
    void endGame(bool win = true);

private:
    int col;
    int row;
    int mineCount;
    int mineFound;
    int emptyTiles;
    int emptyClosedTiles;
    std::vector<Tile*> map;
};


