#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include <utility>
#include <map>
#include <set>

enum class Cell{
    Black_P=0,Black_K,White_P,White_K,Empty
};

typedef unsigned long long ULL;

struct TreeNode{
    int sid,cid,h;
    Cell stone;
    unsigned long long checked;
    std::map<int,TreeNode *> ch;
    TreeNode(Cell **,int ,int ,int ,Cell, ULL);
    TreeNode(Cell **,int ,int ,Cell );
    ~TreeNode();
    TreeNode *moveTo(int );
    void expand(Cell **);
};

class Board{
    Cell **bd;
    TreeNode **rt;
public:
    static const int N = 10;
    static const int M = 50;
    Board();
    Board(const Board &) = delete;
    Board(Board &&);
    Board & operator=(const Board &) = delete;
    Board & operator=(Board &&);
    ~Board();
    void print(ULL = 0);
    std::set<int> find_possible_move(int );
    void select_cell(int );
    void make_move(int ,int ,int ,bool);
    Cell *operator[](int _id){return bd[_id];}
    Cell &operator[](std::pair<int,int> _id){return bd[_id.first][_id.second];}
    static int id(int x,int y){
        return ((x^y)&1) ? x*5+(y>>1) : -1;
    }
    static std::pair<int,int> pos(int _id){
        return std::pair<int,int>(_id/5,(_id%5<<1)^(~(_id/5)&1));
    }
    static bool in_board(int x,int y){
        return x>=0 && x<N && y>=0 && y<N;
    }
};

inline bool colorDiff(Cell, Cell);
inline bool is_king(Cell );
#endif /* end of include guard: __BOARD_H__ */
