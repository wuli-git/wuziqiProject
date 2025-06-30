#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 
#include <array>

class Game {
public:
    Game();
    void run();

private:
    // 游戏常量
    static constexpr int BOARD_SIZE = 15;
    static constexpr int CELL_SIZE = 40;
    static constexpr int WINDOW_MARGIN = 50;
    static constexpr float PIECE_RADIUS = CELL_SIZE * 0.4f;

    // 游戏状态
    enum class GameState { MainMenu, Playing, GameOver };
    GameState currentState;

    // SFML 对象
    sf::RenderWindow window;
    sf::Font gameFont;
    bool fontLoaded = false;

    // 游戏数据
    std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE> board;
    int currentPlayer;
    bool gameOver;
    int winner;

    // 文本对象
    sf::Text statusText{ gameFont };  // 使用成员初始化
    sf::Text titleText{ gameFont };
    sf::Text startText{ gameFont };
    sf::Text exitText{ gameFont };
    sf::Text infoText{ gameFont };
    sf::Text infoContentText{ gameFont };
    bool showingInfo = false;

    // 按钮背景
    sf::RectangleShape buttonBg;
    sf::RectangleShape infoBg;

    //背景音乐
    sf::Music bgMusic;  // 背景音乐对象
    bool loadMusic();   // 音乐加载方法
    bool isMusicPaused = false;

    // 私有方法
    void initTextObjects();
    void processEvents();
    void update();
    void render();
    void drawChessboard();
    void drawPieces();
    void handleClick(int x, int y);
    bool checkWin(int row, int col, int player);
    int countDirection(int row, int col, int player, int dr, int dc);
    void resetGame();
};

#endif // GAME_H