#include "Board.h"

Board::Board() {
    reset();//重新开始
}
//放棋子
bool Board::placePiece(int row, int col, int player) {
    if (grid[row][col] == 0) {
        grid[row][col] = player;
        return true;
    }
    return false;
}
//获取
int Board::getCell(int row, int col) const {
    return grid[row][col];
}
//重置函数
void Board::reset() {
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            grid[i][j] = 0;
}
//检查赢
bool Board::checkWin(int row, int col, int player) const {
    const int dx[4] = {1, 0, 1, 1};
    const int dy[4] = {0, 1, 1, -1};

    for (int d = 0; d < 4; ++d) {
        int count = 1;

        for (int dir = -1; dir <= 1; dir += 2) {
            int x = row + dx[d] * dir;
            int y = col + dy[d] * dir;//玩家每下一个就进行检查

            while (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && grid[x][y] == player) {
                ++count;
                x += dx[d] * dir;
                y += dy[d] * dir;
            }
        }

        if (count >= 5)
            return true;
    }
    return false;
}

