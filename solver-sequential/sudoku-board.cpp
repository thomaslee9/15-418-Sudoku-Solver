// sudoku-board.cpp

#include "sudoku-board.h"
#include <cmath>

int boardSize = 16;

int getBoxRow(int row) {
    int bdim = sqrt(boardSize);

    int result = row / bdim;
    return result;
}

int getBoxCol(int col) {
    int bdim = sqrt(boardSize);

    int result = col / bdim;
    return result;
}

bool uniqueInBox(board myBoard, int value, int row, int col) {
    int bdim = sqrt(boardSize);
    int count = 0;

    int brow = getBoxRow(row);
    int bcol = getBoxCol(col);

    if (brow >= bdim || bcol >= bdim) return false;
    if (brow < 0 || bcol < 0) return false;

    for (int i = bcol * bdim; i < (bcol + 1) * bdim; i++) {
        for (int j = brow * bdim; j < (brow + 1) * bdim; j++) {
            if (myBoard.grid[i][j].val == value) count++;
            if (count > 1) return false;
        }
    }

    return true;
}

bool uniqueInRow(board myBoard, int value, int row) {
    int count = 0;

    if (row < 0 || row >= boardSize) return false;

    for (int i = 0; i < boardSize; i++) {
        if (myBoard.grid[i][row].val == value) count++;
        if (count > 1) return false;
    }

    return true;
}

bool uniqueInCol(board myBoard, int value, int col) {
    int count = 0;

    if (col < 0 || col >= boardSize) return false;

    for (int i = 0; i < boardSize; i++) {
        if (myBoard.grid[col][i].val == value) count++;
        if (count > 1) return false;
    }

    return true;
}

void setOption(cell box, int value, bool set) {

    if (value <= 0 || value > boardSize) return;
    if (box.val != 0) return;
    if (box.options[value] == 0 && set == false) return;
    if (box.options[value] == 1 && set == true) return;

    if (set == true) {
        box.options[0] += 1;
        box.options[value] = 1;
    } else {
        box.options[0] -= 1;
        box.options[value] = 0;
    }
    return;
}

void resetAllOptions(cell grid[][16]) {

    if (grid == NULL) return;

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            grid[i][j].options[0] = boardSize;
            for (int k = 1; k < boardSize + 1; k++) {
                grid[i][j].options[k] = 1;
            }
        }
    }

}

void resetValues(cell grid[][16]) {
    if (grid == NULL) return;

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            grid[i][j].val = 0;
        }
    }

}

bool isSolved(board myBoard) {
    int temp = 0;
    
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (myBoard.grid[i][j].val < 1 || myBoard.grid[i][j].val > boardSize) return false;
            temp = myBoard.grid[i][j].val;
            if (!uniqueInRow(myBoard, temp, j)) return false;
            if (!uniqueInCol(myBoard, temp, i)) return false;
            if (!uniqueInBox(myBoard, temp, i, j)) return false;
        }
    }

    return true;
}