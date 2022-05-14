#include <iostream>
#include "Game.h"
#include "Utilities.h"

int main(int argc, char** argv)
{
	bool isExit = false;
	Game game;

	try
	{
		game.readCMDParameters(argc, argv);
	}

	catch (const char* errorType)
	{
		cout << errorType << endl;
		cout << "Usage: " << "[-save][-load | -load -silent]" << endl;
		isExit = true;
	}

	hideCursor();

	while (!isExit)
	{
		try
		{
			isExit = game.startProgram();
		}
		catch (string& error)
		{
			game.printAnnouncment(error);
			isExit = true;
		}

	}
	return 0;
}