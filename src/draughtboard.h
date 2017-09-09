#ifndef DRAUGHTBOARD_H
#define DRAUGHTBOARD_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <QSignalMapper>
#include <QPushButton>
#include <QSound>
#include "board.h"

class DraughtBoard : public QWidget
{
    Q_OBJECT
    static const int SELECTABLE = 0x01;
    static const int REACHABLE = 0x02;
    static const int REMOVED = 0x04;
public:
    explicit DraughtBoard(QWidget *parent = nullptr);
    QGridLayout *gridLayout;
    QPushButton *grid[Board::N][Board::N];
    QIcon pic[5];
    Board board;
    int state[Board::N][Board::N];
    int color,my_color;
    int current,checked_count;
    bool has_selected;
    QSignalMapper *signalMapper;
    ~DraughtBoard();
    void exchangeTurn();
    void clearStyle(int );
    void showPossibleMove(int );
    void makeSelectable(int ,int );
    void makeReachable(int ,int );
    void makeRemoved(int ,int );
    void makeMove(int );
    void makeMove(int ,int ,int ,int );
    void redraw(int );
    void setCell(int ,int );
signals:
    void turnOver();
    void gameOver(int );
    void moveFT(int ,int ,int ,int );
public slots:
    void onClick(const QString &);
    //void makeMove(int );
    /*
    void setCell(int, int);
    void selectCell(int ,int);
    void makeMove(int ,int );
    void doClicked(const QString &);
    */
};

#endif // DRAUGHTBOARD_H
