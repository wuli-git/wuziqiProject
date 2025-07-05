#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 
#include <array>
#include <future>  // 添加这行
#include <atomic>  // 用于原子操作

class Game {
public:
    Game();
    void run();
    enum class LoadingState {
        NotStarted,
        LoadingFont,
        LoadingSounds,
        LoadingMusic,
        Finished,
        Failed
    };
private:
    // 游戏常量
    static constexpr int BOARD_SIZE = 15;
    static constexpr int CELL_SIZE = 40;
    static constexpr int WINDOW_MARGIN = 50;
    static constexpr float PIECE_RADIUS = CELL_SIZE * 0.4f;

    // 游戏状态
    enum class GameState { MainMenu, Playing, GameOver ,AISelectDifficulty};
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
    sf::Text aiText{ gameFont };
    sf::Text difficultyTitle{ gameFont };
    sf::Text easyText{gameFont};
    sf::Text mediumText{gameFont};
    sf::Text hardText{gameFont};
    sf::Text startGameText{gameFont};
    bool showingInfo = false;

    LoadingState loadingState;
    float loadingProgress;
    sf::Text loadingText{gameFont};
    sf::RectangleShape progressBarBg;
    sf::RectangleShape progressBar;
    std::future<bool> loadingTask;
    std::atomic<bool> resourcesLoaded{ false };

    // 添加加载方法
   /* void initLoadingScreen();
    bool loadAllResources();
    std::future<bool> loadResourcesAsync();
    void updateLoadingScreen();
    void renderLoadingScreen(); */

    // 按钮背景
    sf::RectangleShape buttonBg;
    sf::RectangleShape infoBg;
    sf::RectangleShape huiqiBg;

    //悔棋模块
    std::vector<std::pair<int, int>> moveHistory; // 落子历史记录,记录的是位置
    bool canUndo = false;            // 是否可以悔棋
    bool requestUndo = false;
    static sf::Clock undoCooldown;
    //背景音乐
    sf::Music bgMusic;  // 背景音乐对象
    bool loadMusic();   // 音乐加载方法
    bool isMusicPaused = false;
    bool loadPlaceSound();
    sf::SoundBuffer placeSound;
    sf::Sound PlaceSound{ placeSound };
    //sf::Music 
    sf::SoundBuffer VictorySoundBuffer;
    sf::Sound VictorySound{ VictorySoundBuffer };
    bool loadVictorySound();

    //AI
    bool vsAI; // 是否是对战AI模式
    int AIDifficulty; // 1=简单, 2=中等, 3=困难
    bool AITurn; // 是否是AI的回合
    std::pair<int, int> lastMove;

    //fan回主菜单
    // 添加返回主菜单按钮
    sf::RectangleShape returnToMenuBg;
    sf::Text returnToMenuText{gameFont};
    bool showReturnButton; // 是否显示返回按钮

    void handleAIClick(int x, int y); // 处理AI对战模式的点击
    void AITurnLogic(); // AI走棋逻辑
    void easyAIMove(); // 简单AI走法
    void mediumAIMove(); // 中等AI走法
    void hardAIMove(); // 困难AI走法
    bool AIPendingMove;      // 标记是否有AI走棋待处理
    sf::Clock AIDelayClock;  // 用于计算延迟时间
    sf::Vector2i pendingAIMovePos; // 存储待处理的AI走棋位置
    std::pair<int, int> findBestMove(); // 寻找最佳走法(用于困难AI)
    int evaluatePosition(int player); // 评估当前局面(用于困难AI)
    int checkLine(int row, int col, int dr, int dc, int player); // 检查特定方向的连线情况
    int getScoreByCount(int count);
    
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
    void returnToMainMenu();
};

#endif // GAME_H