#include "Utilities.h"
//---------------------change the color of the output-----------------------------------//
void setTextColor(Color colorToSet)  // change the color of the output
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)colorToSet);
}
//------------------function for hiding the cursor----------------------------------------//
void hideCursor() // function for hide the cursor
{
	HANDLE myconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CURSOR;
	CURSOR.dwSize = 1;
	CURSOR.bVisible = FALSE;
	SetConsoleCursorInfo(myconsole, &CURSOR);//second argument need pointer
}
//--------------------------------------------------------------------------------------//
void pause() // waits for any key press
{
	while (!_kbhit())
	{
		//do nothing until the user hit key
	}
	_getch();
}
//--------------------------------------------------------------------------------------//
void gotoxy(short x, short y) // move the cursor to the (x,y) postion on the screen
{
	static HANDLE hConsoleOutput = NULL;
	if (NULL == hConsoleOutput)
		hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD dwCursorPosition = { x, y };
	cout.flush();
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}
//--------------------------------------------------------------------------------------//