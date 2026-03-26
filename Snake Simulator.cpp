#include<iostream>
#include<vector>
#include<conio.h>
#include<windows.h> // To get console dimensions(screen size).
using namespace std;

#define MAX_LENGTH 1000 // Just write "MAX_LENGTH" instead of "1000".

const char DIR_UP = 'U'; //"const" makes the variable read-only, no change further in the code.
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

int conHeight, conWidth;

void intScreen() { // To get console dimensions(screen size).
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    conHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    conWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

struct point { // To represent a point in 2D space,___Used for both snake segments and food.
    int xCoord, yCoord;
    
    point() {} // For food.
    
    point(int x, int y) { // For snake body. 
        xCoord = x;
        yCoord = y;
    }
};

class Snake {
    int length;
    char direction;

public:
    point body[MAX_LENGTH]; //Array of Point,___Stores all snake segments.

    Snake(int x, int y) { // Constructor,___Initializing snake with length 1 at (x,y) and moving right.
        length = 1;
        body[0] = point(x, y); // Initial position of the snake's head.
        direction = DIR_RIGHT;
    }

    int getLength() { // Gettter
        return length;
    }

    void changeDirection(char newDirection) { // Impressive method to check direction with just one condition(just prevent reverse direction).
        if(newDirection == DIR_UP && direction != DIR_DOWN) { // If pressed "UP" then check if current movement must not be "DOWN".
            direction = newDirection; // If condition passes then "UP" is valid,___Now snake is moving "UP".
        } else if(newDirection == DIR_DOWN && direction != DIR_UP) { // If pressed "DOWN" then check if current movement must not be "UP".
            direction = newDirection; // If condition passes then "DOWN" is valid,___Now snake is moving "DOWN".
        } else if(newDirection == DIR_LEFT && direction != DIR_RIGHT) { // If pressed "LEFT" then check if current movement must not be "RIGHT".
            direction = newDirection; // If condition passes then "LEFT" is valid,___Now snake is moving "LEFT".
        } else if(newDirection == DIR_RIGHT && direction != DIR_LEFT) { // If pressed "RIGHT" then check if current movement must not be "LEFT".
            direction = newDirection; // If condition passes then "RIGHT" is valid,___Now snake is moving "RIGHT".
        }
    }
};