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
#include <limits>
#include <queue>
#include <deque>
#include <cmath>
#include <string>
using namespace std;
/* Head ends here */

typedef vector< pair<int, int> > coordVector;
typedef pair<int, int> moveCoord;
typedef pair<int, int> point;

typedef struct st {
	vector<string> board;
	pair<int, int> player;
	pair<int, int> enemy;
	pair<int, int> nextMove;
} State;

void printMove(char player, int plx , int ply, int dirx, int diry, vector <string> board);
coordVector getAvailNeighbors(int x, int y, vector <string> board);
void basicWalking(char player,int x, int y, int o_x, int o_y, vector <string> board);
vector<int> connexComponents(int x, int y, int o_x, int o_y, vector<string> *board);
int bfs(int x, int y, vector<string>  *board);
void print_board(vector<string> *board);
point double_bfs(int mx, int my, int ex, int ey, vector<string> *normal_board);
int max_move(State s, moveCoord &b, int depth);
int min_move(State s, moveCoord &b, int depth);

const int infinity = numeric_limits<int>::max() - 1;
const int n_infinity = numeric_limits<int>::min() + 1;
const int dir_x[] = { 0,  1,  0, -1};
const int dir_y[] = {-1,  0,  1,  0};
const int maxdepth = 3;
int force_stop = 5;
bool flag1 = true, flag2 = true;

//FILE *f;
ofstream f;


void swap (int *a, int*b) {
  int aux;
	aux = *b;
	*b = *a;
	*a = aux;
}

void print_board(vector<string> *board) {
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

string get_direction(point curPos, point movPos) {
  //char *s = (char *) calloc(5, sizeof(char));
  //considera mutarea valida
  if(curPos.second + dir_y[0] == movPos.second) {
    return("LEFT\n");
  }
  if(curPos.first + dir_x[1] == movPos.first) {
    return("DOWN\n");
  }
  if(curPos.second + dir_y[2] == movPos.second) {
    return("RIGHT\n");
  }
  if(curPos.first + dir_x[3] == movPos.first) {
    return("UP\n");
  }
  return("error\n");
}

/* Tail starts here */
void moveto(moveCoord goTo, moveCoord player) {
	if(player.first == goTo.first)
		if (player.second > goTo.second)
			printf("LEFT\n");
		else
			printf("RIGHT\n");
	if(player.second == goTo.second)
		if(player.first < goTo.first)
			printf("DOWN\n");
		else
			printf("UP\n");
}

void doMove(State &state, moveCoord m, bool me) {
  if(me) {
    state.board[m.first][m.second] = state.board[state.player.first][state.player.second];
	  state.player.first = m.first;
	  state.player.second = m.second;
	}
	else {
    state.board[m.first][m.second] = state.board[state.enemy.first][state.enemy.second];
	  state.enemy.first = m.first;
	  state.enemy.second = m.second;	
	}
}

void undoMove(State &state, moveCoord m, bool me) {
  if(me) {
    state.board[state.player.first][state.player.second] = '-';
	  state.player.first = m.first;
	  state.player.second = m.second;
	}
	else {
	  state.board[state.enemy.first][state.enemy.second] = '-';
	  state.enemy.first = m.first;
	  state.enemy.second = m.second;
	}
}

bool finalState(State s, int depth) {
	return depth == 0;
}

int max_move(State s, moveCoord &b, int depth) {
	printf("\n\n\nin max, depth = %d", depth);
	print_board(&s.board);
	//depth--;
	int retVal = n_infinity;
	moveCoord result;
	if (finalState(s, depth) == true) {
	  printf("final state\n");
		vector<string> backup = s.board;
		result = double_bfs(s.player.first, s.player.second, s.enemy.first, s.enemy.second, &backup);
		return (100 * result.first / result.second);
	} else {
		coordVector moves;
		moveCoord mv, player;
		moves = getAvailNeighbors(s.player.first, s.player.second, s.board);
		int dim = moves.size();
		if (dim == 0) {
		  printf("no moves\n");
			return n_infinity;
		}
		player.first = s.player.first;
	  player.second = s.player.second;
		for (int i = 0; i < dim; i++) {
			mv = moves[i];
		  printf("going for move <%d,%d>\n", mv.first, mv.second);
		  cout << get_direction(player, mv);
			doMove(s, mv, true);
			//print_board(&s.board);
			moveCoord opsBestMove;
			int crt = min_move(s, opsBestMove, depth - 1);
			if(depth == maxdepth) {
			  f << get_direction(player, mv) + " for depth " << depth << " and profit " << crt <<endl;
			}
			//fprintf(f, " for depth %d, profit %d\n", maxdepth - depth, crt);
			if (crt >= retVal) {
				retVal = crt;
				b = mv;
			}
			undoMove(s, player, true);
		}
	}
	return retVal;
}

int min_move(State s, moveCoord &b, int depth) {
	printf("\n\n\nin min, depth = %d", depth);
	print_board(&s.board);
	int retVal = infinity;
	if (finalState(s, depth) == true) {
	  printf("final state\n");
		vector<string> backup = s.board;
		moveCoord result;
		result = double_bfs(s.player.first, s.player.second, s.enemy.first, s.enemy.second, &backup);
		return (100 * result.second / result.first);
	} else {
		coordVector moves;
		moveCoord mv, player;
		moves = getAvailNeighbors(s.enemy.first, s.enemy.second, s.board);
		int dim = moves.size();
		if (dim == 0) {
		  printf("no moves\n");
			return infinity;
		}
		player.first = s.enemy.first;
		player.second = s.enemy.second;
		for (int i = 0; i < dim; i++) {
		  mv = moves[i];
		  printf("going for move <%d,%d>\n", mv.first, mv.second);
		  cout << get_direction(player, mv);
			doMove(s, mv, false);
			//print_board(&s.board);
			moveCoord opsBestMove;
			int crtVal = max_move(s, opsBestMove, depth - 1);
			if (crtVal <= retVal) {
				retVal = crtVal;
				b = mv;
			}
			undoMove(s, player, false);
		}
	}
	return retVal;
}

/*int min_move(State s, moveCoord &b, int depth) {
  if(!(force_stop--)) {
    return -1;
  }
	printf("\n\n\nin min, depth = %d", depth);
	print_board(&s.board);
	int retVal = infinity;
	if (finalState(s, depth) == true) {
	  printf("final state\n");
		vector<string> backup = s.board;
		moveCoord result;
		result = double_bfs(s.player.first, s.player.second, s.enemy.first, s.enemy.second, &backup);
		return ((result.second / result.first) * 100);
	} else {
		coordVector moves;
		moveCoord mv, player;
		moves = getAvailNeighbors(s.player.first, s.player.second, s.board);
		int dim = moves.size();
		if (dim == 0) {
		  printf("no moves\n");
			return infinity;
		}
		player.first = s.player.first;
		player.second = s.player.second;
		for (int i = 0; i < dim; i++) {
		  mv = moves[i];
		  printf("going for move <%d,%d>\n", mv.first, mv.second);
		  cout << get_direction(player, mv);
			doMove(s, mv);
			print_board(&s.board);
			moveCoord opsBestMove;
			int crtVal = max_move(s, opsBestMove, depth - 1);
			if (crtVal <= retVal) {
				retVal = crtVal;
				b = mv;
			}
			Restore(s, player);
		}
	}
	return retVal;
}*/

moveCoord minimax(State s) {
	moveCoord best;
	int sc = max_move(s, best, maxdepth);
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

  while(!mq.empty() || !eq.empty()) {

    if(mq.empty()) {
      mx = -1;
    }
    else {
      mx = mq.front().first;
      my = mq.front().second;
    }
    
    if(eq.empty()) {
      ex = -1;
    }
    else {
      ex = eq.front().first;
      ey = eq.front().second;
    }
    //avansam in coada doar pt valoarea mai mica
    if(!mq.empty() && !eq.empty()) {
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
    }
    else {
      if(mq.empty()) {
        eq.pop();
      }
      else {
        mq.pop();
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






int main() {

    char player;
    int x, y;
    int o_x, o_y;
    int size_x, size_y;
    vector <string> board;
    
    //f = fopen("debug", "w");
    
    f.open ("debug");

    cin >> player;
    cin >> x >> y >> o_x >> o_y >> size_x >> size_y;
    for(int i=0; i<size_x; i++) {
        string s; cin >> s;
        board.push_back(s);
    }

    //basicWalking(player, x, y, o_x, o_y, &board);
    State s;
    s.player.first = x;
    s.player.second = y;
    s.enemy.first = o_x;
    s.enemy.second = o_y;
    s.board = board;
    moveCoord res, aux;
    if(player =='g') {
    	aux = s.player;
    	s.player = s.enemy;
    	s.enemy = aux;
    }
    //point p = double_bfs(s.player.first, s.player.second, s.enemy.first, s.enemy.second, &(s.board));
    //printf("\n%d %d   %d\n", p.first, p.second, 100 * p.first / p.second);
    res =  minimax(s);

    printf("\nfinal result\n");
    moveto(res, s.player);
    return 0;
}





/*
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
