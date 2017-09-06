#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>

enum class Cell{
    Empty,Black_P,Black_K,White_P,White_K;
};

struct PossibleMove{
    int sx,sy;
    std::vector<pair<int,int>> pos;
    std::vector<pair<int,int>> checkedStones;
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
    bool is_over(int );
    Cell *operator[](int id){return bd[id];}
    Cell &operator[](pair<int,int> id){return bd[id.first][id.second];}
};

#endif /* end of include guard: __BOARD_H__ */
