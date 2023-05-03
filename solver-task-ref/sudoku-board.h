// sudoku-board.h

struct cell {
    int val;

    int options[17];
};

struct board {
    bool solved;

    cell grid[16][16];
};
