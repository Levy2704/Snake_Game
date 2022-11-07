#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING // Needed with c++17
/******************************************************************************
*
*  ACADEMIC INTEGRITY DECLARATION
*
*   I, José Pablo Levy Villaseñor declare that except where I have referenced,
*  the work I am are submitting in this attachment is my own work. I
*  acknowledge and agree that the assessor of this assignment may,
*  for the purpose of authenticating this assignment, reproduce it
*  for the purpose of detecting plagiarism. I have read and am aware
*  of the Think Education Policy and Procedure viewable online at
*  https://www.torrens.edu.au/policies-and-forms
*
******************************************************************************/

#include <iostream>
#include <string>
#include "textpixels_enums.h"
#include "textpixels.h"
#include "Snake.h"
#include "Fruit.h"
#include <conio.h>
using namespace std;
using namespace textpixels;

// A collection of screens you would want in game
enum Screen         
{
  MENU = 1,
  PLAY,
  PAUSE,
  GAMEOVER,
  EXIT,
  NONE,
  HARD_MODE,
  EASY_MODE,
};
enum WALL
{
    WALL_LEFT = 0,
    WALL_RIGHT = 34,
    WALL_UP = 0,
    WALL_BELOW = 14,
};
// The playing area (snake garden) is above the graphic user interface (GUI)
const int LEVEL_WIDTH = 35;
const int LEVEL_HEIGHT = 15;
const int GUI_HEIGHT = 10;

// Function declarations
int drawLevelAndGui();
int displayMenuScreen();
int playSnake();

/* ------------------------------------------------------------------------- MAIN */

int main()
{
    // textpixels creates our 30x30 console window
    textpixels::setupConsole(LEVEL_WIDTH, LEVEL_HEIGHT + GUI_HEIGHT);

    /// I chose `screen` as a term for part of the program with its own visuals+controls
    int screen = Screen::MENU;    // Start the game at the menu.
    int score = 0;
    /// Main game loop
  while (screen != Screen::EXIT)
  {
    if (screen == Screen::MENU)
    {
      screen = displayMenuScreen();
    }
    else if (screen == Screen::PLAY)
    {
      score = playSnake();
      screen = Screen::MENU;      /// When game is done, we want to go to menu.
    }
  }
  // Show Exit screen?
  textpixels::exit();
  return (0);
}

/* -------------------------------------------------------- FUNCTION DEFINITIONS */
// Draw the Level's ground and border wall to top of window,
// draw gui with score and any other info at bottom.
void drawLevelAndGui(Snake snake, int score)
{
    // Draw the background and a wall.
    fillWindow(DARK_BLUE);
    fillRect(1, 1, LEVEL_WIDTH - 2, LEVEL_HEIGHT - 2, DARK_GREY);
    // Draw the gui background, starting at bottom of level
    fillRect(0, LEVEL_HEIGHT, LEVEL_WIDTH, GUI_HEIGHT, BLACK);
    drawString(1, 0, "SCORE: " + to_string(score), layerColours(FG_WHITE, BG_DARK_BLUE));
    drawString(2, LEVEL_HEIGHT + 1, "(Q) ", FG_CYAN);
    drawString(6, LEVEL_HEIGHT + 1, "Quit", FG_GREY);
    drawString(2, LEVEL_HEIGHT + 3, "(P)", FG_CYAN);
    drawString(6 ,LEVEL_HEIGHT + 3, "for Pause", FG_GREY);
    drawString(25, LEVEL_HEIGHT - 1, "fps: ", layerColours(FG_WHITE, BG_DARK_BLUE));
    drawString(30, LEVEL_HEIGHT - 1, textpixels::getFpsString(), layerColours(FG_WHITE, BG_DARK_BLUE));
    drawString(2, LEVEL_HEIGHT + 7, "Snake moves every " + to_string(snake.moveInterval) + " frames", layerColours(FG_GREY, BG_BLACK));
}
// Draw the start menu
// Select mode and show rules 
void startMenu()
{
    textpixels::startFrame;
    fillWindow(BLACK);
    drawString(4, 3, "Choose your mode", FG_GREY);
    drawString(1, 2, "-------------------------------", FG_GREY);
    drawString(7, 5, "(H)", FG_RED);
    drawString(11, 5, "For Hard Mode", FG_GREY);
    drawString(7, 7, "(E) ", FG_CYAN);
    drawString(11, 7, "For Easy Mode", FG_GREY);
    drawString(1, 9, "-------------------------------", FG_GREY);
    drawString(4, 11, "Learn the rules:", FG_GREY);
    drawString(4, 13, "Banana", FG_YELLOW);
    drawString(10, 13, " gives you 1 pt", FG_GREY);
    drawString(4, 15, "Grapes", FG_MAGENTA);
    drawString(10, 15, " gives 10 pts and speed ", FG_GREY);
    drawString(4, 17, "Hard Mode", FG_RED);
    drawString(14, 17, "bananas add speed,", FG_GREY);
    drawString(4,19, "walls and grapes kill you", FG_GREY);
    drawString(1, 20, "-------------------------------", FG_GREY);
    _getch();
    textpixels::endFrame;
}
// Pause the game so player can take a break
void pause()
{
    textpixels::startFrame;
    fillRect(6, 3, 25, 15, BLACK);
    drawString(8, 5, "Pause", FG_WHITE);
    drawString(8, 7, "(R)", FG_CYAN);
    drawString(12, 7, "To return to game", FG_WHITE);
    drawString(8, 9, "(Q) ", FG_RED);
    drawString(12, 9, "To quit", FG_WHITE);
    _getch();
    textpixels::endFrame;
}
// Hit a wall show player how they suck
// End Game
void hitWall()
{
    textpixels::startFrame;
    fillRect(6, 3, 25, 15, BLACK);
    drawString(6, 5, "You hit a wall you lose", FG_GREY);
    drawString(8, 7, "(S)", FG_CYAN);
    drawString(11,7, " Start game again ", FG_GREY);
    drawString(8,9, "(Q)", FG_RED);
    drawString(12,9, "To quit", FG_GREY);
    _getch();
    textpixels::endFrame;
}
// Ate the forbidden fruit in hardmode 
// Didn´t you watch snowhite?
void eatForbidden()
{
    textpixels::startFrame;
    fillRect(6, 3, 25, 20, BLACK);
    drawString(8, 5, "You ate the", FG_GREY);
    drawString(19, 5, " grape", FG_MAGENTA);
    drawString(8, 7, "You loose!!!!!", FG_GREY);
    drawString(8, 9, " (S)", FG_CYAN);
    drawString(11, 9, "Start game again ", FG_GREY);
    drawString(8, 11, "(Q)", FG_RED);
    drawString(12, 11, "To quit", FG_GREY);
    _getch();
    textpixels::endFrame;
}
void snakeBitten()
{
    textpixels::startFrame;
    fillRect(6, 3, 25, 20, BLACK);
    drawString(8, 5, "You bitten yourself", FG_GREY);
    drawString(8, 7, "You loose!!!!!", FG_GREY);
    drawString(8, 9, "(S)", FG_CYAN);
    drawString(11, 9, " Start game again ", FG_GREY);
    drawString(8, 11, "(Q)", FG_RED);
    drawString(12, 11, "To quit", FG_GREY);
    _getch();
    textpixels::endFrame;
}

// Play the game! Responsible for core game loop: listening for input,
// simulating changes in the game world, drawing the new state to screen.
// Returns score 

int playSnake()
{
    textpixels::setFps(200);      // Set a high frame rate for input sensitivity.

    // Set up variables thats persists in the loops
    bool playerHasQuit = false;
    bool playerHit = false;
    bool playerEatsForbidden = false;
    bool snakeEatsSnake = false;
    bool hardMode = 0;
    bool menu = true;
    bool playerPause = false;
    int score = 0;
    int fruit = 0;

    // Declare a snake object using the new Snake type (see Snake.h in header files)
    // Note on moveInterval: Moving slithers every 12th frame is still quick at 200fps.
    Snake slithers;
    slithers.colour =  FG_DARK_RED;
    slithers.moveInterval = 15;
    slithers.location.x = 5;    // 5 squares across
    slithers.location.y = 1;
    slithers.length = 1;
    slithers.xDir = Direction::RIGHT;
    slithers.yDir = Direction::NONE;

    //Declare fruits----------
    // Banana is a good food gives you points 
    Fruit banana;
    banana.location = Point2d(13, 6);
    banana.colour = YELLOW;
    banana.pointValue = 1;

    // Forbidden fruit gives 10 points and you are faster 
    //Hard Mode: the Forbidden fruit is like in snow white eat and die
    Fruit forbidden;
    forbidden.location = Point2d(27, 5);
    forbidden.colour = MAGENTA;
    forbidden.pointValue = -1;

    do
    {
        textpixels::startFrame();   // Let textpixels know we're doing stuff in this frame.

        /* ---------------------------------------------------------------------- INPUT */
        // Input for interface
       // S to play again 
        if (keyIsDown('S'))
        {
            playerHit = false;
            playerEatsForbidden = false;
            snakeEatsSnake = false;
            score = 0;
            slithers.colour = FG_DARK_RED;
            slithers.location = Point2d(6, 1);
            slithers.moveInterval = 15;
            slithers.length = 1;
        }
        // Q for Quit
        if (keyIsDown('Q'))
        {
            playerHasQuit = true;
            break;  // End the DO loop now, go to the line after it ends (return score?)
        }
        // H for hardMode
        if (keyIsDown('H'))
        {
            hardMode = true;
            menu = false;
        }
        // E for easy mode
        if (keyIsDown('E'))
        {
            hardMode = false;
            menu = false;
        }
        // P for pause 
        if (keyIsDown('P'))
        {
            playerPause = true;
        }
        // R to unpause
        if (keyIsDown('R'))
        {
            playerPause = false;
        }

        // Check if a, d, left/right arrow is pressed, store it 
        Direction xDirectionInput = textpixels::getLeftRightInput();
        Direction yDirectionInput = textpixels::getUpDownInput();

        /* ----------------------------------------------------------------- PROCESSING */
        if (xDirectionInput != Direction::NONE)
        {
            // Change our snake speed to match the direction that was entered
            slithers.xDir = xDirectionInput;
            slithers.yDir = Direction::NONE;
        }
        else if (yDirectionInput != Direction::NONE)
        {
            // Set slithers' vertical direction to match the new input.
            slithers.yDir = yDirectionInput;
            // also, don't forget to set her horizontal to NONE.. otherwise she might move diagonally.
            slithers.xDir = Direction::NONE;
        }
        // If the snake's moveInterval divides evenly into the current frame count, move her.
        if (textpixels::frame() % slithers.moveInterval == 0)
        {
            slithers.travel.push_back(slithers.location);
            slithers.travel.insert(slithers.travel.begin(), slithers.location);
            slithers.travel.resize(slithers.length + 3);
            slithers.location.x += slithers.xDir; // xDir is -1, 0 or 1 (see Direction enum) 
            slithers.location.y += slithers.yDir;

            // Handle snak+fruit location
            if (slithers.location == banana.location)
            {
                score += 1;
                slithers.length += 1;

                // Move fruit to a random location within the walls.
                int rfruitx = rand();
                int rfruity = rand();
                int fruitx = (float(rfruitx) / float(RAND_MAX) * 31) + 1;
                int fruity = (float(rfruity) / float(RAND_MAX) * 11) + 1;
                banana.location.x = fruitx;
                banana.location.y = fruity;
                if (hardMode == true)
                {
                    slithers.moveInterval -= 1;
                }
            }
            if (slithers.location == forbidden.location)
            {
                score += 10;
                slithers.length += 2;
                slithers.moveInterval -= 1;

                // Forbidden fruit moves in to random location
                int rfruitx = rand();
                int rfruity = rand();
                int fruitx = (float(rfruitx) / float(RAND_MAX) * 31) + 1;
                int fruity = (float(rfruity) / float(RAND_MAX) * 11) + 1;
                forbidden.location.x = fruitx;
                forbidden.location.y = fruity;

                //Hard Mode forbidden kills you
                if (hardMode == true)
                {
                    // Hardmoe the fruit actually kills you
                    playerEatsForbidden = true;
                    slithers.colour = RED;
                    _getch();
                }
            }

            // Walls kill you when you hit it in HardMode
            if (hardMode == true)
            {
                if (slithers.xDir != Direction::NONE)
                {
                    if (slithers.location.x == WALL_LEFT || slithers.location.x == WALL_RIGHT)
                    { // When hit wall change colour to red, stop screen and display pop-up screen
                        playerHit = true;
                        slithers.colour = RED;
                    }
                }
                if (slithers.yDir != Direction::NONE)
                {
                    if (slithers.location.y == WALL_UP || slithers.location.y == WALL_BELOW)
                    {
                        playerHit = true;
                        slithers.colour = RED;
                    }
                }
            }
            //Normal mode: When slithers hits a wall it is "teleports" to the opposite wall
            else
            {
                if (slithers.xDir != Direction::NONE)
                {
                    if (slithers.location.x == WALL_LEFT)
                    {
                        slithers.location.x = 33;
                    }
                    if (slithers.location.x == WALL_RIGHT)
                    {
                        slithers.location.x = 0;
                    }
                }
                if (slithers.yDir != Direction::NONE)
                {
                    if (slithers.location.y == WALL_UP)
                    {
                        slithers.location.y = 12;
                    }
                    if (slithers.location.y == WALL_BELOW)
                    {
                        slithers.location.y = 0;
                    }
                }
            }
        }

        /* ---------------------------------------------------------------- OUTPUT */

        drawLevelAndGui(slithers, score);
        // Draw our snake as a pixel: use the x,y location and colour info from slithers.
        drawPixel(slithers.location, slithers.colour);
        //Draw slither body by using the travel log after eating a fruit
        if (slithers.length-1 > 0)
        {   
            //Add one pixel and the tail to slithers body
            //After eating a fruit;
            for (int i = 0; i < slithers.length; i++)
            {
                drawPixel(slithers.travel[i], FG_WHITE);
                drawPixel(slithers.travel[i+1], FG_BLACK);
                // If the snake hits its own body, game over
                if (slithers.location == slithers.travel[i])
                {
                   snakeEatsSnake= true;
                   slithers.colour = FG_RED;
                }

            }
        }
        drawPixel(banana.location, banana.colour);
        drawPixel(forbidden.location, forbidden.colour);

        //------Activate Pop-Up messages----------
        if (menu == true)
        {
            startMenu();
        }
        if (playerPause== true)
        {
            pause();
        }
        // Show Pop-Up message of why you die
        if (playerHit == true)
        {
            hitWall();
        }
        if (playerEatsForbidden== true)
        {
            eatForbidden();
        }
        if (snakeEatsSnake==true)
        {
            snakeBitten();
        }
      
        textpixels::endFrame(); // Tell textpixel it can draw the screen and sleep
    } while (!playerHasQuit);  // Only stop when playerHasQuit

    return score; // placeholder score 
}

// Draws the menu screen, waits for user input.
// Returns the player's choice using a constant from enum Screen.
int displayMenuScreen()
{
    textpixels::setFps(120);
    int choice = Screen::NONE;
    do                            // Keeps looping, waiting for input
    {
        textpixels::startFrame();  // Needed always at start of game loop
        fillWindow(FG_WHITE);
        drawString(8, 3, "Game Snakes!!!!", layerColours(FG_WHITE, BG_DARK_RED));
        drawString(5, 0, "|", FG_GREY); drawString(5, 1, "|", FG_GREY); drawString(5, 2, "|", FG_GREY);
        drawString(5, 3, "|", FG_GREY); drawString(5, 4, "|", FG_GREY); drawString(5, 5, "|", FG_GREY);
        drawString(5, 6, "|", FG_GREY); drawString(5, 7, "|", FG_GREY); drawString(5, 8, "|", FG_GREY);
        drawString(5, 9, "|", FG_GREY); drawString(5, 10, "|", FG_GREY); drawString(5, 11, "|", FG_GREY);
        drawString(5, 12, "|", FG_GREY); drawString(5, 13, "|", FG_GREY); drawString(5, 14, "|", FG_GREY);
        drawString(5, 15, "|", FG_GREY); drawString(5, 16, "|", FG_GREY); drawString(5, 17, "|", FG_GREY);
        drawString(5, 18, "|", FG_GREY); drawString(5, 19, "|", FG_GREY); drawString(5, 20, "|", FG_GREY);
        drawString(5, 21, "|", FG_GREY); drawString(5, 22, "|", FG_GREY); drawString(5, 23, "|", FG_GREY);
        drawString(5, 24, "|", FG_GREY); drawString(5, 25, "|", FG_GREY); drawString(5, 26, "|", FG_GREY);
        drawString(5, 27, "|", FG_GREY); drawString(5, 28, "|", FG_GREY); drawString(5, 29, "|", FG_GREY);
        drawString(8,7,"(S) To start",layerColours(FG_WHITE, BG_DARK_RED));
        drawString(8,9, "(X) Exit", layerColours(FG_WHITE, BG_DARK_GREY));
        drawString(0, 5, "-----------------------------------", FG_GREY);
       drawString(14, windowHeight()-2, "v0.3 The fruitening", layerColours(FG_DARK_GREY, BG_WHITE));

    // Check if any valid keys are down. `else if` makes sure we only take one
    // input at a time. 
    if (keyIsDown('S'))
    {
      choice = Screen::PLAY;
    }
    else if (keyIsDown('X'))
    {
      choice = Screen::EXIT;
    }
    textpixels::endFrame();     // Needed always at end of game loop.
  } while (choice == Screen::NONE);     // Only stop when playerHasQuit  
  return(choice);
}
