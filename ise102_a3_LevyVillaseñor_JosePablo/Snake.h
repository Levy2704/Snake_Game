#ifndef __SNAKE__
#define __SNAKE__
#include "Point2d.h"
#include "textpixels_enums.h"
#include <vector>
using textpixels::Point2d;
using textpixels::Direction;
using std::vector;

class Snake
{
public:

  short colour = 0x0;
  
  // Position and speed use data types/enums are defined in textpixels_enums.h
  Point2d location { 1,1 };
  int xDir = Direction::NONE;
  int yDir = Direction::NONE;

  // variale for:
  // lenghy a whole number for how many parts representing the parts of snale (inlcudes head)
  int length = 5;
  // history or travekg of location we'he vissited since we started the game.
  vector <Point2d> travel;
  
  

 

  

  // How many frames pass before it's time for Snakey to move again? A high update 
  // rate for the game means we capture all input but if snakey moves every frame 
  // she's too fast. Snakey doesn't have to move every frame. She's chill.
  int moveInterval = 10;
};

#endif

