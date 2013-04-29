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

typedef vector< pair<int, int> > coordVector;
typedef pair<int, int> move;
typedef pair<int, int> point;

void printMove(char player, int plx , int ply, int dirx, int diry, vector <string> board);
coordVector getAvailNeighbors(int x, int y, vector <string> board);
void basicWalking(char player,int x, int y, int o_x, int o_y, vector <string> board);
vector<int> connexComponents(int x, int y, int o_x, int o_y, vector<string> *board);
int bfs(int x, int y, vector<string>  *board);
void print_board(vector<string> *board);
point double_bfs(int mx, int my, int ex, int ey, vector<string> *normal_board);
int min_move(vector<string> state, pair <int, int> player, move &b);
int max_move(vector<string> state, pair<int, int> player, move &b);


const int dir_x[] = { 0,  1,  0, -1};
const int dir_y[] = {-1,  0,  1,  0};

FILE *f;
move DieMove() {
	//get a tie if poss, otherwise,
	//explore efficiently
}
move doMove(vector<string> &state, move m) {
	move mkockup;
	return mkockup;
}
void Restore(vector<string> &state, move m) {

}

int max_move(vector<string> state, pair<int, int> player, move &b) {
	///change -999 to -infinity + 1
	int retVal = -999;
	bool lost = false;
	//TODO: Count if we re closed and our number of tiles is < his number
	if (lost == true) {
		b = DieMove();
		//evaluare end game
		return retVal;
	} else {
		coordVector moves;
		move mv;
		moves = getAvailNeighbors(player.first, player.second, state);
		int dim = moves.size();
		for (int i = 0; i < dim; i++) {

			mv = moves[i];
			doMove(state, mv);
			move opsBestMove;
			int crt = min_move(state,player , opsBestMove);
			if (crt > retVal) {
				retVal = crt;
				b = moves[i];
			}
			Restore(state, moves[i]);
		}
	}
	return retVal;
}

int min_move(vector<string> state, pair <int, int> player, move &b) {
	int reVal = 1000; //replace with infinity -1
	bool lost;
	if (lost == true) {
		//do shit
		//evaluare end game
	} else {
		coordVector moves;
		moves = getAvailNeighbors(player.first, player.second, state);
		int dim = moves.size();
		for (int i = 0; i < dim; i++) {
			doMove(state, moves[i]);
			move opsBestMove;
			int crtVal = max_move(state, player, opsBestMove);
			if (crtVal < reVal) {
				reVal = crtVal;
				b = moves[i];
			}
			Restore(state, moves[i]);
		}
	}
	return reVal;
}

move minimax(vector<string> state, pair<int, int> player, int maxdepth) {
	move best;
	int sc;
	sc = max_move(state, player, best);

	return best;

}

point double_bfs(int mx, int my, int ex, int ey, vector<string> *normal_board) {

  int m = (*normal_board).size();
  int n = (*normal_board)[0].length();
  vector<vector<int> > board(m);
  
  //0 = liber, 1 = ocupat, 2 sau -2 = la distanta de 1 mutare
  for(int i = 0; i < m; i++) {
    board[i].resize(n);
    for(int j = 0; j < n; j++) {
      if((*normal_board)[i][j] == '-') {
        board[i][j] = 0;
      }
      if((*normal_board)[i][j] == '#'  ||  (*normal_board)[i][j] == 'r'  ||  (*normal_board)[i][j] == 'g') {
        board[i][j] = 1;
      }
    }
  }
  board[mx][my] = 1;
  board[ex][ey] = -1;
  
  int nx, ny, mt = 1, et = 1;
  queue<point> mq, eq;
  mq.push(make_pair(mx, my));
  eq.push(make_pair(ex, ey));

  while(mq.size() && eq.size()) {
    
    mx = mq.front().first;
    my = mq.front().second;
    ex = eq.front().first;
    ey = eq.front().second;
    //avansam in coada doar pt valoarea mai mica
    if(board[mx][my] < abs(board[ex][ey])) {
      mq.pop();
      ex = -1;
    }
    else {
      eq.pop();
      if(board[mx][my] == abs(board[ex][ey])) {
        mq.pop();
      }
      else {
        mx = -1;
      }
    }
    
    for(int i = 0; i < 4; i++) {
      if(mx != -1) {
        nx = mx + dir_x[i];
        ny = my + dir_y[i];
        if(board[nx][ny] == 0) {
          board[nx][ny] = board[mx][my] + 1;
          mq.push(make_pair(nx, ny));
          mt++;
        }
      }
      if(ex != -1) {
        nx = ex + dir_x[i];
        ny = ey + dir_y[i];
        if(board[nx][ny] == 0) {
          board[nx][ny] = board[ex][ey] - 1;
          eq.push(make_pair(nx, ny));
          et++;
        }
        if(board[nx][ny] == -board[ex][ey] + 1) {
          //zona la distante egale se adauga in ambele totaluri
          et++;
        }
      }
    }
    
    //print_board2(&board);
  }  
  
  //totalul meu si al inamicului
  return make_pair(mt, et);
}

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

coordVector getAvailNeighbors(int x, int y, vector <string> board) {
	coordVector result;
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



/*void print_board(vector<string> *board) {
  cout<<"\nboard state:\n";
  for(int i = 0; i < (*board).size(); i++) {
    cout<<(*board).at(i)<<"\n";
  }
}

void print_board2(vector<vector<int> > *board) {
  cout<<"\nboard state:\n";
  for(int i = 0; i < (*board).size(); i++) {
    for(int j = 0; j < (*board)[0].size(); j++) {
      if((*board)[i][j] == 1) {
        printf("  # ");
      }
      else {
        printf("%3d ", (*board)[i][j]);
      }
    }
    printf("\n");
  }
}

vector<point>* articulation_points(vector<string> *board, vector<point> *articulations) {
  
  for(int i = 1; i < (*board).size() - 1; i++) {
    for(int j = 1; j < (*board)[0].length() - 1; j++) {
      //pereti pe coloanele din stanga si dreapta
      if((*board)[i][j] == '-') {
        if(((*board)[i - 1][j - 1] == '#' || (*board)[i][j - 1] == '#'  || (*board)[i + 1][j - 1] == '#')  &&  ((*board)[i - 1][j + 1] == '#' || (*board)[i][j + 1] == '#'  || (*board)[i + 1][j + 1] == '#')) {
          (*articulations).push_back(make_pair(i, j));
          (*board)[i][j] = 'a';
        }
        //pereti pe liniile de sus si jos
        if(((*board)[i - 1][j - 1] == '#' || (*board)[i - 1][j] == '#'  || (*board)[i - 1][j + 1] == '#')  &&  ((*board)[i + 1][j - 1] == '#' || (*board)[i + 1][j] == '#'  || (*board)[i + 1][j + 1] == '#')) {
          (*articulations).push_back(make_pair(i, j));
          (*board)[i][j] = 'a';
        }
      }
    }
  }
  
  return articulations;
}*/
