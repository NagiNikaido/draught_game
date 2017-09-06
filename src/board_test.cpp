#include "board.h"
#include <bits/stdc++.h>

using namespace std;

int main(){
    Board a;
    for(;;){
        a.print();
        int x,y;
        cin >> x >> y;
        vector<PossibleMove> moves;
        moves = a.find_possible_move(x,y);
        for(auto mm : moves){
            cout << "finalPos = (" << mm.finalPos.first << "," << mm.finalPos.second << ")";
            cout << endl;
            cout << "{";
            for(auto pp : mm.checkedStones)
                cout << "(" << pp.first << "," << pp.second << "), ";
            cout << "}" << endl;
        }
        int num;
        cin >> num;
        if(num<moves.size()) a.make_move(moves[num]);
    }
    return 0;
}
