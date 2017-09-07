#include <bits/stdc++.h>
#include "board.h"

using namespace std;

Board :: Board(){
    bd = new Cell* [N];
    rt = new TreeNode* [M];
    for(int i=0;i<M;i++) rt[i] = nullptr;
    for(int i=0;i<N;i++) bd[i] = new Cell[N];
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++) bd[i][j]=Cell::Empty;
    for(int i=0;i<4;i++)
        for(int j=!(i&1);j<N;j+=2) bd[i][j]=Cell::Black_P;
    for(int i=6;i<N;i++)
        for(int j=!(i&1);j<N;j+=2) bd[i][j]=Cell::White_P;
}

Board :: Board(Board &&_){
    bd = _.bd; _.bd = nullptr;
    rt = _.rt; _.rt = nullptr;
}

Board& Board::operator= (Board &&_){
    if(this!=&_){
        swap(_.bd,bd);
        swap(_.rt,rt);
    }
    return *this;
}

Board :: ~Board(){
    if(bd){
        for(int i=0;i<N;i++) delete[] bd[i];
        delete[] bd;
        bd = nullptr;
    }
    if(rt){
        for(int i=0;i<M;i++) if(rt[i]) delete rt[i];
        delete[] rt;
        rt = nullptr;
    }
}


static const int dx[4]={1,1,-1,-1};
static const int dy[4]={-1,1,1,-1};

inline bool colorDiff(Cell a,Cell b){
    if(a==Cell::Empty || b==Cell::Empty) return false;
    return (int(a) ^ int(b)) & 2;
}
inline bool is_king(Cell a){
    return int(a) & 1;
}

TreeNode :: ~TreeNode(){
    for(auto a : ch) if(a.second){
        delete a.second;
        a.second = nullptr;
    }
//    cerr << sid << ' ' << cid << endl;
}
TreeNode * TreeNode :: moveTo(int _id){
    TreeNode *p = this->ch[_id];
    this->ch[_id] = nullptr;
    this->~TreeNode();
    return p;
}
TreeNode :: TreeNode(Cell **bd,int sid,int cid,int h,Cell stone,ULL checked){
    pair<int,int> pos;
    int sx,sy,x,y;
    pos = Board :: pos(sid); sx = pos.first; sy = pos.second;
    pos = Board :: pos(cid); x = pos.first; y = pos.second;
    this->sid = sid; this->cid = cid;
    this->h = h; this->checked = checked;
    this->stone = stone;

    if(is_king(stone)){
        for(int d=0;d<4;d++){
            int tx=x+dx[d],ty=y+dy[d];
            for(;Board::in_board(tx,ty) && (bd[tx][ty]==Cell::Empty || tx==sx && ty==sy);tx+=dx[d],ty+=dy[d]);
            int dd = Board::id(tx,ty);
            if(Board::in_board(tx,ty) && colorDiff(bd[tx][ty],stone)
                && !(checked & (1ULL << dd)) && Board::in_board(tx+dx[d],ty+dy[d])
                    && (bd[tx+dx[d]][ty+dy[d]]==Cell::Empty || tx+dx[d]==sx && ty+dy[d]==sy)){
                        int newId = Board::id(tx+=dx[d],ty+=dy[d]);
                        this->ch[newId] = new TreeNode(bd,sid,newId,h+1,stone,checked|(1ULL<<dd));
                        this->h = max(this->h,this->ch[newId]->h);
                        for(tx+=dx[d],ty+=dy[d];
                            (Board::in_board(tx,ty) && bd[tx][ty]==Cell::Empty || tx==sx && ty==sy);tx+=dx[d],ty+=dy[d])
                                {
                                    newId = Board::id(tx,ty);
                                    this->ch[newId] = new TreeNode(bd,sid,newId,h+1,stone,checked|(1ULL<<dd));
                                    this->h = max(this->h,this->ch[newId]->h);
                                }
                    }
        }
    }
    else{
        for(int d=0;d<4;d++){
            int tx=x+dx[d],ty=y+dy[d],dd=Board::id(tx,ty);
            if(Board::in_board(tx,ty) && colorDiff(bd[tx][ty],stone)
                && !(checked & (1ULL << dd)) && Board::in_board(tx+dx[d],ty+dy[d])
                    && (bd[tx+dx[d]][ty+dy[d]]==Cell::Empty || tx+dx[d]==sx && ty+dy[d]==sy)){
                        int newId = Board::id(tx+dx[d],ty+dy[d]);
                        this->ch[newId] = new TreeNode(bd,sid,newId,h+1,stone,checked|(1ULL<<dd));
                        this->h = max(this->h,this->ch[newId]->h);
            }
        }
    }
}

TreeNode :: TreeNode(Cell **bd,int sid,int cid,Cell stone){
    this->sid = sid, this->cid = cid;
    this->h = 0; this->checked = 0;
    this->stone = stone;
}

void TreeNode :: expand(Cell **bd){
    pair<int,int> pos;
    int x,y;
    pos = Board::pos(cid); x = pos.first; y = pos.second;
    if(is_king(stone)){
        for(int d=0;d<4;d++){
            for(int tx=x+dx[d],ty=y+dy[d];Board::in_board(tx,ty) && bd[tx][ty]==Cell::Empty;tx+=dx[d],ty+=dy[d]){
                int newId = Board::id(tx,ty);
                this->ch[newId] = new TreeNode(bd,sid,newId,stone);
            }
        }
    }
    else{
        for(int d=0;d<2;d++){
            int tx=x+dx[int(stone)&2|d],ty=y+dy[int(stone)&2|d];
            if(Board::in_board(tx,ty) && bd[tx][ty]==Cell::Empty){
                int newId = Board::id(tx,ty);
                this->ch[newId] = new TreeNode(bd,sid,newId,stone);
            }
        }
    }
}

set<int> Board :: find_possible_move(int color){
    for(int i=0;i<M;i++){
        if(rt[i]) delete rt[i];
        rt[i] = nullptr;
        if(int((*this)[Board::pos(i)])/2==color){
            rt[i] = new TreeNode(bd,i,i,0,(*this)[Board::pos(i)],0);
            if(!rt[i]->h) rt[i]->expand(bd);
        }
    }
    int current_h = 0;
    for(int i=0;i<M;i++){
        if(rt[i] && rt[i]->h > current_h) current_h = rt[i]->h;
    }
    set<int> res;
    for(int i=0;i<M;i++) if(rt[i] && rt[i]->h==current_h && rt[i]->ch.size())
        res.insert(i);
    return res;
}
static const char __mapping[5][5]={
    "BP","BK","WP","WK","  "
};

void Board::print(ULL checked){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            printf("%s",__mapping[int(bd[i][j])]);
            if(Board::id(i,j)!=-1 && ((checked >> Board::id(i,j))&1)) putchar('x');
            else putchar(' ');
        }
        puts("");
    }
}

void Board::make_move(int from,int to,int checked,bool isFinal){
    swap((*this)[Board::pos(from)],(*this)[Board::pos(to)]);
    if(checked!=-1) (*this)[Board::pos(checked)] = Cell::Empty;
    if(isFinal &&  !is_king((*this)[Board::pos(to)])){
        pair<int,int> t = Board::pos(to);
        int color = int((*this)[t]) & 2;
        if(color==0 && t.first == 9 || color==1 && t.first==0)
            (*this)[Board::pos(to)] = Cell(int((*this)[Board::pos(to)]) | 1);
    }
}
void Board::select_cell(int pp){
    for(;rt[pp]->ch.size();){
        for(auto qq : rt[pp]->ch)
            if(qq.second->h == rt[pp]->h){
                pair<int,int> np = Board::pos(qq.first);
                cout << "(" << np.first<< "," << np.second <<")" << " ";
            }
        cout << endl;
        int x,y;
        cin >> x >> y;
        ULL kp = rt[pp]->checked ^ rt[pp]->ch[Board::id(x,y)]->checked;

        make_move(rt[pp]->cid,Board::id(x,y),__builtin_ctzll(kp),rt[pp]->ch[Board::id(x,y)]->ch.size());
        rt[pp] = rt[pp]->moveTo(Board::id(x,y));
        print(rt[pp]->checked);
    }
}
