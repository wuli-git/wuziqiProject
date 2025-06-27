#ifndef BOARD_H
#define BOARD_H

const int BOARD_SIZE = 15;

class Board {
private:
    int grid[BOARD_SIZE][BOARD_SIZE];

public:
    Board();
    bool placePiece(int row, int col, int player);
    int getCell(int row, int col) const;
    bool checkWin(int row, int col, int player) const;
    void reset();
};

#endif
