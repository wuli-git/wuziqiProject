#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <optional>

class Game {
public:
    Game();
    void run();

private:
    static constexpr int BOARD_SIZE = 15;
    static constexpr int CELL_SIZE = 40;
    static constexpr int WINDOW_MARGIN = 50;
    static constexpr int PIECE_RADIUS = CELL_SIZE / 2 - 2;

    sf::RenderWindow window;
    sf::Font font;
    sf::Text statusText;

    std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE> board{};
    int currentPlayer = 1; // 1=黑子，2=白子
    bool gameOver = false;
    int winner = 0;

    void processEvents();
    void update();
    void render();
    void handleClick(int x, int y);
    bool checkWin(int row, int col, int player);
    int countDirection(int row, int col, int player, int dr, int dc);
    void resetGame();
};