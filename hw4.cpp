#include<iostream>

using namespace std;
int index1;
char board[9] = {' ',' ',' ',' ',' ',' ',' ',' ',' '}; 

int isFull() 
{
  for (int i = 0; i < 9; i++) {
    if (board[i] != 'X') {
      if (board[i] != 'O') {
        return 0;
      }
    }
  }
  return 1;
}

int userWon() 
{
  for (int i = 0; i < 9; i += 3) {
    if ((board[i] == board[i + 1]) && (board[i + 1] == board[i + 2]) && (board[i] == 'O'))
      return 1;
  }
  for (int i = 0; i < 3; i++) {
    if ((board[i] == board[i + 3]) && (board[i + 3] == board[i + 6]) && (board[i] == 'O'))
      return 1;
  }
  if ((board[0] == board[4]) && (board[4] == board[8]) && (board[0] == 'O')) {
    return 1;
  }
  if ((board[2] == board[4]) && (board[4] == board[6]) && (board[2] == 'O')) {
    return 1;
  }
  return 0;
}

int AIWon() 
{
  for (int i = 0; i < 9; i += 3) {
    if ((board[i] == board[i + 1]) && (board[i + 1] == board[i + 2]) && (board[i] == 'X'))
      return 1;
  }
  for (int i = 0; i < 3; i++) {
    if ((board[i] == board[i + 3]) && (board[i + 3] == board[i + 6]) && (board[i] == 'X'))
      return 1;
  }
  if ((board[0] == board[4]) && (board[4] == board[8]) && (board[0] == 'X')) {
    return 1;
  }
  if ((board[2] == board[4]) && (board[4] == board[6]) && (board[2] == 'X')) {
    return 1;
  }
  return 0;
}

void printBoard() 
{
  cout << endl;
  cout << board[0] << "|" << board[1] << "|" << board[2] << endl;
  cout << "-----" << endl;
  cout << board[3] << "|" << board[4] << "|" << board[5] << endl;
  cout << "-----" << endl;
  cout << board[6] << "|" << board[7] << "|" << board[8] << endl;
}

int minimax(bool flag) 
{

  int alpha = -1000, beta = 1000;
  int i, j, value = 1;
  if (AIWon() == 1) {
    return 10;
  } else if (userWon() == 1) {
    return -10;
  } else if (isFull() == 1) {
    return 0;
  }
  int score[9] = {1,1,1,1,1,1,1,1,1};

  for (i = 0; i < 9; i++) {
    if (board[i] == ' ') {
      if (beta > alpha) 
      {
        if (flag == true) {
          board[i] = 'X';
          value = minimax(false);
        } else {
          board[i] = 'O';
          value = minimax(true);
        }
        board[i] = ' ';
        score[i] = value;
      }
    }
  }

  if (flag == true) {
    alpha = -1000;
    for (j = 0; j < 9; j++) {
      if (score[j] > alpha && score[j] != 1) {
        alpha = score[j];
        index1 = j;
      }
    }
    return alpha;
  }
  if (flag == false) {
    beta = 1000;
    for (j = 0; j < 9; j++) {
      if (score[j] < beta && score[j] != 1) {
        beta = score[j];
        index1 = j;
      }
    }
    return beta;
  }
}

int main() 
{
  int move, choice;
  cout << "Who wants to go first: " << endl << "1 for player " << "2 for AI" << endl;
  cin >> choice;
  if (choice == 1) {
    printBoard();
    cin >> move;
    board[move - 1] = 'O';
    printBoard();
  }

  while (true) {
    minimax(true);
    board[index1] = 'X';
    printBoard();
    if (AIWon() == 1) {
    cout << endl << "GameOver";
      break;
    }
    if (isFull() == 1) {
      cout << endl << "GameOver";
      break;
    }
    cin >> move;
    board[move - 1] = 'O';
    printBoard();
    if (userWon() == 1) {
      cout << endl << "GameOver";
      break;
    }
    if (isFull() == 1) {
      cout << endl << "GameOver";
      break;
    }
  }

}
