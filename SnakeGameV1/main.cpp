#include <iostream>
#include <windows.h> 
#include <conio.h>
#include <vector>
#include <iomanip>
using namespace std;

bool gameOver;
int width = 22;
int length = 22;
int x, y, fruitX, fruitY;
int score;
enum eDirection
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
eDirection dir;
vector<pair<int, int>> tail;

void clearScreen()
{
    cout << "\033[2J\033[H"; // ANSI escape sequence to clear screen
}

void hideCursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;       // set cursor size to 1
    cursorInfo.bVisible = false; // hide cursor
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void setup(void)
{
    hideCursor();
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = length / 2;
    srand(time(0)); // Initialize random seed
    fruitX = rand() % (width - 2) + 1;
    fruitY = rand() % (length - 2) + 1;
    score = 0;
    tail.clear(); // clear the tail vector, now it's empty
}

void draw(void)
{
    clearScreen();
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < length; j++)
        {
            if (i == 0 || j == 0 || i == width - 1 || j == length - 1)
            {
                cout << "#";
            }
            else if (i == y && j == x)
            {
                cout << "O"; // Player
            }
            else if (i == fruitY && j == fruitX)
            {
                cout << "F"; // Fruit
            }
            else
            {
                bool isTail = false;
                for (auto const &mytail : tail)
                {
                    if (mytail.first == j && mytail.second == i)
                    {
                        isTail = true;
                        cout << "o";
                    }
                }
                if (!isTail)
                {
                    cout << " ";
                }
            }
        }
        cout << endl;
    }
    cout << "Score : " << score << endl;
}

void input(void)
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'w':
            dir = UP;
            break;
        case 'x':
            gameOver = true;
            break;
        default:
            break;
        }
    }
}

void logic(void)
{
    int prevX = x;
    int prevY = y;
    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    if (!tail.empty())
    { // if the tail not empty -> update the tail coordinates by pushing prevX, prevY at the front and popping last coordinate in the vector
        tail.insert(tail.begin(), pair{prevX, prevY});
        tail.pop_back();
    }
    if (x == fruitX && y == fruitY)
    {
        score += 10;
        fruitX = rand() % (width - 2) + 1;
        fruitY = rand() % (length - 2) + 1;
        tail.insert(tail.begin(), pair{x, y});
    }
    else if (x >= width || x <= 0 || y >= length || y <= 0)
    {
        gameOver = true;
    }
}

void gameIsOver()
{
    clearScreen();
    for (int i = 0; i < (length / 2); i++)
    {
        cout << endl;
    }
    cout << setw(21) << right << "Game Over" << endl;
    cout << setw(20) << right << "Score : " << score << endl;
    for (int i = 0; i < (length / 2); i++)
    {
        cout << endl;
    }
}

int main()
{
    setup();
    while (!gameOver)
    {
        draw();
        input();
        logic();
        Sleep(100);
    }
    gameIsOver();
    return 0;
}
