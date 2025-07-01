#include "Game.h"
#include <iostream>

Game::Game() :
    window(
        sf::VideoMode(
            { BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN,
            BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN }
        ),
        L"五子棋",
        sf::Style::Default
    ),
    currentState(GameState::MainMenu),
    currentPlayer(1),
    gameOver(false),
    winner(0)
{
    // 1. 加载字体
    if (!gameFont.openFromFile("simhei.ttf")) {
        if (!gameFont.openFromFile("C:/Windows/Fonts/simhei.ttf")) {
            std::cerr << "错误：无法加载字体文件！" << std::endl;
            fontLoaded = false;
        }
        else {
            fontLoaded = true;
        }
    }
    else {
        fontLoaded = true;
    }

    // 2. 初始化文本对象
    if (fontLoaded) {
        initTextObjects();
    }

    // 3. 初始化按钮背景
    buttonBg.setSize(sf::Vector2f(300, 200));
    buttonBg.setFillColor(sf::Color(255, 255, 255, 200));
    buttonBg.setPosition(
        { static_cast<float>((BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - 300) / 2),
        150.0f }
    );

    infoBg.setSize(sf::Vector2f(500, 300));
    infoBg.setFillColor(sf::Color(255, 255, 255, 220));
    infoBg.setPosition(
        { static_cast<float>((BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - 500) / 2),
        100.0f }
    );

    huiqiBg.setSize(sf::Vector2f(120, 40));
    huiqiBg.setFillColor(sf::Color(200, 200, 200, 200));
    huiqiBg.setPosition(
        { static_cast<float>(BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - 120 - 20),
        static_cast<float>(10) }
    );

    
    // 4. 初始化棋盘
    resetGame();
    // 加载音乐（放在其他资源加载之后）
    if (loadMusic()) {
        bgMusic.play();  // 如果加载成功立即播放
    }
}

// Game.cpp
bool Game::loadMusic() {
    if (!bgMusic.openFromFile("五子棋背景音乐.ogg")) {  // 推荐使用OGG格式
        std::cerr << "无法加载背景音乐文件！" << std::endl;
        return false;
    }
    bgMusic.setLooping(true);      // 设置循环播放
    bgMusic.setVolume(30.f);    // 音量设置(0-100)
    return true;
}

void Game::initTextObjects() {
    // 状态文本
    statusText.setFont(gameFont);
    statusText.setCharacterSize(30);
    statusText.setFillColor(sf::Color::Red);
    statusText.setPosition({ 50.f, 10.f });

    // 标题文本
    titleText.setFont(gameFont);
    titleText.setString(L"五子棋");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::Black);
    titleText.setPosition(
        { static_cast<float>((BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - titleText.getLocalBounds().size.x) / 2),
        50.0f }
    );
    // 开始按钮文本
    startText.setFont(gameFont);
    startText.setString(L"开始游戏");
    startText.setCharacterSize(30);
    startText.setFillColor(sf::Color::Black);
    startText.setPosition(
        { static_cast<float> ((BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - startText.getLocalBounds().size.x) / 2),
        180.0f }
    );

    // 退出按钮文本
    exitText.setFont(gameFont);
    exitText.setString(L"退出游戏");
    exitText.setCharacterSize(30);
    exitText.setFillColor(sf::Color::Black);
    exitText.setPosition(
        { static_cast<float>((BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - exitText.getLocalBounds().size.x) / 2),
        230.0f }
    );

    // 介绍按钮文本
    infoText.setFont(gameFont);
    infoText.setString(L"游戏介绍");
    infoText.setCharacterSize(30);
    infoText.setFillColor(sf::Color::Black);
    infoText.setPosition(
        { static_cast<float>((BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - infoText.getLocalBounds().size.x) / 2),
        280.0f }
    );

    // 介绍内容文本
    infoContentText.setFont(gameFont);
    infoContentText.setString(
        L"游戏规则:\n"
        L"1. 黑白双方轮流落子\n"
        L"2. 先在横、竖、斜方向连成五子者胜\n\n"
        
        L"操作说明:\n"
        L"• 鼠标点击放置棋子\n"
        L"• 游戏结束后按R重新开始"
        L". 按R键可以重新开始\n"
        L". 按M键可以关闭背景音乐"
    );
    infoContentText.setCharacterSize(24);
    infoContentText.setFillColor(sf::Color::Black);
    infoContentText.setPosition(
        { static_cast<float>((BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - 450) / 2),
        120.0f }
    );
    // 悔棋按钮文本
    huiqiText.setFont(gameFont);
    huiqiText.setString(L"悔棋 (U)");
    huiqiText.setCharacterSize(24);
    huiqiText.setFillColor(sf::Color::Black);
    huiqiText.setPosition(
        { static_cast<float>(BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - 120 - 20),
        static_cast<float>(10) });

}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    //sf::Event event;
    for (auto event = window.pollEvent(); event; event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::M)) {
            if (isMusicPaused) {
                bgMusic.play();  // 从暂停位置继续
            }
            else {
                bgMusic.pause(); // 暂停音乐
            }
            isMusicPaused = !isMusicPaused; // 切换状态
        }
        if (!gameOver && event->is<sf::Event::MouseButtonPressed>() &&
            sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            if (currentState == GameState::MainMenu) {
                if (showingInfo) {
                    showingInfo = false;
                }
                else {
                    if (startText.getGlobalBounds().contains({ static_cast<float>( mousePos.x), static_cast<float>(mousePos.y) })) {
                        currentState = GameState::Playing;
                    }
                    else if (exitText.getGlobalBounds().contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y )})) {
                        window.close();
                    }
                    else if (infoText.getGlobalBounds().contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) })) {
                        showingInfo = true;
                    }
                }
            }
            else if (currentState == GameState::Playing && !gameOver) {
                handleClick(mousePos.x, mousePos.y);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::U)) {
            if (canUndo) {
                undoMove();
            }
        }
        //sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        //if (huiqiBg.getGlobalBounds().contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) })) {
        //    huiqiBg.setFillColor(sf::Color(180, 180, 180, 220)); // 悬停时变深
        //}
        //else {
        //    huiqiBg.setFillColor(sf::Color(200, 200, 200, 200)); // 恢复正常
        //}
        // 鼠标点击悔棋按钮
        if (!gameOver && event->is<sf::Event::MouseButtonPressed>() &&
            sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (huiqiBg.getGlobalBounds().contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) }) && canUndo) {
                undoMove();
            }
        }
        if (gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::R)) {
            resetGame();
        }
    }
}

void Game::update() {
    if (currentState == GameState::Playing && !gameOver && fontLoaded) {
        statusText.setString(currentPlayer == 1 ? L"黑方回合" : L"白方回合");
    }
}

void Game::render() {
    window.clear(sf::Color(238, 238, 210));

    // 绘制棋盘背景
    drawChessboard();

    if (currentState == GameState::MainMenu) {
        window.draw(buttonBg);
        window.draw(titleText);
        window.draw(startText);
        window.draw(exitText);
        window.draw(infoText);

        if (showingInfo) {
            window.draw(infoBg);
            window.draw(infoContentText);
        }
    }
    else {
        drawPieces();
        if (fontLoaded) {
            window.draw(statusText);
        }
    }
    if (currentState == GameState::Playing) {
        // 绘制悔棋按钮
        window.draw(huiqiBg);
        window.draw(huiqiText);

        // 根据可悔棋状态调整颜色
        if (!canUndo) {
            huiqiBg.setFillColor(sf::Color(150, 150, 150, 150));
            huiqiText.setFillColor(sf::Color(100, 100, 100));
        }
        else {
            huiqiBg.setFillColor(sf::Color(200, 200, 200, 200));
            huiqiText.setFillColor(sf::Color::Black);
        }
    }
    window.display();
}

void Game::drawChessboard() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
            cell.setPosition(
                { static_cast<float>(WINDOW_MARGIN + j * CELL_SIZE),
                static_cast<float>(WINDOW_MARGIN + i * CELL_SIZE) }
            );
            cell.setOutlineThickness(1);
            cell.setOutlineColor(sf::Color::Black);
            cell.setFillColor(sf::Color::Transparent);
            window.draw(cell);
        }
    }
}

void Game::drawPieces() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == 1) {
                sf::CircleShape piece(PIECE_RADIUS);
                piece.setPosition(
                    { static_cast<float>(WINDOW_MARGIN + j * CELL_SIZE - PIECE_RADIUS),
                    static_cast<float>(WINDOW_MARGIN + i * CELL_SIZE - PIECE_RADIUS)}
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
}

void Game::handleClick(int x, int y) {
    int col = (x - WINDOW_MARGIN + CELL_SIZE / 2) / CELL_SIZE;
    int row = (y - WINDOW_MARGIN + CELL_SIZE / 2) / CELL_SIZE;

    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || board[row][col] != 0) {
        return;
    }

    board[row][col] = currentPlayer;
    
    if (board[row][col] != 0)
        {
            // 记录落子位置
            moveHistory.emplace_back(row, col);
            canUndo = true;
        }
    if (checkWin(row, col, currentPlayer)) {
        gameOver = true;
        winner = currentPlayer;
        if (fontLoaded) {
            statusText.setString(winner == 1 ? L"黑方胜利！按R重新开始" : L"白方胜利！按R重新开始");
        }
    }
    else {
        currentPlayer = 3 - currentPlayer;
    }
}

bool Game::checkWin(int row, int col, int player) {
    const std::pair<int, int> directions[] = {
        {1, 0}, {0, 1}, {1, 1}, {1, -1}
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
    for (auto& row : board) {
        row.fill(0);
    }
    currentPlayer = 1;
    gameOver = false;
    winner = 0;
    if (fontLoaded) {
        statusText.setString(L"黑方先手");
    }
    moveHistory.clear();
    canUndo = false;
}

void Game::undoMove() {
    if (moveHistory.empty()) {
        canUndo = false;
        return;
    }

    auto [row, col] = moveHistory.back();
    moveHistory.pop_back();

    board[row][col] = 0;            // 清空该位置
    currentPlayer = 3 - currentPlayer; // 切换回上一个玩家
    canUndo = !moveHistory.empty();  // 检查是否还能继续悔棋
}