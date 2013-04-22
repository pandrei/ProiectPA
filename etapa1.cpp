/*
 * Proiect PA - Tron
 * Echipa Game of Tron
 * Popescu Andrei - Constantin 332 CA capitan
 * Tudor Tache 323 CA
 * Necula Roxana - Valentina 332 CA
 */

#include <iostream>
#include <vector>
#include <list>
#include <fstream>
using namespace std;
/* Head ends here */

typedef list< pair<int, int> > coordList;

void printMove(char player, int plx , int ply, int dirx, int diry, vector <string> board);
coordList getAvailNeighbors(int x, int y, vector <string> board);
void basicWalking(char player,int x, int y, int o_x, int o_y, vector <string> board);
vector<int> connexComponents(int x, int y, int o_x, int o_y, vector<string> *board);
int bfs(int x, int y, vector<string>  *board);
void print_board(vector<string> *board);


const int dir_x[] = { 0,  1,  0, -1};
const int dir_y[] = {-1,  0,  1,  0};

FILE *f;

void swap (int *a, int*b) {
  int aux;
	aux = *b;
	*b = *a;
	*a = aux;
}

/*
 * basic walking function when no wall is close, we just take first direction available
 * might be improved to some more complex logic
 */
void basicWalking(char player,int x, int y, int o_x, int o_y, vector <string> *board) {

  if(player == 'g') {
	  swap(&x, &o_x);
	  swap(&y, &o_y);
  }  
  
  vector<int> counts = connexComponents(x, y, o_x, o_y, board);
	int max = -1, pmax = 0;
	for(int i = 0; i < 4; i++) {
	  if(counts[i] > max) {
	    max = counts[i];
	    pmax = i;
	  }
	}
	switch(pmax) {
	  case 0:
	    cout << "LEFT" << endl;
	  break;
	  case 1:
	    cout << "DOWN" << endl;
	  break;
	  case 2:
	    cout << "RIGHT" << endl;
	  break;
	  case 3:
	    cout << "UP" << endl;
	  break;
	}
}

coordList getAvailNeighbors(int x, int y, vector <string> board) {
	coordList result;
	int nx, ny;

	for(int i = 0; i < 4; i++) {
		nx = x + dir_x[i];
		ny = y + dir_y[i];
     
		if(board[nx][ny] == '-') {
		  result.push_back(make_pair(nx, ny));
		}
  }
  
	return result;
}

int bfs(int xx, int yy, vector<string> *board) {

  list <int> x, y;
  int px, py, nx, ny, total = 0;

  if((*board).at(xx).at(yy) == '-') {
    x.push_back(xx);
    y.push_back(yy);
  }
  
  while(x.size()) {
    total++;
    px = x.front();
    py = y.front();
    x.pop_front();
    y.pop_front();
    
    for(int i = 0; i < 4; i++) {
      nx = px + dir_x[i];
      ny = py + dir_y[i];
      if((*board)[nx][ny] == '-') {
        x.push_back(nx);
        y.push_back(ny);
        (*board)[nx][ny] = '*';
      }
    }
  }  
  return total;
}

/* 
 * works good if wall follow has the same direction priority
 * Isolated areas have a size:
 * [0] my left
 * [1] my down
 * [2] my right
 * [3] my up
 * [4] enemy's left
 * [5] enemy's down
 * [6] enemy's right
 * [7] enemy's up
 */
vector<int> connexComponents(int x, int y, int o_x, int o_y, vector<string> *board) {

  vector<string> my_board = (*board), opponent_board = (*board);
  vector<int> res(8);
  
  for(int i = 0; i < 4; i++) {
    res[i] = bfs(x + dir_x[i], y + dir_y[i], &my_board);
    res[i + 4] = bfs(o_x + dir_x[i], o_y + dir_y[i], &opponent_board);
  }
  
  return res;
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

    basicWalking(player, x, y, o_x, o_y, &board);
    return 0;
}
