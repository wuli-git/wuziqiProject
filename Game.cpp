#include "Game.h"
#include <stdexcept>
// 在包含 SFML 头文件之前定义此宏
//#define SFML_STATIC
#include <SFML/Graphics.hpp>
Game::Game() :
    window(
        sf::VideoMode(
            { BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN,
            BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN }
        ),
        "五子棋 - SFML 3.0",
        sf::Style::Default
    ),
    statusText(font) // SFML 3.0要求构造时绑定字体
{
    // 尝试加载字体
    if (!font.openFromFile("YeZiGongChangGuaiJiaoHei-2.ttf")) {
        // 回退到系统字体
        if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
            throw std::runtime_error("无法加载字体文件！");
        }
    }

    // 初始化文本
    statusText.setCharacterSize(30);
    statusText.setFillColor(sf::Color::Red);
    statusText.setPosition({ 50.0f, 10.0f });
    statusText.setString("黑方先手");

    resetGame();
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    for (auto event = window.pollEvent(); event; event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (!gameOver && event->is<sf::Event::MouseButtonPressed>() &&
            sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            const auto mousePos = sf::Mouse::getPosition(window);
            handleClick(mousePos.x, mousePos.y);
        }
        else if (gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::R)) {
            resetGame();
        }
    }
}

void Game::update() {
    // 更新游戏状态
    if (!gameOver) {
        statusText.setString(currentPlayer == 1 ? "黑方回合" : "白方回合");
    }
}

void Game::render() {
    window.clear(sf::Color(238, 238, 210)); // 浅黄色背景

    // 绘制棋盘网格
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
            cell.setPosition({static_cast<float>(WINDOW_MARGIN + j * CELL_SIZE), static_cast<float>(WINDOW_MARGIN + i * CELL_SIZE)
        });
            cell.setOutlineThickness(1);
            cell.setOutlineColor(sf::Color::Black);
            cell.setFillColor(sf::Color::Transparent);
            window.draw(cell);
        }
    }

    // 绘制棋子
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == 1) {
                sf::CircleShape piece(PIECE_RADIUS);
                piece.setPosition(
                    { static_cast<float>(WINDOW_MARGIN + j * CELL_SIZE - PIECE_RADIUS),static_cast<float>
                   ( WINDOW_MARGIN + i * CELL_SIZE - PIECE_RADIUS )}
                );
                piece.setFillColor(sf::Color::Black);
                window.draw(piece);
            }
            else if (board[i][j] == 2) {
                sf::CircleShape piece(PIECE_RADIUS);
                piece.setPosition(
                    { static_cast<float>(WINDOW_MARGIN + j * CELL_SIZE - PIECE_RADIUS),
                    static_cast<float>(WINDOW_MARGIN + i * CELL_SIZE - PIECE_RADIUS)}
                );
                piece.setFillColor(sf::Color::White);
                piece.setOutlineThickness(1);
                piece.setOutlineColor(sf::Color::Black);
                window.draw(piece);
            }
        }
    }

    // 绘制状态文本
    if (gameOver) {
        statusText.setString(winner == 1 ? "黑方胜利！按R重新开始" : "白方胜利！按R重新开始");
    }
    window.draw(statusText);

    window.display();
}

void Game::handleClick(int x, int y) {
    // 转换为棋盘坐标
    int col = (x - WINDOW_MARGIN + CELL_SIZE / 2) / CELL_SIZE;
    int row = (y - WINDOW_MARGIN + CELL_SIZE / 2) / CELL_SIZE;

    // 检查边界
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return;
    }

    // 检查位置是否为空
    if (board[row][col] != 0) {
        return;
    }

    // 放置棋子
    board[row][col] = currentPlayer;

    // 检查胜利
    if (checkWin(row, col, currentPlayer)) {
        gameOver = true;
        winner = currentPlayer;
    }
    else {
        // 切换玩家
        currentPlayer = 3 - currentPlayer;
    }
}

bool Game::checkWin(int row, int col, int player) {
    constexpr std::array<std::pair<int, int>, 4> directions = {
        std::make_pair(1, 0),   // 横向
        std::make_pair(0, 1),   // 纵向
        std::make_pair(1, 1),   // 斜向 ↘
        std::make_pair(1, -1)   // 斜向 ↙
    };

    for (const auto& [dr, dc] : directions) {
        if (countDirection(row, col, player, dr, dc) +
            countDirection(row, col, player, -dr, -dc) + 1 >= 5) {
            return true;
        }
    }
    return false;
}

int Game::countDirection(int row, int col, int player, int dr, int dc) {
    int count = 0;
    for (int i = 1; i <= 4; ++i) {
        int r = row + dr * i;
        int c = col + dc * i;
        if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE || board[r][c] != player) {
            break;
        }
        count++;
    }
    return count;
}

void Game::resetGame() {
    // 清空棋盘
    for (auto& row : board) {
        row.fill(0);
    }
    currentPlayer = 1;
    gameOver = false;
    winner = 0;
    statusText.setString("黑方先手");
}