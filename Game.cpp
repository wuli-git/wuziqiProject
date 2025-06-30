#include "Game.h"
#include <stdexcept>
// �ڰ��� SFML ͷ�ļ�֮ǰ����˺�
//#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include <iostream>
// ע�⣺ȷ�� Game ������������Ա������
// sf::RenderWindow window;
// sf::Font font;
// sf::Text statusText;  // ��Ϊ��Ա���������Ǿֲ�����

Game::Game() :
    // ��ʼ�����ڣ��ߴ硢���⡢��ʽ��
    window(
        sf::VideoMode(
            { BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN,
              BOARD_SIZE * CELL_SIZE + 2 * WINDOW_MARGIN }
        ),
        L"������ - SFML 3.0",
        sf::Style::Default
    ),
    // ��ʼ���ı����󣨰����壬SFML 3.0 Ҫ��
    statusText(font)
{
    // 1. �������壨�����Զ������壬ʧ�����Ա�ѡ·����
    bool fontLoaded = false;
    // ���Ե�ǰĿ¼�������ļ�
    if (font.openFromFile("simhei.ttf")) {
        fontLoaded = true;
        std::cout << "��ǰĿ¼������سɹ�" << std::endl;
    }
    // ���Ա�ѡ·��������·����
    else if (font.openFromFile("C:/Windows/Fonts/simhei.ttf")) {
        fontLoaded = true;
        std::cout << "����·��������سɹ�" << std::endl;
    }
    // ����ʧ�ܣ���ʾ���󣨿ɿ��ǽ�һ�����˵�ϵͳ���壩
    else {
        std::cerr << "���棺��������·������ʧ�ܣ����ֿ����޷���ʾ" << std::endl;
        // ��ѡ������ϵͳ���壨�� Arial��
        // if (font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        //     fontLoaded = true;
        //     std::cout << "ϵͳ������سɹ�" << std::endl;
        // }
    }

    // 2. ��ʼ���ı�����ʹ�ó�Ա���� statusText�����Ǿֲ�������
    if (fontLoaded) {
        statusText.setCharacterSize(30);         // �����С
        statusText.setFillColor(sf::Color::Red); // ������ɫ
        statusText.setPosition({ 50.0f, 10.0f });  // λ�ã��������Ͻ�ƫ�ƣ�
        
    }
    else {
        // �������ʧ��ʱ�����ٱ�֤���򲻱�����������Ĭ���ı�����������ʾ���룩
        statusText.setString("�������ʧ��");
    }

    // 3. ������Ϸ״̬����ʼ�����̡���ҵȣ�
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
            handleClick(mousePos.x, mousePos.y);//������
        }
        else if (gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::R)) {
            resetGame();//��Ϸ�������¿�ʼ
        }
    }
}

void Game::update() {
    // ֻ����Ϸ�����и��»غ��ı�
    if (!gameOver && currentPlayer != 0) {  // ���currentPlayer���
        statusText.setString(currentPlayer == 1 ? L"�ڷ��غ�" : L"�׷��غ�");
    }
    // ��Ϸ�������ʼ״̬����ԭ�ı�����
}
//��Ⱦ��Ϸ
void Game::render() {
    //window.draw(statusText);
    //window.display();
    window.clear(sf::Color(238, 238, 210)); // ǳ��ɫ����

    // ������������
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

    // ��������
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

    // ����״̬�ı�
    if (gameOver) {
        statusText.setString(winner == 1 ? L"�ڷ�ʤ������R���¿�ʼ" : L"�׷�ʤ������R���¿�ʼ");
    }
    window.draw(statusText);

    window.display();
}

void Game::handleClick(int x, int y) {
    // ת��Ϊ��������
    int col = (x - WINDOW_MARGIN + CELL_SIZE / 2) / CELL_SIZE;
    int row = (y - WINDOW_MARGIN + CELL_SIZE / 2) / CELL_SIZE;

    // ���߽�
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return;
    }

    // ���λ���Ƿ�Ϊ��
    if (board[row][col] != 0) {
        return;
    }

    // ��������
    board[row][col] = currentPlayer;

    // ���ʤ��
    if (checkWin(row, col, currentPlayer)) {
        gameOver = true;
        winner = currentPlayer;
    }
    else {
        // �л����
        currentPlayer = 3 - currentPlayer;
    }
}

bool Game::checkWin(int row, int col, int player) {
    constexpr std::array<std::pair<int, int>, 4> directions = {
        std::make_pair(1, 0),   // ����
        std::make_pair(0, 1),   // ����
        std::make_pair(1, 1),   // б�� �K
        std::make_pair(1, -1)   // б�� �L
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
    // �������
    for (auto& row : board) {
        row.fill(0);
    }
    currentPlayer = 1;
    gameOver = false;
    winner = 0;
    statusText.setString(L"�ڷ�����");
}