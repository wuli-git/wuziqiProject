#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 
#include <array>
enum class Difficulty {
    Easy,   // 简单：搜索深度1
    Medium, // 中等：搜索深度3
    Hard    // 困难：搜索深度5
};

// 棋型评分（分数越高优先级越高）
const int SCORE_FIVE = 100000;    // 五连
const int SCORE_FOUR = 10000;     // 活四/冲四
const int SCORE_THREE = 1000;     // 活三/冲三
const int SCORE_TWO = 100;        // 活二/冲二
const int SCORE_ONE = 10;         // 活一/冲一

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
    sf::Text huiqiText{ gameFont };
    bool showingInfo = false;

    // 按钮背景
    sf::RectangleShape buttonBg;
    sf::RectangleShape infoBg;
    sf::RectangleShape huiqiBg;

    //悔棋模块
    std::vector<std::pair<int, int>> moveHistory; // 落子历史记录,记录的是位置
    bool canUndo = false;            // 是否可以悔棋

    //背景音乐
    sf::Music bgMusic;  // 背景音乐对象
    bool loadMusic();   // 音乐加载方法
    bool isMusicPaused = false;
    bool loadPlaceSound();
    sf::SoundBuffer placeSound;
    sf::Sound PlaceSound{ placeSound };
    //sf::Music 

    //AI部分
    Difficulty currentDifficulty;  // 当前难度
    bool isAIThinking;             // AI思考中标记
    sf::Text thinkingText{gameFont};         // "思考中"提示文本
    
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
    void undoMove();//悔棋函数
};

#endif // GAME_H