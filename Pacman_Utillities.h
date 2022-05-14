#pragma once

#include "Utilities.h"
//--------------------------------------------------------------------------------------//
enum class Game_Mode
{
	SIMPLE, SAVE, LOAD
};
//--------------------------------------------------------------------------------------//
enum class Keys
{
	UP = 'W',
	RIGHT = 'D',
	DOWN = 'X',
	LEFT = 'A',
	STAY = 'S',
	ESC = 27
};
//--------------------------------------------------------------------------------------//
enum class File_Symbols
{
	PACMAN_FILE = '@',
	GHOST_FILE = '$',
	LEGEND_FILE = '&',
	WALL_FILE = '#',
	LIMITS = '%'
};
//--------------------------------------------------------------------------------------//
enum class Choices
{
	ONE= '1',
	TWO= '2',
	THREE= '3',
	FOUR= '4',
	EIGHT= '8',
	NINE= '9'
};
//--------------------------------------------------------------------------------------//
enum class NUMBERS
{
	one = 1,
	two = 2,
	three = 3,
	four =4, 
	eight =8,
	nine =9
};
static const char END_LINE = '\n';

static const char EMPTY_STR = 'O';
static const int END_OF_INT_VEC = -1;

static const int MAX_GHOSTS = 4;
static const int INT_ZERO = 0;
static const char CHAR_ZERO = '0';



void printTilte(); // prints the title "PACMAN"