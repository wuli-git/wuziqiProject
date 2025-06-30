#include "Game.h"
#include <stdexcept>
// 在包含 SFML 头文件之前定义此宏
//#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include <iostream>
// 注意：确保 Game 类中已声明成员变量：
// sf::RenderWindow window;
// sf::Font font;
// sf::Text statusText;  // 作为成员变量，而非局部变量

Game::Game() :
    // 初始化窗口（尺寸、标题、样式）
    window(
        sf::VideoMode(
            { BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN,
              BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN }
        ),
        L"五子棋 - SFML 3.0",
        sf::Style::Default
    ),
    // 初始化文本对象（绑定字体，SFML 3.0 要求）
    statusText(font)
{
    // 1. 加载字体（优先自定义字体，失败则尝试备选路径）
    bool fontLoaded = false;
    // 尝试当前目录的字体文件
    if (font.openFromFile("simhei.ttf")) {
        fontLoaded = true;
        std::cout << "当前目录字体加载成功" << std::endl;
    }
    // 尝试备选路径（桌面路径）
    else if (font.openFromFile("C:/Windows/Fonts/simhei.ttf")) {
        fontLoaded = true;
        std::cout << "桌面路径字体加载成功" << std::endl;
    }
    // 若仍失败，提示错误（可考虑进一步回退到系统字体）
    else {
        std::cerr << "警告：所有字体路径加载失败！文字可能无法显示" << std::endl;
        // 可选：尝试系统字体（如 Arial）
        // if (font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        //     fontLoaded = true;
        //     std::cout << "系统字体加载成功" << std::endl;
        // }
    }

    // 2. 初始化文本对象（使用成员变量 statusText，而非局部变量）
    if (fontLoaded) {
        statusText.setCharacterSize(30);         // 字体大小
        statusText.setFillColor(sf::Color::Red); // 文字颜色
        statusText.setPosition({ 50.0f, 10.0f });  // 位置（窗口左上角偏移）
        
    }
    else {
        // 字体加载失败时，至少保证程序不崩溃（可设置默认文本，但可能显示乱码）
        statusText.setString("字体加载失败");
    }

    // 3. 重置游戏状态（初始化棋盘、玩家等）
    resetGame();
}

void Game::run() {
    while (window.isOpen()) {
        //window.draw(statusText);
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
            handleClick(mousePos.x, mousePos.y);//处理点击
        }
        else if (gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::R)) {
            resetGame();//游戏结束重新开始
        }
    }
}

void Game::update() {
    // 只在游戏进行中更新回合文本
    if (!gameOver && currentPlayer != 0) {  // 添加currentPlayer检查
        statusText.setString(currentPlayer == 1 ? L"黑方回合" : L"白方回合");
    }
    // 游戏结束或初始状态保持原文本不变
}
//渲染游戏
void Game::render() {
    //window.draw(statusText);
    //window.display();
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
        statusText.setString(winner == 1 ? L"黑方胜利！按R重新开始" : L"白方胜利！按R重新开始");
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
    statusText.setString(L"黑方先手");
}