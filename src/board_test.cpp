#include "board.h"
#include <bits/stdc++.h>

using namespace std;

int main(){
    Board a;
    for(int i=0;i<Board::M;i++) a.set_cell(i,Cell::Empty);
    a.set_cell(6,Cell::Black_P);a.set_cell(8,Cell::White_P);a.set_cell(9,Cell::White_P);
    a.set_cell(14,Cell::Black_P);
    a.set_cell(16,Cell::Black_P);
    a.set_cell(21,Cell::White_P);
    a.set_cell(26,Cell::Black_P);
    a.set_cell(32,Cell::Black_P);
    a.set_cell(36,Cell::Black_P);
    a.set_cell(42,Cell::White_P);
    for(int color=1;;color^=1){
        a.print();
        set<int> moves;
        moves = a.find_possible_move(color);
        for(auto p : moves){
            pair<int,int> t = Board::pos(p);
            cout << "(" << t.first<< "," << t.second <<")" << " ";
        }
        cout << endl;
        int x,y;
        cin >> x >> y;
        a.select_cell(Board::id(x,y));

        cout << endl;
    }
    return 0;
}
