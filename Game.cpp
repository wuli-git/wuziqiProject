#include "Game.h"
#include <iostream>
#include <random>
// 静态成员初始化
sf::Clock Game::undoCooldown;

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
    vsAI(false),//是否AI对战模式
    AIDifficulty(1),
    AITurn(false),
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

    returnToMenuBg.setSize(sf::Vector2f(150, 40));
    returnToMenuBg.setFillColor(sf::Color(200, 200, 200, 200));
    returnToMenuBg.setPosition({ static_cast<float>(BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN) / 2 - 90, // 右侧留20像素边距
        static_cast <float>(10) });



    // 4. 初始化棋盘
    resetGame();
    // 加载音乐（放在其他资源加载之后）
    if (loadMusic()) {
        bgMusic.play();  // 如果加载成功立即播放
    }
    if (!loadPlaceSound()) {
        std::cerr << "加载落子音效成功！" << std::endl;
    }
    if (!loadVictorySound()) {
        std::cerr << "加载胜利音效失败" << std::endl;
    }
    if (window.isOpen()) {
        std::cout << "窗口创建成功！尺寸：" << window.getSize().x << "x" << window.getSize().y << std::endl;
    }
    else {
        std::cerr << "窗口创建失败！" << std::endl;
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

bool Game::loadPlaceSound() {
    if (!placeSound.loadFromFile("落子音效.ogg")) {
        if (!placeSound.loadFromFile("D:/desktop 2/落子音效.ogg")) {
            std::cerr << "无法加载落子音效文件！" << std::endl;
            return false;
        }
        else {
            std::cerr << "加载成功" << std::endl;
        }
        PlaceSound.setBuffer(placeSound);
        PlaceSound.setVolume(30.f);
        return true;
    }
}
bool Game::loadVictorySound() {
    // 缁熶竴鍙橀噺鍛藉悕锛屼娇鐢?victorySound 鍜?victorySoundBuffer
    if (!VictorySoundBuffer.loadFromFile("胜利音效.ogg")) {
        if (!VictorySoundBuffer.loadFromFile("胜利音效.ogg")) {
            std::cerr << "加载失败 " << std::endl;
            return false;
        }
        else {
            std::cerr << "加载成功从电脑" << std::endl;
            VictorySound.setBuffer(VictorySoundBuffer);
        }
    }
    else {
        std::cerr << "从项目加载成功" << std::endl;
    }

    VictorySound.setVolume(30.f);
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
        L". 按M键可以关闭背景音乐\n"
        L".按U键可以悔棋\n"
        L".按ESC或者点击返回可以回到主菜单"
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
    // 添加AI对战按钮文本
    aiText.setFont(gameFont);
    aiText.setString(L"AI对战");
    aiText.setCharacterSize(30);
    aiText.setFillColor(sf::Color::Black);
    aiText.setPosition(
        { static_cast<float>((BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - aiText.getLocalBounds().size.x) / 2),
        330.0f }
    );
    //返回主菜单文本
    returnToMenuText.setFont(gameFont);
    returnToMenuText.setString(L"返回主菜单 (B)");
    returnToMenuText.setCharacterSize(24);
    returnToMenuText.setFillColor(sf::Color::Black);
    returnToMenuText.setPosition(
        { static_cast<float>(BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN) / 2 - returnToMenuText.getLocalBounds().size.x / 2, // 文本向右调整10像素
        static_cast <float>(15) }
    );

}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    // 悔棋按钮悬停效果
    if (huiqiBg.getGlobalBounds().contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) })) {
        huiqiBg.setFillColor(sf::Color(180, 180, 180, 220));
    }
    else {
        huiqiBg.setFillColor(canUndo ? sf::Color(200, 200, 200, 200) : sf::Color(220, 220, 220, 180));
    }

    // 处理所有事件
    for (auto event = window.pollEvent(); event; event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::M)) {
            //auto keyEvent = event->getIf<sf::Event::KeyPressed>();

            // 音乐开关
            /*if (keyEvent->code == sf::Keyboard::Key::M) {*/
            if (isMusicPaused) {
                bgMusic.play();
            }
            else {
                bgMusic.pause();
            }
            isMusicPaused = !isMusicPaused;
        }

        // 重新开始
        if (gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::R)) {
            resetGame();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::B)) {
            returnToMainMenu();
        }

        if (event->is<sf::Event::MouseButtonPressed>()) {
            auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();

            if (mouseEvent->button == sf::Mouse::Button::Left && !gameOver) {
                PlaceSound.play();
                mousePos = sf::Mouse::getPosition(window);

                if (currentState == GameState::MainMenu) {
                    if (showingInfo) {
                        showingInfo = false;
                    }
                    else {
                        if (startText.getGlobalBounds().contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) })) {
                            currentState = GameState::Playing;
                            vsAI = false;
                        }
                        else if (exitText.getGlobalBounds().contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) })) {
                            window.close();
                        }
                        else if (infoText.getGlobalBounds().contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) })) {
                            showingInfo = true;
                        }
                        else if (aiText.getGlobalBounds().contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) })) {
                            currentState = GameState::AISelectDifficulty;
                            vsAI = true;
                        }

                    }
                }
                if (currentState != GameState::MainMenu && returnToMenuText.getGlobalBounds().contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) })) {
                    returnToMainMenu();
                }
                if (currentState == GameState::AISelectDifficulty) {
                    float centerX = (BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - 200) / 2;

                    if (mousePos.x >= centerX && mousePos.x <= centerX + 200) {
                        if (mousePos.y >= 200 && mousePos.y <= 250) {
                            AIDifficulty = 1;
                        }
                        else if (mousePos.y >= 270 && mousePos.y <= 320) {
                            AIDifficulty = 2;
                        }
                        else if (mousePos.y >= 340 && mousePos.y <= 390) {
                            AIDifficulty = 3;
                        }
                        else if (mousePos.y >= 420 && mousePos.y <= 470) {
                            currentState = GameState::Playing;
                            resetGame();
                            AITurn = (currentPlayer == 2);
                        }
                    }
                }
                if (currentState == GameState::Playing) {
                    if (huiqiText.getGlobalBounds().contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) })) {
                        requestUndo = true;
                    }
                    if (!vsAI) {
                        handleClick(mousePos.x, mousePos.y);
                    }
                    else if (vsAI && currentPlayer == 1) {
                        handleAIClick(mousePos.x, mousePos.y);
                    }
                }
            }
        }
        /*if (requestUndo && undoCooldown.getElapsedTime().asMilliseconds() > 300) {
            requestUndo = false;
            if (canUndo&& huiqiText.getGlobalBounds().contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) })) {
                undoMove();
            }
        }*/
    }

    // 处理悔棋按钮点击（移出事件循环）
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        mousePos = sf::Mouse::getPosition(window);
        if ((huiqiBg.getGlobalBounds().contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) }) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::U)) && canUndo) {
            if (undoCooldown.getElapsedTime().asMilliseconds() > 300) {
                undoCooldown.restart();
                //requestUndo = false;
                undoMove();
                //requestUndo = true;
            }
        }
    }
}

void Game::update() {
    if (currentState == GameState::Playing && !gameOver && fontLoaded) {
        if (vsAI) {
            statusText.setString(currentPlayer == 1 ? L"玩家回合(黑棋)" : L"AI思考中...");
        }
        else {
            statusText.setString(currentPlayer == 1 ? L"黑方回合" : L"白方回合");
        }
    }

    // 处理AI延迟走棋
    if (AIPendingMove && AIDelayClock.getElapsedTime().asMilliseconds() >= 500) {
        AIPendingMove = false;
        AITurnLogic();
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
        window.draw(aiText);
        if (showingInfo) {
            window.draw(infoBg);
            window.draw(infoContentText);
        }
    }

    else if (currentState == GameState::AISelectDifficulty) {
        // 渲染AI难度选择界面
        window.draw(infoBg);

        /*sf::Text difficultyTitle{gameFont};*/
        difficultyTitle.setFont(gameFont);
        difficultyTitle.setString(L"选择AI难度");
        difficultyTitle.setCharacterSize(40);
        difficultyTitle.setFillColor(sf::Color::Black);
        difficultyTitle.setPosition(
            { static_cast<float>((BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - difficultyTitle.getLocalBounds().size.x) / 2),
            120.0f }
        );
        window.draw(difficultyTitle);

        // 简单难度按钮
        sf::RectangleShape easyBg(sf::Vector2f(200, 50));
        easyBg.setFillColor(AIDifficulty == 1 ? sf::Color(150, 150, 255) : sf::Color(200, 200, 200));
        easyBg.setPosition(
            { static_cast<float>((BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - 200) / 2),
            200.0f }
        );
        window.draw(easyBg);


        easyText.setFont(gameFont);
        easyText.setString(L"简单");
        easyText.setCharacterSize(30);
        easyText.setFillColor(sf::Color::Black);
        easyText.setPosition(
            { static_cast<float>((BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - easyText.getLocalBounds().size.x) / 2),
            210.0f }
        );
        window.draw(easyText);

        // 中等难度按钮
        sf::RectangleShape mediumBg(sf::Vector2f(200, 50));
        mediumBg.setFillColor(AIDifficulty == 2 ? sf::Color(150, 150, 255) : sf::Color(200, 200, 200));
        mediumBg.setPosition(
            { static_cast<float>((BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - 200) / 2),
            270.0f }
        );
        window.draw(mediumBg);


        mediumText.setFont(gameFont);
        mediumText.setString(L"中等");
        mediumText.setCharacterSize(30);
        mediumText.setFillColor(sf::Color::Black);
        mediumText.setPosition(
            { static_cast<float>((BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - mediumText.getLocalBounds().size.x) / 2),
            280.0f }
        );
        window.draw(mediumText);

        // 困难难度按钮
        sf::RectangleShape hardBg(sf::Vector2f(200, 50));
        hardBg.setFillColor(AIDifficulty == 3 ? sf::Color(150, 150, 255) : sf::Color(200, 200, 200));
        hardBg.setPosition(
            { static_cast<float>((BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - 200) / 2),
            340.0f }
        );
        window.draw(hardBg);


        hardText.setFont(gameFont);
        hardText.setString(L"困难");
        hardText.setCharacterSize(30);
        hardText.setFillColor(sf::Color::Black);
        hardText.setPosition(
            { static_cast<float>((BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - hardText.getLocalBounds().size.x) / 2),
            350.0f }
        );
        window.draw(hardText);

        // 开始按钮
        sf::RectangleShape startBg(sf::Vector2f(200, 50));
        startBg.setFillColor(sf::Color(100, 255, 100));
        startBg.setPosition(
            { static_cast<float>((BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - 200) / 2),
            420.0f }
        );
        window.draw(startBg);


        startGameText.setFont(gameFont);
        startGameText.setString(L"开始游戏");
        startGameText.setCharacterSize(30);
        startGameText.setFillColor(sf::Color::Black);
        startGameText.setPosition(
            { static_cast<float>((BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN - startGameText.getLocalBounds().size.x) / 2),
            430.0f }
        );
        window.draw(startGameText);
    }
    else {
        drawPieces();
        if (fontLoaded) {
            window.draw(statusText);
        }
        else {
            // 显示资源加载失败提示
            std::cout << "资源加载失败" << std::endl;
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

    if (currentState != GameState::MainMenu) {
        window.draw(returnToMenuBg);
        window.draw(returnToMenuText);
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
                    static_cast<float>(WINDOW_MARGIN + i * CELL_SIZE - PIECE_RADIUS) }
                );
                piece.setFillColor(sf::Color::Black);
                window.draw(piece);
            }
            else if (board[i][j] == 2) {
                sf::CircleShape piece(PIECE_RADIUS);
                piece.setPosition(
                    { static_cast<float>(WINDOW_MARGIN + j * CELL_SIZE - PIECE_RADIUS),
                    static_cast<float>(WINDOW_MARGIN + i * CELL_SIZE - PIECE_RADIUS) }
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
    if (vsAI) {
        handleAIClick(x, y);
    }
    else {
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
            lastMove = { row, col };
            canUndo = true;
        }
        if (checkWin(row, col, currentPlayer)) {
            gameOver = true;
            winner = currentPlayer;
            VictorySound.play();
            if (fontLoaded) {
                statusText.setString(winner == 1 ? L"黑方胜利！按R重新开始" : L"白方胜利！按R重新开始");
            }
        }
        else {
            currentPlayer = 3 - currentPlayer;
        }
        // 如果是AI对战模式且游戏未结束，轮到AI走棋
        if (vsAI && !gameOver && currentPlayer == 2) {
            AITurnLogic();
        }
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
    AIPendingMove = false; // 重置AI走棋标志
    if (fontLoaded) {
        statusText.setString(vsAI ? L"玩家先手(黑棋)" : L"黑方先手");
    }
    moveHistory.clear();
    canUndo = false;
    AITurn = false;
    showReturnButton = false;

    // 如果是AI对战且AI执白，则AI先走
    if (vsAI && currentPlayer == 2) {
        AIPendingMove = true;
        AIDelayClock.restart();
    }
}

void Game::undoMove() {
    // 安全保护
    if (!canUndo || moveHistory.empty()) {
        canUndo = false;
        return;
    }

    // 播放音效
    PlaceSound.play();

    // 计算需要悔棋的步数
    int stepsToUndo = (vsAI == 1) ? 2 : 1;
    stepsToUndo = std::min(stepsToUndo, static_cast<int>(moveHistory.size()));

    // 执行悔棋
    for (int i = 0; i < stepsToUndo; ++i) {
        if (!moveHistory.empty()) {
            auto [row, col] = moveHistory.back();
            moveHistory.pop_back();
            board[row][col] = 0;
        }
    }

    // 完全重置状态
    if (vsAI)
        currentPlayer = 1;  // 总是回到玩家回合
    gameOver = false;
    winner = 0;
    AIPendingMove = false;
    AITurn = false;

    // 更新状态
    canUndo = !moveHistory.empty();
    if (fontLoaded) {
        statusText.setString(vsAI ? L"玩家回合(黑棋)" : L"黑方回合");
    }

    // 调试输出
    std::cout << "悔棋完成! 剩余步数: " << moveHistory.size()
        << ", 当前玩家: " << currentPlayer << std::endl;
}

void Game::AITurnLogic() {
    switch (AIDifficulty) {
    case 1: easyAIMove(); break;
    case 2: mediumAIMove(); break;
    case 3: hardAIMove(); break;
    }

    // 检查AI是否获胜
    if (checkWin(lastMove.first, lastMove.second, currentPlayer)) {
        gameOver = true;
        winner = currentPlayer;
        VictorySound.play();
        if (fontLoaded) {
            statusText.setString(winner == 1 ? L"黑方胜利！按R重新开始" : L"白方胜利！按R重新开始");
        }
    }
    else {
        currentPlayer = 3 - currentPlayer;
    }
}

// 简单AI：随机走棋
void Game::easyAIMove() {
    std::vector<std::pair<int, int>> emptyCells;

    // 收集所有空位置
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == 0) {
                emptyCells.emplace_back(i, j);
            }
        }
    }

    if (!emptyCells.empty()) {
        // 随机选择一个空位置
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, emptyCells.size() - 1);
        int index = dis(gen);

        auto [row, col] = emptyCells[index];
        board[row][col] = currentPlayer;
        lastMove = { row, col };
        moveHistory.emplace_back(row, col);
        canUndo = true;
    }
}

// 中等AI：有基本攻防意识
void Game::mediumAIMove() {
    // 1. 首先检查是否能直接获胜
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == 0) {
                board[i][j] = currentPlayer;
                if (checkWin(i, j, currentPlayer)) {
                    lastMove = { i, j };
                    moveHistory.emplace_back(i, j);
                    canUndo = true;
                    return;
                }
                board[i][j] = 0;
            }
        }
    }

    // 2. 检查是否需要阻止玩家获胜
    int opponent = 3 - currentPlayer;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == 0) {
                board[i][j] = opponent;
                if (checkWin(i, j, opponent)) {
                    board[i][j] = currentPlayer; // 阻止玩家
                    lastMove = { i, j };
                    moveHistory.emplace_back(i, j);
                    canUndo = true;
                    return;
                }
                board[i][j] = 0;
            }
        }
    }

    // 3. 如果没有紧急情况，随机走棋但倾向于中心位置
    std::vector<std::pair<int, int>> emptyCells;
    std::vector<int> weights;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == 0) {
                emptyCells.emplace_back(i, j);
                // 中心位置权重更高
                int distanceToCenter = abs(i - BOARD_SIZE / 2) + abs(j - BOARD_SIZE / 2);
                weights.push_back(BOARD_SIZE - distanceToCenter);
            }
        }
    }

    if (!emptyCells.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::discrete_distribution<> dis(weights.begin(), weights.end());
        int index = dis(gen);

        auto [row, col] = emptyCells[index];
        board[row][col] = currentPlayer;
        lastMove = { row, col };
        moveHistory.emplace_back(row, col);
        canUndo = true;
    }
}

// 困难AI：使用简单评估函数
void Game::hardAIMove() {
    auto [row, col] = findBestMove();
    board[row][col] = currentPlayer;
    lastMove = { row, col };
    moveHistory.emplace_back(row, col);
    canUndo = true;
}

std::pair<int, int> Game::findBestMove() {
    int bestScore = -10000;
    std::pair<int, int> bestMove = { -1, -1 };

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == 0) {
                // 尝试下这个位置
                board[i][j] = currentPlayer;

                // 如果能直接赢，就下这里
                if (checkWin(i, j, currentPlayer)) {
                    board[i][j] = 0;
                    return { i, j };
                }

                int score = evaluatePosition(currentPlayer);

                // 恢复空位置
                board[i][j] = 0;

                if (score > bestScore) {
                    bestScore = score;
                    bestMove = { i, j };
                }
            }
        }
    }

    // 如果没有找到好的走法（理论上不会发生，因为棋盘不会满）
    if (bestMove.first == -1) {
        // 随便找一个空位置
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == 0) {
                    return { i, j };
                }
            }
        }
    }

    return bestMove;
}

int Game::evaluatePosition(int player) {
    int score = 0;
    const int opponent = 3 - player;

    // 检查所有可能的连线
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == player) {
                // 水平方向
                int count = checkLine(i, j, 0, 1, player);
                score += getScoreByCount(count);

                // 垂直方向
                count = checkLine(i, j, 1, 0, player);
                score += getScoreByCount(count);

                // 对角线方向
                count = checkLine(i, j, 1, 1, player);
                score += getScoreByCount(count);

                // 反对角线方向
                count = checkLine(i, j, 1, -1, player);
                score += getScoreByCount(count);
            }
            else if (board[i][j] == opponent) {
                // 对对手的棋进行评估（负分）
                int count = checkLine(i, j, 0, 1, opponent);
                score -= getScoreByCount(count) * 2; // 更重视防守

                count = checkLine(i, j, 1, 0, opponent);
                score -= getScoreByCount(count) * 2;

                count = checkLine(i, j, 1, 1, opponent);
                score -= getScoreByCount(count) * 2;

                count = checkLine(i, j, 1, -1, opponent);
                score -= getScoreByCount(count) * 2;
            }
        }
    }

    // 中心位置加分
    for (int i = BOARD_SIZE / 2 - 2; i <= BOARD_SIZE / 2 + 2; ++i) {
        for (int j = BOARD_SIZE / 2 - 2; j <= BOARD_SIZE / 2 + 2; ++j) {
            if (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE && board[i][j] == player) {
                int distance = abs(i - BOARD_SIZE / 2) + abs(j - BOARD_SIZE / 2);
                score += (5 - distance) * 2;
            }
        }
    }

    return score;
}

int Game::checkLine(int row, int col, int dr, int dc, int player) {
    // 检查是否是连线的起点（避免重复计算）
    int prevRow = row - dr;
    int prevCol = col - dc;
    if (prevRow >= 0 && prevRow < BOARD_SIZE && prevCol >= 0 && prevCol < BOARD_SIZE
        && board[prevRow][prevCol] == player) {
        return 0; // 不是起点，跳过
    }

    // 正常计数逻辑（从当前点开始）
    int count = 1;
    for (int i = 1; i < 5; ++i) {
        int r = row + dr * i;
        int c = col + dc * i;
        if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE || board[r][c] != player) {
            break;
        }
        count++;
    }
    return count;
}
int Game::getScoreByCount(int count) {
    switch (count) {
    case 1: return 1;
    case 2: return 10;
    case 3: return 100;
    case 4: return 1000;
    case 5: return 10000; // 五连珠
    default: return 0;
    }
}

void Game::handleAIClick(int x, int y) {
    // 只有在玩家回合（currentPlayer == 1）且没有待处理的AI走棋时才处理点击
    if (currentPlayer != 1 || AIPendingMove) {
        return;
    }

    // 转换点击坐标到棋盘位置
    int col = (x - WINDOW_MARGIN + CELL_SIZE / 2) / CELL_SIZE;
    int row = (y - WINDOW_MARGIN + CELL_SIZE / 2) / CELL_SIZE;

    // 检查位置是否有效
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || board[row][col] != 0) {
        return;
    }

    // 播放落子音效
    PlaceSound.play();

    // 玩家落子
    board[row][col] = currentPlayer;
    lastMove = { row, col };
    moveHistory.emplace_back(row, col);
    canUndo = true;

    // 检查玩家是否获胜
    if (checkWin(row, col, currentPlayer)) {
        gameOver = true;
        winner = currentPlayer;
        VictorySound.play();
        if (fontLoaded) {
            statusText.setString(L"玩家胜利！按R重新开始");
        }
        return;
    }

    // 切换玩家（到AI）
    currentPlayer = 3 - currentPlayer;

    // 更新状态显示
    if (fontLoaded) {
        statusText.setString(L"AI思考中...");
    }

    // 设置AI延迟走棋标志
    AIPendingMove = true;
    AIDelayClock.restart(); // 重置计时器
}

void Game::returnToMainMenu() {
    currentState = GameState::MainMenu;
    resetGame();
    showingInfo = false;
    showReturnButton = false;
}