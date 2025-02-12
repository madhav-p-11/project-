#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <vector>

using namespace std;

const int WIDTH = 30;
const int HEIGHT = 30;

enum Direction { STOP = 0, UP, DOWN, LEFT, RIGHT };

class GameObject {
public:
    virtual void setPosition() = 0;
};

class Food : public GameObject {
public:
    int x, y;
    void setPosition() override {
        x = rand() % WIDTH;
        y = rand() % HEIGHT;
    }
};

class Snake {
public:
    vector<pair<int, int>> body;
    void startPosition(int startX, int startY) {
        body.clear();
        body.push_back({startX, startY});
        body.push_back({startX, startY + 1});
        body.push_back({startX, startY + 2});
    }
};

class Game {
private:
    Direction dir;
    int x, y;
    int score;
    bool gameOver;
    int speed;
    bool gameStarted;
    Snake snake;
    Food food;

public:
    Game() {
        initialize();
    }

    void setTextColor(int color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }

    void moveCursor(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void hideCursor() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    }

    void initialize() {
        SetConsoleOutputCP(CP_UTF8);
        gameOver = false;
        dir = STOP;
        x = WIDTH / 2;
        y = HEIGHT / 2;
        snake.startPosition(x, y);
        food.setPosition();
        score = 0;
        speed = 100;
        gameStarted = false;
        hideCursor();
        cout << "Press 'S' to start the game and 'Enter' to exit at any time." << endl;
    }

    void render() {
        moveCursor(0, 0);
        setTextColor(1);
        for (int i = 0; i < (WIDTH + 2)/2+1; i++)
            cout << "🧱";
        cout << endl;
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (j == 0) {
                    setTextColor(7);
                    cout << "🧱";
                }
                setTextColor(0);
                bool printed = false;
                for (const auto &segment : snake.body) {
                    if (segment.first == j && segment.second == i) {
                        setTextColor(2);
                        cout << "O";  // Replaced Unicode '⬤' with 'O'
                        printed = true;
                        break;
                    }
                }
                if (!printed && i == y && j == x) {
                    setTextColor(3);
                    char headChar = '^';  // Default direction UP
                    if (dir == DOWN) headChar = 'v';
                    else if (dir == LEFT) headChar = '<';
                    else if (dir == RIGHT) headChar = '>';
                    cout << headChar;
                } else if (!printed && i == food.y && j == food.x) {
                    setTextColor(4 | FOREGROUND_INTENSITY);
                    cout << "F";  // Replaced Unicode '■' with 'F'
                } else if (!printed) {
                    cout << " ";
                }
                if (j == WIDTH - 1) {
                    setTextColor(7);
                    cout << "🧱";
                }
            }
            cout << endl;
        }
        for (int i = 0; i < (WIDTH + 2)/2+1; i++)
            cout << "🧱";
        cout << endl;
        setTextColor(15);
        cout << "Score: " << score << endl;
    }

    void getInput() {
        if (_kbhit()) {
            char ch = _getch();
            if (!gameStarted) {
                if (ch == 's' || ch == 'S') {
                    gameStarted = true;
                    dir = UP;
                } else if (ch == 13) {
                    gameOver = true;
                }
            } else {
                switch (ch) {
                    case 'w': if (dir != DOWN) dir = UP; break;
                    case 's': if (dir != UP) dir = DOWN; break;
                    case 'a': if (dir != RIGHT) dir = LEFT; break;
                    case 'd': if (dir != LEFT) dir = RIGHT; break;
                    case 'x': gameOver = true; break;
                    case 13: gameOver = true; break;
                }
            }
        }
    }

    void updateLogic() {
        for (int i = snake.body.size() - 1; i > 0; i--) {
            snake.body[i] = snake.body[i - 1];
        }
        snake.body[0] = {x, y};
        switch (dir) {
            case UP: y--; break;
            case DOWN: y++; break;
            case LEFT: x--; break;
            case RIGHT: x++; break;
            default: break;
        }
        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) gameOver = true;
        for (int i = 1; i < snake.body.size(); i++) {
            if (snake.body[i].first == x && snake.body[i].second == y)
                gameOver = true;
        }
        if (x == food.x && y == food.y) {
            score += 10;
            snake.body.push_back({-1, -1});
            food.setPosition();
            if (score % 10 == 0) speed = max(10, speed * 80 / 100);
        }
    }

    void startGame() {
        while (!gameStarted) {
            getInput();
        }
        while (!gameOver) {
            render();
            getInput();
            updateLogic();
            Sleep(speed);
        }
        setTextColor(12);
        cout << "OOPS!, GAME OVER AND YOUR FINAL SCORE IS: " << score << endl;
    }
};

int main() {
    Game game;
    game.startGame();
    return 0;
}
