#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include <utility>

enum class Cell{
    Black_P=0,Black_K,White_P,White_K,Empty
};

struct PossibleMove{
    int sx,sy;
    std::pair<int,int> finalPos;
    std::vector<std::pair<int,int>> checkedStones;
};

class Board{
    Cell **bd;
public:
    static const int N = 10;
    Board();
    Board(const Board &);
    Board(Board &&);
    Board & operator=(const Board &);
    Board & operator=(Board &&);
    ~Board();
    std::vector<PossibleMove> find_possible_move(int ,int );
    void make_move(PossibleMove );
//    bool is_over(int );
    void print();
    Cell *operator[](int id){return bd[id];}
    Cell &operator[](std::pair<int,int> id){return bd[id.first][id.second];}
};

inline bool in_board(int ,int );
inline bool colorDiff(Cell, Cell);
inline bool is_king(Cell );
#endif /* end of include guard: __BOARD_H__ */
