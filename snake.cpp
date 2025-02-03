#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <vector>

using namespace std;

const int WIDTH = 30;
const int HEIGHT = 30;

enum Direction { STOP = 0, UP, DOWN, LEFT, RIGHT };
Direction dir;

int x, y;
int foodX, foodY;
int score;
vector<pair<int, int>> snake;
bool gameOver;
int speed;
bool gameStarted = false;  // New flag to check if the game has started

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void gotoXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false; // Set the cursor to invisible
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void setup() {
    SetConsoleOutputCP(CP_UTF8);
    gameOver = false;
    dir = STOP;

    x = WIDTH / 2;
    y = HEIGHT / 2;

    snake.clear();
    snake.push_back({x, y});
    snake.push_back({x, y + 1});
    snake.push_back({x, y + 2});

    foodX = rand() % WIDTH;
    foodY = rand() % HEIGHT;

    score = 0;
    speed = 100; // Initial speed

    hideCursor(); // Hide cursor at the start of the game

    setColor(16);
    cout << "Press S to start the game!" << endl; // Show message to press 'S'
    cout << "Press Enter to exit the game at any time." << endl;
}

void draw() {
    gotoXY(0, 0); // Move to top-left of the screen to start drawing the grid
    setColor(7);

    // Draw top border
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "█";
    cout << endl;

    // Draw game field
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0) {
                setColor(7);
                cout << "█"; // Left wall
            }

            setColor(0);

            // Draw snake
            bool printed = false;
            for (const auto &segment : snake) {
                if (segment.first == j && segment.second == i) {
                    setColor(2);
                    cout << "⬤";
                    printed = true;
                    break;
                }
            }

            // Draw player (head of the snake)
            if (!printed && i == y && j == x) {
                setColor(3);
                if (dir == UP) cout << "▲";
                else if (dir == DOWN) cout << "▼";
                else if (dir == LEFT) cout << "◄";
                else if (dir == RIGHT) cout << "►";
                else cout << "▲";
            }

            // Draw food
            else if (!printed && i == foodY && j == foodX) {
                setColor(4 | FOREGROUND_INTENSITY);
                cout << "■";
            }

            // Draw empty space
            else if (!printed) {
                cout << " ";
            }

            if (j == WIDTH - 1) {
                setColor(7);
                cout << "█"; // Right wall
            }
        }
        cout << endl;
    }

    // Draw bottom border
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "█";
    cout << endl;

    // Draw score (without speed)
    setColor(15);
    cout << "Score: " << score << endl;
}

void input() {
    if (_kbhit()) {
        char ch = _getch();
        if (!gameStarted) {
            // Start the game if the player presses 'S'
            if (ch == 's' || ch == 'S') {
                gameStarted = true; // Game starts
                dir = UP; // Initialize direction
            }
            else if (ch == 13) {  // Enter key
                gameOver = true;  // Exit the game at the start if Enter is pressed
            }
        } else {
            switch (ch) {
                case 'w': if (dir != DOWN) dir = UP; break;
                case 's': if (dir != UP) dir = DOWN; break;
                case 'a': if (dir != RIGHT) dir = LEFT; break;
                case 'd': if (dir != LEFT) dir = RIGHT; break;
                case 'x': gameOver = true; break;
                case 13: gameOver = true; break; // Exit the game during gameplay if Enter is pressed
            }
        }
    }
}

void logic() {
    for (int i = snake.size() - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    snake[0] = {x, y};

    switch (dir) {
        case UP: y--; break;
        case DOWN: y++; break;
        case LEFT: x--; break;
        case RIGHT: x++; break;
        
        default: break;
    }

    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        gameOver = true;

    for (int i = 1; i < snake.size(); i++) {
        if (snake[i].first == x && snake[i].second == y)
            gameOver = true;
    }

    if (x == foodX && y == foodY) {
        score += 10;
        snake.push_back({-1, -1});
        foodX = rand() % WIDTH;
        foodY = rand() % HEIGHT;

        if (score % 10 == 0) {
            speed = max(10, speed * 80 / 100); // Increase speed by 20%, ensuring it doesn't go below 10ms
        }
    }
}

int main() {
    setup();

    while (!gameStarted) {
        input(); // Wait until the user presses 'S' to start or Enter to exit
    }

    // Once game starts, move to the main game loop
    while (!gameOver) {
        draw();
        input();
        logic();
        Sleep(speed);
    }

    setColor(12);
    cout << "OOPS!, GAME OVER AND YOUR FINAL SCORE IS :" << score << endl;
    return 0;
}
