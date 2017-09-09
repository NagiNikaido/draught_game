#include "draughtboard.h"
#include <QDebug>
#include <algorithm>

DraughtBoard::DraughtBoard(QWidget *parent) : QWidget(parent),board()
{
    signalMapper = new QSignalMapper(this);
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);

    pic[0].addFile(":/img/blackP.png");
    pic[1].addFile(":/img/blackK.png");
    pic[2].addFile(":/img/whiteP.png");
    pic[3].addFile(":/img/whiteK.png");
    pic[4].addFile(":/img/empty.png");
/*
    for(int i=0;i<Board::M;i++) board.set_cell(i,Cell::Empty);
    board.set_cell(6,Cell::Black_P);board.set_cell(8,Cell::White_P);board.set_cell(9,Cell::White_P);
    board.set_cell(14,Cell::Black_P);
    board.set_cell(16,Cell::Black_P);
    board.set_cell(21,Cell::White_P);
    board.set_cell(26,Cell::Black_P);
    board.set_cell(32,Cell::Black_P);
    board.set_cell(36,Cell::Black_P);
    board.set_cell(42,Cell::White_P);
*/
    for(int i=0;i<20;i++) board.set_cell(i,Cell::Black_P);
    for(int i=20;i<30;i++) board.set_cell(i,Cell::Empty);
    for(int i=30;i<50;i++) board.set_cell(i,Cell::White_P);
    my_color = 0;color = 0;

    qDebug() << pic[4];
    for(int i=0;i<Board::N;i++)
        for(int j=0;j<Board::N;j++){
            grid[i][j] = new QPushButton(this);
            grid[i][j]->setFixedSize(55,55);
            grid[i][j]->setIconSize(QSize(50,50));
            if((i^j)&1){
                grid[i][j]->setIcon(pic[int(board[i][j])]);
            }
            connect(grid[i][j],SIGNAL(clicked()),signalMapper,SLOT(map()));
            signalMapper->setMapping(grid[i][j],QString::number(Board::id(i,j)));
            gridLayout->addWidget(grid[i][j],i,j);
        }
   connect(signalMapper,SIGNAL(mapped(const QString &)),this,SLOT(onClick(const QString &)));
   //exchangeTurn();
}

void DraughtBoard :: exchangeTurn(){
    clearStyle(SELECTABLE|REACHABLE|REMOVED);
    has_selected=false;current=-1;checked_count = 0;
    for(int i=0;i<Board::N;i++)
        for(int j=0;j<Board::N;j++)
            if((i^j)&1){
                grid[i][j]->setIcon(pic[int(board[i][j])]);
            }
    color ^= 1;
    if(color != my_color) return;
    std::set<int> lst = board.find_possible_move(color);
    if(!lst.size()){
        emit gameOver(color^1);
        return ;
    }
    for(auto p : lst){
        auto pos = Board::pos(p);
        makeSelectable(pos.first,pos.second);
        qDebug() << p << endl;
    }
}

void DraughtBoard :: clearStyle(int bit_flag){
    for(int i=0;i<Board::N;i++){
        for(int j=0;j<Board::N;j++) if(state[i][j] & bit_flag){
            state[i][j] = 0;
            grid[i][j]->setStyleSheet("border : ");
        }
    }
}
void DraughtBoard :: makeSelectable(int x,int y){
    state[x][y] = SELECTABLE;
    grid[x][y]->setStyleSheet("border : 2px solid red");
}
void DraughtBoard :: makeReachable(int x,int y){
    state[x][y] = REACHABLE;
    grid[x][y]->setStyleSheet("border : 2px solid blue");
}
void DraughtBoard :: makeRemoved(int x,int y){
    state[x][y] = REMOVED;
    grid[x][y]->setStyleSheet("border : 2px solid green");
}
void DraughtBoard :: redraw(int id){
    auto pos = Board::pos(id);
    grid[pos.first][pos.second]->setIcon(pic[int(board[pos])]);
}

void DraughtBoard::setCell(int id, int newCell){
    qDebug() << id << newCell;
    board[Board::pos(id)] = Cell(newCell);
    redraw(id);
}

void DraughtBoard :: onClick(const QString &res){
    if(res == "-1" || color != my_color) return;
    auto pos = Board::pos(res.toInt());
    qDebug() << pos << endl;
    if(state[pos.first][pos.second] & SELECTABLE){
        if(!has_selected){
            current = Board::id(pos.first,pos.second);
            clearStyle(REACHABLE);
            showPossibleMove(res.toInt());
        }
    }
    else if(state[pos.first][pos.second] & REACHABLE){
        if(!has_selected){
            has_selected = true;
            clearStyle(SELECTABLE|REACHABLE);
            board.select_cell(current);
        }
        makeMove(res.toInt());
    }
}

void DraughtBoard::showPossibleMove(int id){
    qDebug() << "well" << endl;
    for(auto Pid : board.possible_moves(id)){
        qDebug() << Pid << endl;
        auto pp = Board::pos(Pid);
        makeReachable(pp.first,pp.second);
    }
}

void DraughtBoard::makeMove(int to){
    clearStyle(SELECTABLE|REACHABLE);
    auto info = board.make_move(current,to);
    emit moveFT(current,to,info.first,info.second ? 1 : 0);
    if(info.second){
        exchangeTurn();
    }
    else{
        auto pos = Board::pos(info.first);
        makeRemoved(pos.first,pos.second);
        redraw(current);redraw(to);
        current = to;
        pos = Board::pos(to);
        makeSelectable(pos.first,pos.second);
        showPossibleMove(-1);
        qDebug() << "here!" << pos << endl;
    }
}

static const char * BGMlist[5] = {
    ":/wav/Chatwheel_frog.wav",
    ":/wav/Chatwheel_sad_bone.wav",
    ":/wav/Chatwheel_wan_bu_liao_la.wav",
    ":/wav/Chatwheel_disastah.wav",
    ":/wav/Chatwheel_ay_ay_ay.wav"
};

void DraughtBoard::makeMove(int from,int to,int checked,int isFinal){
    qDebug() << from << " " << to << " " << checked << " " << isFinal;
    clearStyle(SELECTABLE|REACHABLE);
    board.make_move(from,to,checked,isFinal);
    if(checked>=0){
        auto pos = Board::pos(checked);
        ++checked_count;
        makeRemoved(pos.first,pos.second);
    }
    QSound::play(BGMlist[std::min(checked_count,4)]);
    redraw(from);redraw(to);
    auto pos = Board::pos(to);
    makeSelectable(pos.first,pos.second);
    if(isFinal){
        exchangeTurn();
    }
}

DraughtBoard::~DraughtBoard(){
    for(int i=0;i<Board::N;i++)
        for(int j=0;j<Board::N;j++)
            delete grid[i][j];
    delete gridLayout;
    delete signalMapper;
}
