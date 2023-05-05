// solver-thread.cpp
//
// compile:  g++ solver-thread.cpp
// executable:  ./a.out

// final version 0

#include <chrono>
#include <mutex>

#include "sudoku-board.cpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <cassert>
#include <stack>
#include <omp.h>

omp_lock_t stackLock;


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
        if (value == 0) {
            std::fill_n(curr.options, boardSize+1, 1);
            curr.options[0] = boardSize;
        }
        else {
            std::fill_n(curr.options, boardSize+1, 0);
            curr.options[0] = 0;
        }
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

void printBoardOptions(board toPrint) {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++){
            std::cout << "[";
            for (int k = 0; k < boardSize + 1; k++) {
            std::cout << toPrint.grid[i][j].options[k] << " ";
            }
            std::cout << "] ";
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

board reduceBoardOptions(board current) {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            int value = current.grid[i][j].val;
            if (value != 0) current = reduceOptions(current, value, i, j);
        }
    }
    return current;
}


void eliminate(board myBoard, std::stack<board> *boardStack, int coords[2]) {
    cell current = myBoard.grid[coords[0]][coords[1]];
    // only one option for cell, so fill in cell with that option
    for (int i = 1; i <= boardSize; i++) {
        if (current.options[i] == 1) {
            myBoard.grid[coords[0]][coords[1]].val = i;
            myBoard = reduceOptions(myBoard,i,coords[0],coords[1]);

            omp_set_lock (&stackLock);
            boardStack->push(myBoard);
            omp_unset_lock (&stackLock);

            break;
        }
    }
}

void eliminateLocal(board myBoard, std::stack<board> *localStack, int coords[2]) {
    cell current = myBoard.grid[coords[0]][coords[1]];
    // only one option for cell, so fill in cell with that option
    for (int i = 1; i <= boardSize; i++) {
        if (current.options[i] == 1) {
            myBoard.grid[coords[0]][coords[1]].val = i;
            myBoard = reduceOptions(myBoard,i,coords[0],coords[1]);

            localStack->push(myBoard);

            break;
        }
    }
}

void uniqueOption(board myBoard, std::stack<board> *boardStack, int coords[2]) {
    cell current = myBoard.grid[coords[0]][coords[1]];
    // see if any of the cell's options is unique to the options of it's row,
    // col, or box
    if (current.val > 0) return;
    
    for (int i = 1; i <= boardSize; i++) {
        // if possible option check if that option is unique to row, col or box
        if (current.options[i] == 1) {

            if (fullyUniqueOption(myBoard, i, coords[0], coords[1])) {
                myBoard.grid[coords[0]][coords[1]].val = i;
                myBoard.grid[coords[0]][coords[1]].options[0] = 0;
                boardStack->push(myBoard);
                return;
            }
        }
    }
}

int main(int argc, char** argv) {
    board initial;
    bool solved = false;
    std::stack<board> boardStack;
    omp_init_lock (&stackLock);
    std::string filename = "test-standard-medium.txt";
    loadFromFile(filename, initial);

    auto start = std::chrono::steady_clock::now();
    initial = reduceBoardOptions(initial);
    boardStack.push(initial);

    board sudoku;
    cell current;
    std::stack<board> localStack;
    std::stack<board> reverseStack;


    #pragma omp parallel private(sudoku, current, localStack, reverseStack) shared(boardStack)
    {
        while (!solved) {
            if (!solved) {
                omp_set_lock (&stackLock);
                if (localStack.empty() && !boardStack.empty() ) {
                    board sudoku = boardStack.top();
                    boardStack.pop();
                    omp_unset_lock (&stackLock);

                    int coords[2] = {-1,-1};
                    if (getZero(sudoku, coords)) {
                        cell current = sudoku.grid[coords[0]][coords[1]];
                        if (current.options[0] == 0) continue;
                        else if (current.options[0] == 1) {
                            eliminate(sudoku,&boardStack,coords);
                            continue;
                        }
                        for (int i = 1; i <= boardSize; i++) {
                            if (current.options[i] == 1) {
                                current.options[i] = 0;
                                current.options[0]--;
                                sudoku.grid[coords[0]][coords[1]] = current;
                                board newBoard = sudoku;
                                cell newCell = current;
                                newCell.val = i;
                                newBoard.grid[coords[0]][coords[1]] = newCell;
                                newBoard = reduceOptions(newBoard,i,coords[0],coords[1]);

                                localStack.push(sudoku);
                                localStack.push(newBoard);
                                break;

                            }
                        }
                        if (localStack.size() > 20) {
                            omp_set_lock (&stackLock);
                            while (!localStack.empty()) {
                                board item = localStack.top();
                                boardStack.push(item);
                                localStack.pop();
                            }
                            omp_unset_lock (&stackLock);
                        }
                    }
                    else {
                        solved = true;
                        #pragma omp flush(solved)
                        #pragma omp cancel parallel
                        auto end = std::chrono::steady_clock::now();
                        std::cout << "           Solved!\n";
                        std::cout << "================================\n";
                        std::cout << "Puzzle: " << filename << "\n";
                        std::cout << "Threadcount: " << omp_get_num_threads() << "\n";
                        std::chrono::duration<double> timeElapsed = end-start;
                        std::cout << "Time Elapsed (sec): [  " << timeElapsed.count() << "  ]\n";
                        std::cout << "================================\n";
                        printBoard(sudoku);
                        exit(0);
                    }
                }
                else if (!localStack.empty()) {
                    omp_unset_lock (&stackLock);
                    board sudoku = localStack.top();
                    localStack.pop();

                    int coords[2] = {-1,-1};
                    if (getZero(sudoku, coords)) {
                        cell current = sudoku.grid[coords[0]][coords[1]];
                        if (current.options[0] == 0) continue;
                        else if (current.options[0] == 1) {
                            eliminateLocal(sudoku,&localStack,coords);
                            continue;
                        }
                        for (int i = 1; i <= boardSize; i++) {
                            if (current.options[i] == 1) {
                                current.options[i] = 0;
                                current.options[0]--;
                                sudoku.grid[coords[0]][coords[1]] = current;
                                board newBoard = sudoku;
                                cell newCell = current;
                                newCell.val = i;
                                newBoard.grid[coords[0]][coords[1]] = newCell;
                                newBoard = reduceOptions(newBoard,i,coords[0],coords[1]);

                                localStack.push(sudoku);
                                localStack.push(newBoard);
                                break;
                            }
                        }
                        if (localStack.size() > 20) {
                            while (!localStack.empty()) {
                                board item = localStack.top();
                                reverseStack.push(item);
                                localStack.pop();
                            }

                            omp_set_lock (&stackLock);
                            while (!reverseStack.empty()) {
                                board item = reverseStack.top();
                                boardStack.push(item);
                                reverseStack.pop();
                            }
                            omp_unset_lock (&stackLock);
                        }
                    }
                    else {
                        solved = true;
                        #pragma omp flush(solved)
                        #pragma omp cancel parallel
                        auto end = std::chrono::steady_clock::now();
                        std::cout << "           Solved!\n";
                        std::cout << "================================\n";
                        std::cout << "Puzzle: " << filename << "\n";
                        std::cout << "Threadcount: " << omp_get_num_threads() << "\n";
                        std::chrono::duration<double> timeElapsed = end-start;
                        std::cout << "Time Elapsed (sec): [  " << timeElapsed.count() << "  ]\n";
                        std::cout << "================================\n";
                        printBoard(sudoku);
                        exit(0);
                    }
                }
                else omp_unset_lock (&stackLock);
            }
        }
        
    }

    if (!solved) std::cout << "Couldn't solve. Consider checking this algorithm for errors.\n";
    
    omp_destroy_lock (&stackLock);
    return 0;
}

