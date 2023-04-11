// sudoku-board.h

struct board {
    bool solved;

    cell grid[16][16];
};

struct cell {
    int val;

    int options[17];
};


