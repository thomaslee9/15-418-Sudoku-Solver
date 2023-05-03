// sudoku-board.cpp

#include "sudoku-board.h"
#include <cmath>
#include <iostream>


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

bool noneInBox(board myBoard, int value, int row, int col) {
               
    int bdim = sqrt(boardSize);
    int brow = getBoxRow(row);
    int bcol = getBoxCol(col);
 
    for (int i = bcol * bdim; i < (bcol + 1) * bdim; i++) {
        for (int j = brow * bdim; j < (brow + 1) * bdim; j++) {
            if (myBoard.grid[j][i].val == value) return false;
        }
    }
    return true;

}

bool noneInRow(board myBoard, int value, int row) {

    for (int i = 0; i < boardSize; i++) {
        if (myBoard.grid[row][i].val == value) return false;
    }
    return true;
}

bool noneInCol(board myBoard, int value, int col) {

    for (int i = 0; i < boardSize; i++) {
        if (myBoard.grid[i][col].val == value) return false;
    }
    return true;
}

board reduceOptions(board myBoard, int value, int row, int col) {
    for (int i = 0; i < boardSize; i++) {
        if (i != row) myBoard.grid[i][col].options[0]-= myBoard.grid[i][col].options[value];
        if (i != col) myBoard.grid[row][i].options[0]-= myBoard.grid[row][i].options[value];
        myBoard.grid[i][col].options[value] = 0;
        myBoard.grid[row][i].options[value] = 0;

    }
    int bdim = sqrt(boardSize);
    int brow = getBoxRow(row);
    int bcol = getBoxCol(col);
    for (int i = bcol * bdim; i < (bcol + 1) * bdim; i++) {
        for (int j = brow * bdim; j < (brow + 1) * bdim; j++) {
            if (i != col && j != row) myBoard.grid[j][i].options[0]-=myBoard.grid[j][i].options[value];
            myBoard.grid[j][i].options[value] = 0;
        }
    }
    return myBoard;
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
            if (myBoard.grid[j][i].val == value) count++;
            if (count > 1) return false;
        }
    }

    return true;
}

bool uniqueInRow(board myBoard, int value, int row) {
    int count = 0;

    if (row < 0 || row >= boardSize) return false;

    for (int i = 0; i < boardSize; i++) {
        if (myBoard.grid[row][i].val == value) count++;
        if (count > 1) return false;
    }

    return true;
}

bool uniqueInCol(board myBoard, int value, int col) {
    int count = 0;

    if (col < 0 || col >= boardSize) return false;

    for (int i = 0; i < boardSize; i++) {
        if (myBoard.grid[i][col].val == value) count++;
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

bool uniqueOptionRow(board myBoard, int value, int row) {
    int numOccur = 0;
    // check each cell in row
    for (int i = 0; i < boardSize; i++) {
        // return false if value has been seen as an option more than once
        if (numOccur > 1) return false;
        cell current = myBoard.grid[row][i];
        // return false if option has already been used in a different cell
        if (current.val == value) return false;
        // increment number of occurences of the option if found
        if ((current.val < 1) && (current.options[value] == 1)) {
            numOccur++;
        }
    }

    if (numOccur == 0) return false;

    return true;
}

bool uniqueOptionCol(board myBoard, int value, int col) {
    int numOccur = 0;
    // check each cell in column
    for (int i = 0; i < boardSize; i++) {
        // return false if value has been seen as an option more than once
        if (numOccur > 1) return false;
        cell current = myBoard.grid[i][col];
        // return false if option has already been used in a different cell
        if (current.val == value) return false;
        // increment number of occurences of the option if found
        if ((current.val < 1) && (current.options[value] == 1)) {
            numOccur++;
        }
    }

    if (numOccur == 0) return false;

    return true;
}

bool uniqueOptionBox(board myBoard, int value, int row, int col) {
    int numOccur = 0;
    
    int bdim = sqrt(boardSize);
    int brow = getBoxRow(row);
    int bcol = getBoxCol(col);
    // check each cell in box
    for (int i = bcol * bdim; i < (bcol + 1) * bdim; i++) {
        for (int j = brow * bdim; j < (brow + 1) * bdim; j++) {
            // return false if value has been seen as an option more than once
            if (numOccur > 1) return false;
            cell current = myBoard.grid[j][i];
            // return false if option has already been used in a different cell
            if (current.val == value) return false;
            // increment number of occurences of the option if found
            if ((current.val < 1) && (current.options[value] == 1)) {
                numOccur++;
            }
        }
    }

    if (numOccur == 0) return false;

    return true;
}

bool fullyUniqueOption(board myBoard, int value, int row, int col) {
    if (uniqueOptionRow(myBoard, value, row) && uniqueOptionCol(myBoard, value, col) && uniqueOptionBox(myBoard, value, row, col)) {
        return true;
    } else {
        return false;
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