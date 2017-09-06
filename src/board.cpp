#include <bits/stdc++.h>
#include "board.h"

using namespace std;

Board :: Board(){
    bd = new Cell* [N];
    for(int i=0;i<N;i++) bd[i] = new Cell[N];
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++) bd[i][j]=Cell::Empty;
    for(int i=0;i<4;i++)
        for(int j=!(i&1);j<N;j+=2) bd[i][j]=Cell::Black_P;
    for(int i=6;i<N;i++)
        for(int j=!(i&1);j<N;j+=2) bd[i][j]=Cell::White_P;
}

Board :: Board(const Board &_){
    bd = new Cell *[N];
    for(int i=0;i<N;i++) bd[i] = new Cell[N];
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
            for(int i=0;i<N;i++) bd[i] = new Cell[N];
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

void Board :: make_move(PossibleMove current_move){
    swap((*this)[current_move.finalPos],(*this)[current_move.sx][current_move.sy]);
    for(auto dd : current_move.checkedStones)
        (*this)[dd] = Cell::Empty;
}


static const int dx[4]={1,1,-1,-1};
static const int dy[4]={-1,1,1,-1};
static bool checked[Board::N][Board::N];
static vector<PossibleMove> res;
static int max_count;

inline bool in_board(int x,int y){
    return x>=0 && x<Board::N && y>=0 && y<Board::N;
}
inline bool colorDiff(Cell a,Cell b){
    if(a==Cell::Empty || b==Cell::Empty) return false;
    return (int(a) ^ int(b)) & 2;
}
inline bool is_king(Cell a){
    return int(a) & 1;
}
void dfs(Cell **bd,int x,int y,Cell current_stone,PossibleMove &current_move,int lastDir=-1){
    bool flag = false;
    if(is_king(current_stone)){
        for(int d=0;d<4;d++) if(d!=lastDir){
            int tx,ty;
            for(;in_board(tx,ty) && bd[tx][ty]==Cell::Empty;tx+=dx[d],ty+=dy[d]);
            if(in_board(tx,ty) && colorDiff(bd[tx][ty],current_stone)
                && !checked[tx][ty] && in_board(tx+dx[d],ty+dy[d]) && bd[tx+dx[d]][ty+dy[d]]==Cell::Empty){
                    flag=true;
                    checked[tx][ty]=true;
                    current_move.checkedStones.push_back({tx,ty});
                    dfs(bd,tx+=dx[d],ty+=dy[d],current_stone,current_move);
                    for(tx+=dx[d],ty+=dy[d];in_board(tx,ty) && bd[tx][ty]==Cell::Empty;tx+=dx[d],ty+=dy[d])
                        dfs(bd,tx,ty,current_stone,current_move,d);
                    current_move.checkedStones.pop_back();
                    checked[tx][ty]=false;
                }
        }
    }
    else{
        for(int d=0;d<4;d++){
            int tx=x+dx[d],ty=y+dy[d];
            if(in_board(tx,ty) && colorDiff(bd[tx][ty],current_stone)
                 && !checked[tx][ty] && in_board(tx+dx[d],ty+dy[d]) && bd[tx+dx[d]][ty+dy[d]]==Cell::Empty){
                     flag=true;
                     checked[tx][ty]=true;
                     current_move.checkedStones.push_back({tx,ty});
                     dfs(bd,tx+dx[d],ty+dy[d],current_stone,current_move);
                     current_move.checkedStones.pop_back();
                     checked[tx][ty]=false;
                 }
        }
    }
    if(!flag){
        current_move.finalPos=pair<int,int>(x,y);
        if(current_move.checkedStones.size()>max_count){
            max_count = current_move.checkedStones.size();
            res.clear();res.push_back(current_move);
        }
        else if(current_move.checkedStones.size()==max_count){
            res.push_back(current_move);
        }
    }
}

vector<PossibleMove> Board::find_possible_move(int x,int y){
    PossibleMove current_move;
    Cell current_stone;
    max_count = 0;
    memset(checked,false,sizeof(checked));
    res.clear();
    if(bd[x][y]==Cell::Empty) return res; //an empty cell!
    current_move.sx=x;current_move.sy=y;
    current_stone = bd[x][y];bd[x][y]=Cell::Empty;
    dfs(bd,x,y,current_stone,current_move);
    if(!max_count){
        if(is_king(current_stone)){
            for(int d=0;d<4;d++){
                for(int tx=x+dx[d],ty=y+dy[d];in_board(tx,ty) && bd[tx][ty]==Cell::Empty;tx+=dx[d],ty+=dy[d]){
                    current_move.finalPos=pair<int,int>(tx,ty);
                    res.push_back(current_move);
                }
            }
        }
        else{
            for(int d=0;d<2;d++){
                int tx=x+dx[(int(current_stone)&2)+d],ty=y+dy[(int(current_stone)&2)+d];
                if(in_board(tx,ty) && bd[tx][ty]==Cell::Empty){
                    current_move.finalPos=pair<int,int>(tx,ty);
                    res.push_back(current_move);
                }
            }
        }
    }
    bd[x][y] = current_stone;
    return res;
}

static const char __mapping[5][5]={
    "BP","BK","WP","WK","  "
};

void Board::print(){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++)
            printf("%s ",__mapping[int(bd[i][j])]);
        puts("");
    }
}
