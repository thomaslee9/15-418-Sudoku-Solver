// solver-seq.cpp

#include "sudoku-board.cpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <cassert>
#include <stack>

bool loadFromFile(std::string fileName,
                  board &myBoard) {
  std::ifstream f(fileName);
  assert((bool)f && "Cannot open input file");
  std::string line;
  int index = 0;
  while (std::getline(f, line)) {
    std::stringstream sstream(line);
    std::string str;
    for (int i = 0; i < boardSize; i++) {
        cell curr;
        std::getline(sstream, str, ' ');
        int value = (int)atof(str.c_str()); 
        std::fill_n(curr.options, boardSize+1, 1);
        curr.options[0] = boardSize + 1;
        curr.val = value;
        myBoard.grid[index][i] = curr;
    }
    index++;
  }
  return true;
}

void printBoard(board toPrint) {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++){
            std::cout << toPrint.grid[i][j].val << " ";
        }
        std::cout << "\n";
    }
}


bool getZero(board current, int coords[2]) {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++){
            if (current.grid[i][j].val == 0) {
                coords[0] = i;
                coords[1] = j;
                return true;
            }
        }
    }
    return false;
}

int tryElimination(board myBoard, std::stack<board> boardStack) {
    int phase = 0;

    int singleOption = 0;

    int coords[2] = {-1,-1};

    if (getZero(myBoard, coords)) {
        cell current = myBoard.grid[coords[0]][coords[1]];
        if (current.options[0] == 1) {
            for (int i = 1; i <= boardSize; i++) {
                if (current.options[i] == 1) {
                    singleOption = i;
                    break;
                }
            }

            current.options[singleOption] = 0;
            current.options[0]--;
            myBoard.grid[coords[0]][coords[1]] = current;
            board newBoard = myBoard;
            cell newCell = current;
            newCell.val = singleOption;
            newBoard.grid[coords[0]][coords[1]] = newCell;
            boardStack.push(myBoard);
            boardStack.push(newBoard);
        } else {
            // cannot perform elimination, try next phase
            boardStack.push(myBoard);
            phase = 1;
        }
    } else {
        // no free cells, proceed to end board as it is solved
        boardStack.push(myBoard);
        phase = 1;
    }

    return phase;
}

int main(int argc, char** argv) {
    board initial;
    bool solved = false;
    std::stack<board> boardStack;
    std::string filename = "test2.txt";
    loadFromFile(filename, initial);
    boardStack.push(initial);

    int phase = 0;
    while (!boardStack.empty()) {
        //std::cout << boardStack.size() << "\n";
        board sudoku = boardStack.top();
        //if (boardStack.size() >= 27) {
        //printBoard(sudoku);
       // }
        boardStack.pop();
        int coords[2] = {-1,-1};

        if (phase == 0) {
            phase = tryElimination(sudoku, boardStack);
            std::cout << "did elimination" << " \n";
        }
        else if (getZero(sudoku, coords) && phase == 1) {
            std::cout << "got here" << " ";
            cell current = sudoku.grid[coords[0]][coords[1]];
            if (current.options[0] == 0) continue;
            for (int i = 1; i <= boardSize; i++) {
                if (current.options[i] == 1) {
                    current.options[i] = 0;
                    current.options[0]--;
                    sudoku.grid[coords[0]][coords[1]] = current;
                    if (noneInBox(sudoku,i,coords[0],coords[1]) &&
                        noneInRow(sudoku,i,coords[0]) &&
                        noneInCol(sudoku,i,coords[1])) {                           
                            board newBoard = sudoku;
                            cell newCell = current;
                            newCell.val = i;
                            newBoard.grid[coords[0]][coords[1]] = newCell;
                            boardStack.push(sudoku);
                            boardStack.push(newBoard);
                            break;
                        }
                    else {
                        boardStack.push(sudoku);
                        break;
                    }
                }
            }
        }

        else {
            solved = true;
            std::cout << "Solved!\n";
            printBoard(sudoku);
            break;
        }

        if (boardStack.empty()) printBoard(sudoku);
    }
    if (!solved) std::cout << "Couldn't solve. Consider checking this algorithm for errors.\n";
    return 0;
}
