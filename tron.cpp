#include <iostream>
#include <vector>
#include <list>
using namespace std;
/* Head ends here */


typedef list< pair<int, int> > coordList;

void printMove(int plx , int ply, int dirx, int diry);
short int canFollowWall(int x, int y, vector <string> board);
coordList getAvailNeighbors(int x, int y, vector <string> board);
void basicWalking(char player,int x, int y, int o_x, int o_y, vector <string> board);

/*
 *  Takes player coordinates as well as "future" tile coordinates(also named direction)
 *  and prints the output accordingly
 *  Utility function --
 */
void printMove(int plx , int ply, int dirx, int diry) {
	if (plx ==  dirx) {

		if(ply < diry) {
			cout << "RIGHT" << endl;
		} else {
			cout << "LEFT" << endl;
		}

	} else
	if (ply == diry) {

		if(plx < dirx) {
			cout << "DOWN" << endl;
		} else {
			cout << "UP" << endl;
		}

	}
}

/*
 * Function takes as parameters player position and the map checks if we can follow wall
 * return value :
 *  0 - no wall to follow
 *  1 - can follow vertical
 *  2 - can follow horizontal
 */
short int canFollowWall(int x, int y, vector <string> board) {

	/*
	 * vertical follow
	 */
	if((board.at(x).at( y - 1) == '#' ) || (board.at(x).at( y + 1) == '#' ))
		return 1;

	/*
	 * horizontal follow
	 */
	if((board.at(x - 1).at(y) == '#' ) || (board.at(x + 1).at(y) == '#' ))
			return 2;
	return 0;
}
/*
 * basic walking function when no wall is close, we just take first direction available
 * might be improved to some more complex logic
 */
void basicWalking(char player,int x, int y, int o_x, int o_y, vector <string> board) {

	list<pair <int, int>> nbs = getAvailNeighbors(x,y, board);

	if(nbs.size() == 0) {
		/*
		 * We have a dead end : suicide by going up
		 */
		printMove(x, y, x + 1, y);
	} else {
		/*
		 * we take first direction available
		 */
		printMove(x,y, nbs.front().first, nbs.front().second);
	}
}

coordList getAvailNeighbors(int x, int y, vector <string> board) {

	coordList result;

	if ((x + 1) <  board.size())
		if(board.at(x+1).at(y) == '-')
			result.push_back(make_pair(x + 1, y));
	if(( x - 1) > 0)
		if(board.at( x - 1).at(y) == '-')
			result.push_back(make_pair(x - 1, y));

	if((y + 1) < board.at(x).length())
		if(board.at(x).at(y + 1) == '-')
			result.push_back(make_pair(x, y + 1));

	if((y - 1) > 0)
		if(board.at(x).at(y - 1))
			result.push_back(make_pair(x, y - 1));

	return result;
}

void nextMove(char player,int x, int y, int o_x, int o_y, vector <string> board){
    //your logic here

	coordList nbs = getAvailNeighbors(x,y, board);

	if (canFollowWall(x,y, board) == 0) {

		basicWalking(player, x, y, o_x, o_y, board);

	}
	if (canFollowWall(x,y, board) == 1) {

		coordList::iterator iter = nbs.begin();

		for(;iter != nbs.end();iter++) {

			int n_x = (*iter).first;
			int n_y = (*iter).second;

			if(n_y == y) {
				printMove(x, y, n_x, n_y);
			}
		}
	}

	if (canFollowWall(x,y, board) == 2) {

		coordList::iterator iter = nbs.begin();

		for(;iter != nbs.end();iter++) {

			int n_x = (*iter).first;
			int n_y = (*iter).second;

			if(n_x == x) {
				printMove(x, y, n_x, n_y);
			}

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

