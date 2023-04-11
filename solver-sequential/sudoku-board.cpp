// sudoku-board.cpp

#include <sudoku-board.h>

void resetOptions(cell grid[][16]) {
    int dim = 16;

    if (grid == NULL) return;

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            for (int k = 1; k < dim + 1; k++) {
                grid[i][j].options[k] = 1;
            }
        }
    }

}

void resetValues(cell grid[][16]) {
    int dim = 16;

    if (grid == NULL) return;

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            grid[i][j].val = 0;
        }
    }

}