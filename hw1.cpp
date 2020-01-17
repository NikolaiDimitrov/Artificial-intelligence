#include <iostream>
#include <list>
#include <cmath>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

#define NUMBER_OF_CHILDRENS 4

class Node {
private:
public:
    int manhatan;
    int zeroPosition;
    int boardDimension;
    int* board;
    char position[5];
    Node** childrens;

    Node(int n, int zeroPosition)
    {
        boardDimension = n;
        this->zeroPosition = zeroPosition;
        board = new int[n * n];
        childrens = new Node*[NUMBER_OF_CHILDRENS];
        for (int i = 0; i < NUMBER_OF_CHILDRENS; ++i) {
            childrens[i] = NULL;
        }
        manhatan = 0;
    }

    Node()
    {
        boardDimension = 0;
        this->zeroPosition = 0;
        board = NULL;
        childrens = new Node*[NUMBER_OF_CHILDRENS];
        manhatan = 0;
    }
    ~Node()
    {
        delete[] board;
    }
    void setBoard(int* boardValues)
    {
        for (int i = 0; i < boardDimension * boardDimension; ++i) {
            board[i] = boardValues[i];
        }
    }

    void printBoard()
    {
        for (int i = 0; i < boardDimension * boardDimension; ++i) {
            if (i % boardDimension == 0)
                cout << endl;
            cout << board[i] << " ";
        }
        cout << endl
             << manhatan << endl;
    }

    void swap(int zeroIndex, int index)
    {
        board[zeroIndex - 1] = board[index];
        board[index] = 0;
        this->zeroPosition = index + 1;
    }
    void calculateManhattan()
    {
        int x1, x2, y1, y2;
        for (int i = 0; i < boardDimension * boardDimension; ++i) {
            if (board[i] != 0) {
                x1 = (i / boardDimension);
                if (((i + 1) % boardDimension) == 0)
                    y1 = boardDimension - 1;
                else
                    y1 = (i) % boardDimension;
                //cout << "x1: " << x1 << " y1: " << y1<< " x2: " << x2 << " y2: " << y2 << endl;
                x2 = (board[i] - 1) / boardDimension;
                y2 = (board[i] - 1) % boardDimension;
                manhatan += (abs(x2 - x1) + abs(y2 - y1));
            }
            //cout << "manhatan " << manhatan << endl;
        }
    }
};
bool compareTarget(int* arr, int* target, int size)
{
    bool flag = true;
    for (int i = 0; i < size; ++i) {
        if (arr[i] != target[i]) {
            flag = false;
            break;
        }
    }
    return flag;
}
void search(int boardDimension, Node* element, int* target, int threshold, int counter, bool& targetFound, vector<string>& print, int& path)
{
    if ((threshold < element->manhatan + counter) || targetFound)
        return;
    if (compareTarget(element->board, target, boardDimension * boardDimension)) {
        path = counter;
        targetFound = true;
        return;
    }
    int boardSize = boardDimension * boardDimension;
    Node up(boardDimension, element->zeroPosition);
    Node down(boardDimension, element->zeroPosition);
    Node right(boardDimension, element->zeroPosition);
    Node left(boardDimension, element->zeroPosition);
    if (!((boardSize - element->zeroPosition) >= (boardSize - boardDimension))) {
        up.setBoard(element->board);
        up.swap(element->zeroPosition, element->zeroPosition - boardDimension - 1);
        up.calculateManhattan();
        strcpy(up.position, "DOWN");
        element->childrens[0] = &up;
    }
    if (!((boardSize - element->zeroPosition) < boardDimension)) {
        down.setBoard(element->board);
        down.swap(element->zeroPosition, element->zeroPosition + boardDimension - 1);
        down.calculateManhattan();
        strcpy(down.position, "UP");
        element->childrens[1] = &down;
    }
    if (!((element->zeroPosition - 1) % boardDimension == 0)) {
        left.setBoard(element->board);
        left.swap(element->zeroPosition, element->zeroPosition - 2);
        left.calculateManhattan();
        strcpy(left.position, "RIGHT");
        element->childrens[2] = &left;
    }
    if (!(element->zeroPosition % boardDimension == 0)) {
        right.setBoard(element->board);
        right.swap(element->zeroPosition, element->zeroPosition);
        right.calculateManhattan();
        strcpy(right.position, "LEFT");
        element->childrens[3] = &right;
    }
    for (int i = 0; i < NUMBER_OF_CHILDRENS; ++i) {
        if (element->childrens[i] != NULL) {
            search(boardDimension, element->childrens[i], target, threshold, counter + 1, targetFound, print, path);
        }
        if (targetFound) {

            print.push_back(element->childrens[i]->position);
            break;
        }
    }
}

int main(int argc, char const* argv[])
{

    int n = 3, zeroPosition = 0, finalZero = 0;

    cin >> n >> finalZero;

    n = sqrt(n + 1);

    if (finalZero == -1)
        finalZero = n * n;

    int* board = new int[n * n];
    int* target = new int[n * n];
    int path = 0;

    for (int i = 0; i < n * n; ++i) {
        target[i] = i + 1;
        cin >> board[i];
        if (!board[i])
            zeroPosition = i + 1;
    }
    target[n*n-1] = 0;
    Node root(n, zeroPosition);
    root.setBoard(board);
    root.calculateManhattan();
    root.printBoard();
    bool flag = false;
    
    vector<string> printArr;

    for (int i = 0; i < root.manhatan + 300; ++i) {
        search(n, &root, target, i, 0, flag, printArr, path);
        if (flag) {
            cout << "Path: " << path << endl;
            break;
        }
    }

    reverse(printArr.begin(), printArr.end());
    for (int i = 0; i < printArr.size(); ++i) {
        cout << printArr[i] << endl;
    }

    delete[] board;
    delete[] target;
    return 0;
}
