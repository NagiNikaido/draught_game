#include "board.h"
#include <bits/stdc++.h>

using namespace std;

int main(){
    Board a;
    for(int color=0;;color^=1){
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
