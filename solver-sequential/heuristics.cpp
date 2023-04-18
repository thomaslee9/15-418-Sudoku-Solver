// heuristics.cpp

void eliminate(board myBoard, std::stack<board> *boardStack, int coords[2]) {
    cell current = myBoard.grid[coords[0]][coords[1]];
    // only one option for cell, so fill in cell with that option
    for (int i = 1; i <= boardSize; i++) {
        if (current.options[i] == 1) {
            myBoard.grid[coords[0]][coords[1]].val = i;
            myBoard = reduceOptions(myBoard,i,coords[0],coords[1]);
            boardStack->push(myBoard);
            break;
        }
    }
}

void uniqueOption(board myBoard, std::stack<board> *boardStack, int coords[2]) {
    cell current = myBoard.grid[coords[0]][coords[1]];
    // see if any of the cell's options is unique to the options of it's row,
    // col, or box
    int numOptions = current.options[0];
    int numSeen = 0;
    for (int i = 1; i <= boardSize; i++) {
        // checked all options for uniqueness to row, col, or box
        if (numSeen == numOptions) break;
        // if possible option check if that option is unique to row, col or box
        if (current.options[i] == 1) {
            numSeen++;

            if (fullyUniqueOption(myBoard, i, coords[0], coords[1])) {
                myBoard.grid[coords[0]][coords[1]].val = i;
                myBoard.grid[coords[0]][coords[1]].options[0] = 0;
            }
        }
    }
    boardStack->push(myBoard);
}

bool fullyUniqueOption(board myBoard, int value, int row, int col) {
    if (uniqueOptionRow(myBoard, value, row) && uniqueOptionCol(myBoard, value, col) && uniqueOptionBox(myBoard, value, row, col)) {
        return true;
    } else {
        return false;
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