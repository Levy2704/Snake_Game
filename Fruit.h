#ifndef __FRUIT__
#define __FRUIT__
#include "Point2d.h"
#include "textpixels_enums.h"
using textpixels::Point2d;
using textpixels::Direction;

class Fruit
{
public:
	// Position and speed use data types/enums are defined in textpixels_enums.h
	Point2d location{ 1,1 };
	short colour = 0x0;
	int pointValue = -1;
};

#endif

