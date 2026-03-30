#include<iostream>
#include<vector>
#include<conio.h>
#include<windows.h> // To get console dimensions(screen size).
#include <cstdlib>
#include <ctime>
using namespace std;

#define MAX_LENGTH 1000 // Just write "MAX_LENGTH" instead of "1000".

const char DIR_UP = 'U'; //"const" makes the variable read-only, no change further in the code.
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

int conHeight, conWidth;

void initScreen() { // To get console dimensions(screen size).
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    conHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    conWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

struct Point { // To represent a point in 2D space,___Used for both snake segments and food.
    int xCoord, yCoord;
    
    Point() {} // For food.
    
    Point(int x, int y) { // For snake body,___"body[0] = Point(x, y);" for array, constructor calls like that but temporary and then assign it to left side for permanent.
        xCoord = x;
        yCoord = y;
    }
};

class Snake {
    int length;
    char direction;

public:
    Point body[MAX_LENGTH]; //Array of Point,___Stores all snake segments.

    Snake(int x, int y) { // Constructor,___Initializing snake with length 1 at (x,y) and moving right.
        length = 1;
        body[0] = Point(x, y); // Initial position of the snake's head.
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

    bool move(Point food) {
        for(int i = length-1; i > 0; i--) { // Starts from tail to head.
            body[i] = body[i-1]; // Grab the position of upper one's(think array building) and eventually loop ends before head grabs someone's position.
        }

        switch(direction) {
            int temp; // Temporary variable to store position of head as of those axis which is varying.

            case DIR_UP: // if snake is moving UP, it means y-axis(decreasing) is in effect while x-axis is still.
                temp = body[0].yCoord; // temp got the current y-coordinate of head.
                body[0].yCoord = temp-1; // Move head up, "-1" because in console (0, 0) is at top-left corner thus y-coordinate needs to be decreased to move up.
                break;
            case DIR_DOWN: // if snake is moving DOWN, it means y-axis(increasing) is in effect while x-axis is still.
                temp = body[0].yCoord; 
                body[0].yCoord = temp+1; // Move head down.
                break;
            case DIR_RIGHT: // if snake is moving RIGHT, it means x-axis(increasing) is in effect while y-axis is still.
                temp = body[0].xCoord;
                body[0].xCoord = temp+1; // Move head right.
                break;
            case DIR_LEFT: // if snake is moving LEFT, it means x-axis(decreasing) is in effect while y-axis is still.
                temp = body[0].xCoord;
                body[0].xCoord = temp-1; // Move head left. 
                break;    
        }/*👉 First loop: Body follows old positions
           👉 This block: Head moves to new position
           👉 Together: Creates smooth snake movement,___both created 1 frame, in every frame snake body shift forward,___Remember, it happens 60 times/sec(60 FPS).*/
    
        //Collision detection.
        for(int i = 1; i < length; i++) { //Checks whether head collide with body or not.
            if(body[0].xCoord == body[i].xCoord && body[0].yCoord == body[i].yCoord) return false; //Compare head's x-coordinate(y also) with entire each body's x and y coordinates to check,- Are both same? return false if collision detected.
        }

        if(body[0].xCoord < 0 || body[0].xCoord >= conWidth || body[0].yCoord < 0 || body[0].yCoord >= conHeight) return false;

        if(food.xCoord == body[0].xCoord && food.yCoord == body[0].yCoord) { //if coordinates of food is same as of snake's head coordinates.
            body[length] = Point(body[length-1].xCoord, body[length-1].yCoord); //"body[length]" a new body(remember array starts with 0), fetch current tail coordinates and assign to new part.
            length++; //For array, since length incremented, now new part added.
        }

        return true;
    }
};

class Board {
    Snake *snake; // Created a pointer that will store address actual snake address,___created in stack,___Pointer type must match the data it points to, thus "Snake" instead of int.
    Point food;
    int score;
    const char Snake_Body = 'O';
    const char FOOD = 'X';

public:
    Board(){
        spawnFood();
        snake = new Snake(10, 10); //now pointer takes the address of actual snake object and object created in heap memory because of "new" while pointer created in stack memory.
        score = 0;
    }

    ~Board() {
        delete snake;
    }

    int getScore() {
        return score;
    }

    void spawnFood() {
        int y = rand() % conHeight; //"rand()" gives random numer and module(%) strict it to console height.
        int x = rand() % conWidth; // same for width.

        food = Point(x, y); // creates temporary object and assign it to "food".
    }

    void goToXY(int x, int y) { //move cursor"|" in the console so character print there.
        COORD coord; //"COORD" is a structure with X and Y in "windows.h".
        coord.X = x;
        coord.Y = y;

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); //move cursor.
    }

    void DisplayCurrentScore() { //display score in the top middle of console.
        goToXY(conWidth/2, 0);
        cout << "Score: " << score;
    }

    void draw() { //Draws entire game frame: Clears screen, Draws snake, Draws food and Shows score.
        system("cls"); //clear whole console screen and will print newly fresh as per below.

        for(int i = 0; i < (*snake).getLength(); i++) { //since "snake" was a pointer thus "*snake" or it can be written as "snake->getLength()".
            goToXY((*snake).body[i].xCoord, (*snake).body[i].yCoord); //get coordinates of snake moves cursor to print snake bodys.
            cout << Snake_Body;
        }

        goToXY(food.xCoord, food.yCoord); //again fetch food coordinates and move cursor there.
        cout << FOOD;

        DisplayCurrentScore(); //eeh, u know.
    }

    bool update() {
        bool isAlive = (*snake).move(food); //checks snake's life, because move() has a function who check snake bite itself or not.

        if(!isAlive) return false;

        if(food.xCoord == (*snake).body[0].xCoord && food.yCoord == (*snake).body[0].yCoord) { //if snake ate food then score upgrades and immediately spawn new foodon the screen.
            score++;
            spawnFood();
        }

        return true;
    }

    void getInput() {
        while(_kbhit()) { //"kbhit" be like 'Has user pressed a key?'.
            char key = _getch();

            if(key == 'w' || key == 'W') (*snake).changeDirection(DIR_UP);
            else if(key == 'a' || key == 'A') (*snake).changeDirection(DIR_LEFT);
            else if(key == 'd' || key == 'D') (*snake).changeDirection(DIR_RIGHT);
            else if(key == 's' || key == 'S') (*snake).changeDirection(DIR_DOWN);
        }
    }
};

int main() {
    srand(time(0));
    initScreen();

    Board *board = new Board();
    while(true) {
        board->getInput(); //checks input status.
        if(!board->update()) break; //if "board->update()" gives true(snake is alive) then break will execute, thus "!".
        board->draw(); //clear whole screen and reprint everything as per logic.
        Sleep(75); //wait 75ms, controls fps, as well as snake speed.
    }

    system("cls"); //need to clear whole screen to show result.
    cout << "GAME OVER\nFinal Score : " << (*board).getScore();

    delete board;

    Sleep(7000);
    return 0;
}