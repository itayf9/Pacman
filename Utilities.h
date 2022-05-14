#pragma once

#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <windows.h>
#include <time.h>
#include <queue>
#include "Coord.h"

using namespace std;

//--------------------------------------------------------------------------------------//
enum class Color
{
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHTGREY = 7,
	DARKGREY = 8,
	LIGHTBLUE = 9,
	LIGHTGREEN = 10,
	LIGHTCYAN = 11,
	LIGHTRED = 12,
	LIGHTMAGENTA = 13,
	YELLOW = 14,
	WHITE = 15
};
//--------------------------------------------------------------------------------------//
void setTextColor(Color); // change the color of the output
void hideCursor(); // function for hide the cursor
void pause(); // waits for any key press
void gotoxy(short x, short y); // move the cursor to the (x,y) postion on the screen