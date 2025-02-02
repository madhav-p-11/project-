#include <iostream>
#include <conio.h>   // For _kbhit() and _getch()
#include <cstdlib>   // For rand() and srand()
#include <windows.h> // For Sleep(), SetConsoleOutputCP(), SetConsoleTextAttribute()
#include <vector>

using namespace std;

// Game constants
const int WIDTH = 40;
const int HEIGHT = 20;

// Direction enums
enum Direction { STOP = 0, UP, DOWN, LEFT, RIGHT };
Direction dir;

// Game variables
int x, y;                     // Snake head position
int foodX, foodY;             // Food position
int score;                    // Player score
vector<pair<int, int>> snake; // Snake body
bool gameOver;

// Set color function using Windows API
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Function to initialize the game
void setup() {
    SetConsoleOutputCP(CP_UTF8); // Enable UTF-8 encoding
    gameOver = false;
    dir = STOP; // Wait for the player to start

    // Snake starts in the middle of the grid
    x = WIDTH / 2;
    y = HEIGHT / 2;

    // Initialize the snake's body (3 cells long)
    snake.clear();
    snake.push_back({x, y});
    snake.push_back({x, y + 1});
    snake.push_back({x, y + 2});

    // Generate initial food position
    foodX = rand() % WIDTH;
    foodY = rand() % HEIGHT;

    score = 0;

    // Inform the player to start the game
    setColor(16); // White color
    cout << "Press W/A/S/D to start the game!" << endl;
}

// Function to draw the game board
void draw() {
    system("cls"); // Clear the console

    // Draw the top border using brick patterns
    setColor(7); // Brown for brick-like border
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "|"; // Unicode full block for bricks
    cout << endl;

    // Draw the grid
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0) {
                setColor(7); // Brown for brick-like border
                cout << "█";
            }

            setColor(2 | BACKGROUND_GREEN); // Green background for grass effect

            if (i == y && j == x) {
                setColor(3); // Dark green for snake head
                cout << "-"; // Snake head block (■)
            } else if (i == foodY && j == foodX) {
                setColor(3 | FOREGROUND_INTENSITY); // Dark green for food
                cout << "*";
            } else {
                bool printed = false;
                for (const auto &segment : snake) {
                    if (segment.first == j && segment.second == i) {
                        setColor(2); // Dark green for snake body
                        cout << "■"; // Snake body block (■)
                        printed = true;
                        break;
                    }
                }
                if (!printed) cout << " ";
            }

            if (j == WIDTH - 1) {
                setColor(7); // Brown for brick-like border
                cout << "█";
            }
        }
        cout << endl;
    }

    // Draw the bottom border
    setColor(7); // Brown for brick-like border
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "█";
    cout << endl;

    // Display the score
    setColor(15); // White for score text
    cout << "Score: " << score << endl;
}

// Function to handle user input
void input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'w':
                if (dir != DOWN) dir = UP;
                break;
            case 's':
                if (dir != UP) dir = DOWN;
                break;
            case 'a':
                if (dir != RIGHT) dir = LEFT;
                break;
            case 'd':
                if (dir != LEFT) dir = RIGHT;
                break;
            case 'x':
                gameOver = true;
                break;
        }
    }
}

// Function to update the game logic
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
    }
}

int main() {
    setup();

    while (dir == STOP) {
        input();
    }

    while (!gameOver) {
        draw();
        input();
        logic();
        Sleep(100);
    }

    setColor(12);
    cout << "Game Over! Your final score is: " << score << endl;
    return 0;
}
