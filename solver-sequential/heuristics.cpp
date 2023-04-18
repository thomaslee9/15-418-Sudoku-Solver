// heuristics.cpp

#include "sudoku-board.h"
#include "solver-seq.cpp"
#include <stack>


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
            board newBoard = sudoku;
            cell newCell = current;
            newCell.val = singleOption;
            newBoard.grid[coords[0]][coords[1]] = newCell;
            boardStack.push(sudoku);
            boardStack.push(newBoard);
        } else {
            // cannot perform elimination, try next phase
            phase = 1;
        }
    } else {
        // no free cells, proceed to end board as it is solved
        phase = 1;
    }

    return phase;
}