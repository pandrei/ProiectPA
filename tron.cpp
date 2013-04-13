#include <iostream>
#include <vector>
using namespace std;
/* Head ends here */

bool deadend(int x, int y, vector <string> board) {

	bool result = true;

	if(((x + 1 < board.size()) && (board.at(x + 1).at(y) == '-')) ||
				(( x - 1 > 0 ) && (board.at(x - 1).at(y) == '-')) ||
					((y - 1 > 0) && (board.at(x).at(y - 1) == '-'))  ||
						(y + 1 < (board.at(x).size()) &&(board.at(x).at(y + 1) == '-'))
		)
		result = false;

	return result;
}

void nextMove(char player,int x, int y, int o_x, int o_y, vector <string> board){
    //your logic here
	 if(deadend(x,y, board) ==  true) {
		 cout << "UP"<<endl;
	 } else {
		 if( (x + 1 < board.size()) && (board.at(x + 1).at(y) == '-')) {
			 cout<<"DOWN"<<endl;
		 } else if(( x - 1 > 0 ) && (board.at(x - 1).at(y) == '-')) {
			 cout << "UP"<<endl;
		 } else if ((y - 1 > 0) && (board.at(x).at(y - 1) == '-')) {
			 cout << "LEFT"<<endl;
		 } else if  (y + 1 < (board.at(x).size()) &&(board.at(x).at(y + 1) == '-')) {
			 cout << "RIGHT"<<endl;
		 }
	 }
}
/* Tail starts here */
int main() {

    char player;
    int x, y;
    int o_x, o_y;
    int size_x, size_y;
    vector <string> board;

    cin >> player;
    cin >> x >> y >> o_x >> o_y >> size_x >> size_y;
    for(int i=0; i<size_x; i++) {
        string s; cin >> s;
        board.push_back(s);
    }

    nextMove(player,x,y, o_x, o_y, board);

    return 0;
}

