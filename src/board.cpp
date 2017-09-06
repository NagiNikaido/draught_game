#include <bits/stdc++.h>
#include "board.h"

using namespace std;

Board :: Board(){
    bd = new Cell* [N];
    for(int i=0;i<N;i++) bd = new Cell[N];

}

Board :: Board(const Board &_){
    bd = new Cell *[N];
    for(int i=0;i<N;i++) bd = new Cell[N];
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            bd[i][j] = _.bd[i][j];
}

Board :: Board(Board &&_){
    bd = _.bd; _.bd = nullptr;
}

Board& Board::operator= (const Board &_){
    if(this!=&_){
        if(!bd) {
            bd = new Cell *[N];
            for(int i=0;i<N;i++) bd = new Cell[N];
        }
        for(int i=0;i<N;i++)
            for(int j=0;j<N;j++)
                bd[i][j] = _.bd[i][j];
    }
    return *this;
}

Board& Board::operator= (Board &&_){
    if(this!=&_){
        swap(_.bd,bd);
    }
    return *this;
}

Board :: ~Board(){
    if(bd){
        for(int i=0;i<N;i++) delete[] bd[i];
        delete[] bd;
        bd = nullptr;
    }
}

void make_move(PossibleMove current_move){
    swap((*this)[current_move.pos],(*this)[current_move.sx][current_move.sy]);
    for(auto dd : current_move.checkedStones)
        (*this)[dd] = Cell::Empty;
}
