#include "Game.h"

//--------------------------------------------------------------------------------------//
Game::Game() :board(), curScreen(INT_ZERO)
{
	srand(time(nullptr));
	this->ghosts = new Ghost * [MAX_GHOSTS];
	this->ghosts[0] = new Ghost();
	this->ghosts[1] = new Ghost();
	this->ghosts[2] = new Ghost();
	this->ghosts[3] = new Ghost();
}
//--------------------------------------------------------------------------------------//
Game::Game(const Game& other)
{
	copyGame(other);
}
//--------------------------------------------------------------------------------------//
Game::~Game()
{
	delete ghosts[0];
	delete ghosts[1];
	delete ghosts[2];
	delete ghosts[3];
	delete[] ghosts;
}
//--------------------------------------------------------------------------------------//
void Game::readCMDParameters(int& argc, char** argv) // reads the parameters from the CMD
{
	vector<string> arguments;

	for (int i = 1; i < argc; ++i)
	{
		arguments.push_back(string(argv[i]));
	}

	if (arguments.empty())
	{
		gameMode = (int)Game_Mode::SIMPLE;
	}
	else
	{
		if (arguments[0] == "-save")
		{
			if (arguments.size() == 1)
			{
				gameMode = (int)Game_Mode::SAVE;
			}
			else
			{
				throw "Can't use '-save' mode with other parameters.";
			}
		}
		else if (arguments[0] == "-load")
		{
			if (arguments.size() == 1)
			{
				gameMode = (int)Game_Mode::LOAD;
			}
			else
			{
				if (arguments[1] == "-silent")
				{
					gameMode = (int)Game_Mode::LOAD;
					isSilent = true;
				}
				else
				{
					throw "Can't use '-load' mode with other parameters, execpt '-silent'.";
				}
			}
		}
		else
		{
			throw "Illigal parameter.";
		}
	}
}
//--------------------------------------------------------------------------------------//
void Game::copyGame(const Game& other)
{
	this->pacman = other.pacman;

	this->ghosts = new Ghost * [MAX_GHOSTS];
	this->ghosts[0] = other.ghosts[0];
	this->ghosts[1] = other.ghosts[1];
	this->ghosts[2] = other.ghosts[2];
	this->ghosts[3] = other.ghosts[3];

	this->fruit = other.fruit;

	this->isColorOn = other.isColorOn;
}
//--------------------------------------------------------------------------------------//
bool Game::startProgram() // presents the menu, and handles the menu options 
{
	int choice;
	bool res = false;

	if (gameMode == (int)Game_Mode::LOAD)
	{
		ghostsLevel = (int)Ghost::level::NOVICE;
		startGame();
		res = true;
	}
	else
	{
		printMenu();
		choice = checkVaildChoice();

		switch (choice)
		{
		case (int)NUMBERS::one:
			startGame();
			break;
		case (int)NUMBERS::two:
			colorMode();
			break;
		case (int)NUMBERS::three:
			ghostLevelMode();
			break;
		case (int)NUMBERS::four:
			loadScreenByFileName();
			break;
		case (int)NUMBERS::eight:
			printIstructions();
			break;
		case (int)NUMBERS::nine:
			res = true;
			break;
		default:
			break;
		}
	}
	return res;
}
//--------------------------------------------------------------------------------------//
int Game::checkVaildChoice() // gets an input from the user, and checks the validation of it
{
	int choice;

	gotoxy(2, 22);
	cout << "Please enter your choice." << endl;

	choice = _getch();// to avoid if the user insert char

	while (choice != (char)Choices::ONE && choice != (char)Choices::TWO && choice != (char)Choices::THREE && choice != (char)Choices::FOUR && choice != (char)Choices::EIGHT && choice != (char)Choices::NINE)
	{
		gotoxy(2, 22);
		cout << "Invalid choice. " << "Please enter a valid choice." << endl;

		if (isColorOn) { setTextColor(Color::WHITE); }

		choice = _getch();

		if (isColorOn) { setTextColor(Color::LIGHTRED); }
	}
	if (isColorOn) { setTextColor(Color::WHITE); }

	return choice - (int)CHAR_ZERO;// return the number as int 
}
//--------------------------------------------------------------------------------------//
void Game::getBySuffix(vector<string>& name, string subfix) // gets all files ending with 'subfix' from the directory
{
	string path = ".";
	for (const auto& entry : fs::directory_iterator(path))
	{
		if (entry.path().string().ends_with(subfix))
		{
			name.push_back((entry.path().string().substr(2, entry.path().string().length())));
		}
	}
}
//--------------------------------------------------------------------------------------//
void Game::startGame() // starts a new game, prints the game board and handles a running game
{
	bool isOpened;
	if (gameMode != (int)Game_Mode::LOAD)
	{
		if (ghostsLevel == -1)
		{
			ghostLevelMode();
		}
	}

	if (!customScreenMode) // gets the screen file names, in lexicographical order
	{
		getBySuffix(screenName, (string)".screen");
	}

	if (screenName.size() == 0)
	{
		printAnnouncment((string)"No valid screen files were found.");

		return;
	}
	if (gameMode != (int)Game_Mode::SIMPLE)
	{
		createResultAndStepsFile();
	}

	curScreen = 0;
	while (curScreen < screenName.size()) // run through the screens
	{
		board.resetBoard();

		try
		{
			readScreenFile(isOpened);

			if (gameMode == (int)Game_Mode::LOAD)
			{
				readResultFile(isOpened);
				readStepsFile(isOpened);
			}
		}
		catch (string& screenFail)
		{
			if (isSilent)
			{
				throw (string)"Test failed";
			}
			printAnnouncment((string)"Error in file: " + screenFail, (string)"Fail to open current screen. Continue will try to load the next screen.");

		}

		if (isOpened)
		{
			if (!isSilent)
			{
				printBackground();

				printBoard(); // print the board 

				printGameInfoSideBar(); // prints the legend

				printStartPositions(); // print the pacman and ghost 

				printScore(pacman.getScore());

				printLives(pacman.getNumOfLives());

				if (gameMode != (int)Game_Mode::LOAD) // in simple and save modes
				{
					printMessegeContinue();
				}
			}

			try
			{
				move();// all the moves of the game in this function
				if (gameMode == (int)Game_Mode::SAVE)
				{
					writeResultFile();
					writeStepsFile();
				}


			}
			catch (bool&)
			{
				printAnnouncment((string)"Error in file. Fail to save current screen.", (string)"Continue will try to load the next screen.");
			}
			catch (string& error)
			{
				if (isSilent)
				{
					throw error;
				}
				printAnnouncment(error);
				curScreen = screenName.size();
			}
		}

		pacman.clearSteps();
		for (int k = 0; k < numOfGhosts; k++)
		{
			(*ghosts[k]).clearSteps();
		}
		fruit.clearSteps();
		fruit.getHideAndShowVec().clear();
		fruit.getFruitValues().clear();
		fruit.getPositionsVec().clear();
		pacman.getResultsLoseLifeVec().clear();
		fruit.setIsAppear(false);
		curScreen++;
	}
	if (gameMode != (int)Game_Mode::LOAD)
	{
		pause();

		resetGame();
	}
	else if (gameMode == (int)Game_Mode::LOAD && isSilent && curScreen == screenName.size())
	{
		printAnnouncment((string)"Test succeeded.");
	}

}
//--------------------------------------------------------------------------------------//
void Game::createResultAndStepsFile() // creates new '.steps' and new '.results' files with the same name
{
	string nameOfFile, copyNameFile, addSteps = ".steps", addResult = ".result";
	for (auto i : screenName)
	{
		nameOfFile = i.substr(0, i.length() - 7);

		copyNameFile = nameOfFile;

		nameOfFile = nameOfFile + addSteps;

		copyNameFile = copyNameFile + addResult;

		resultName.push_back(copyNameFile);

		stepsName.push_back(nameOfFile);
	}
}
//--------------------------------------------------------------------------------------//
void Game::printMenu() //prints the menu
{
	system("cls");
	printBorder();

	printTilte();

	gotoxy(1, 12); cout << " (1) Start a new game.";

	gotoxy(1, 13); cout << " (2) Color Mode: ";

	if (isColorOn)
	{
		cout << "ON ";
	}
	else
	{
		cout << "OFF";
	}

	gotoxy(1, 14); cout << " (3) Ghost Level: ";

	if (ghostsLevel == -1)
	{
		cout << "Please select";
	}
	else
	{
		cout << levelArr[ghostsLevel];
	}

	gotoxy(1, 15); cout << " (4) Load a Custom Screen. ";

	if (customScreenMode)
	{
		cout << "Selected Screen: " << screenName[INT_ZERO];
	}

	gotoxy(1, 19); cout << " (8) Present instructions and keys. ";

	gotoxy(1, 20); cout << " (9) EXIT. ";
}
//--------------------------------------------------------------------------------------//
void Game::printIstructions() // prints the instructions menu
{
	printBorder();

	gotoxy(2, 2); cout << "Insructions: " << endl;

	gotoxy(2, 4); cout << " The player is represented by : " << PACMAN_SYMBOL << endl;

	gotoxy(2, 5); cout << " The player needs to escape from the two ghosts : " << GHOST_SYMBOL << endl;

	gotoxy(2, 6); cout << " the player's goal is to 'eat' all the breadcrumbs around the map : " << (char)Board_Symbols::BREAD << endl;

	gotoxy(2, 8); cout << " Keys: " << endl;

	gotoxy(2, 9); cout << " W- Move Up" << endl;

	gotoxy(2, 10); cout << " D- Move Right" << endl;

	gotoxy(2, 11); cout << " X- Move Down" << endl;

	gotoxy(2, 12); cout << " A- Move Left" << endl;

	gotoxy(2, 13); cout << " S- Stay" << endl;

	gotoxy(2, 14); cout << " ESC- Pause the Game" << endl;

	gotoxy(2, 22); cout << "Press any key to return to the menu." << endl;

	pause();
}

//--------------------------------------------------------------------------------------//
void Game::readScreenFile(bool& isOpened) // opened the next screen file, read the data from it, and writes it to the 'board'
{
	int i, j;
	int ghostCount = 0;
	int countBread = 0;
	char currChar;
	bool isLegendInLine;
	bool isHavePacman = false;
	bool isHaveLegend = false;
	char prevChar;
	int placeOfLegend;

	ifstream screenFile(screenName[curScreen]);

	if (!screenFile.is_open()) // fails to open
	{
		isOpened = false;
		throw screenName[curScreen];
	}

	for (i = 0; i < MAX_ROWS; i++) // fills the board, and converts the file's data to the borad's symbols
	{
		j = 0;
		isLegendInLine = false;
		currChar = screenFile.get();

		while (j < MAX_COLS)
		{
			prevChar = currChar;

			switch (currChar)
			{
			case (char)File_Symbols::PACMAN_FILE:
				pacman.setStartPos(Coord(j, i));
				currChar = (char)Board_Symbols::SPACE;
				isHavePacman = true;
				break;

			case (char)File_Symbols::GHOST_FILE:
				if (ghostCount < MAX_GHOSTS)
				{
					(*ghosts[ghostCount]).setStartPos(Coord(j, i));
					ghostCount++;
				}
				currChar = (char)Board_Symbols::SPACE;
				break;

			case (char)File_Symbols::LEGEND_FILE:
				board.setLegendPos(Coord(j, i));
				currChar = (char)Board_Symbols::SPACE;
				placeOfLegend = j;
				isLegendInLine = true;
				isHaveLegend = true;
				break;

			case (char)File_Symbols::WALL_FILE:
				currChar = (char)Board_Symbols::WALL;
				break;

			case (char)Board_Symbols::SPACE:
				if (j >= board.getWidth())
				{
					currChar = (char)File_Symbols::LIMITS;
				}
				else
				{
					currChar = (char)Board_Symbols::BREAD;
					countBread++;
				}
				break;

			case (char)File_Symbols::LIMITS://%
				break;

			default:
				currChar = (char)File_Symbols::LIMITS;
			}

			board.setCell(i, j, currChar);
			j++;

			currChar = screenFile.get();

			if (currChar == END_LINE)
			{
				if (i == 0)
				{
					if (prevChar == (char)File_Symbols::LEGEND_FILE)
					{
						if (j + 19 > MAX_COLS)
						{
							board.setWidth(MAX_COLS);
						}
						else
						{
							board.setWidth(j + 19);
						}
					}
					else
					{
						board.setWidth(j);
					}
				}

				j = MAX_COLS;
			}

			if (screenFile.eof())
			{
				if (isLegendInLine)
				{
					if (i + 2 >= MAX_ROWS)
					{
						i = MAX_ROWS - 1;
					}
					else
					{
						i += 2;
					}
				}

				board.setHight(i + 1);
				j = MAX_COLS;
				i = MAX_ROWS;
			}
		}

		while (currChar != END_LINE && !screenFile.eof())
		{
			currChar = screenFile.get();
		}
	}

	if (pacman.getCurPos().getX() >= board.getWidth()) // checks the file vaildation
	{
		isHavePacman = false;
	}
	if (board.getLegendPos().getX() >= board.getWidth())
	{
		isHaveLegend = false;
	}

	if (isHavePacman && isHaveLegend)
	{
		isOpened = true;

		board.setCountBread(countBread);

		board.fixLegendArea();

		numOfGhosts = ghostCount;
	}
	else
	{
		isOpened = false;
		throw screenName[curScreen];
	}

}
//--------------------------------------------------------------------------------------//
void Game::printBoard() // print the game board
{
	int i, j;
	char currChar;

	gotoxy(0, 0);

	for (j = 0; j < board.getHight(); j++)
	{
		for (i = 0; i < board.getWidth(); i++)
		{
			currChar = board.getCell(j, i);

			if (isColorOn && currChar == (char)Board_Symbols::WALL)
			{
				setTextColor(Color::BLUE);
			}

			if (currChar == (char)File_Symbols::LIMITS)
			{
				currChar = (char)Board_Symbols::SPACE;
			}

			cout << currChar;
			if (isColorOn)
			{
				setTextColor(Color::WHITE);
			}
		}
		cout << endl;
	}
}
//--------------------------------------------------------------------------------------//
void Game::printScore(int score) // prints the current score status
{
	gotoxy(board.getLegendPos().getX() + 9, board.getLegendPos().getY());
	cout << score;
}
//--------------------------------------------------------------------------------------//
void Game::printLives(int lives) // prints the current lives status
{
	gotoxy(board.getLegendPos().getX() + 9, board.getLegendPos().getY() + 1);
	cout << lives;
	gotoxy(board.getLegendPos().getX() + 11, board.getLegendPos().getY() + 1);
	cout << "    ";

	if (isColorOn)
	{
		setTextColor(Color::LIGHTRED);
	}

	for (int i = 0; i < lives; i++)
	{
		gotoxy(board.getLegendPos().getX() + 11 + i, board.getLegendPos().getY() + 1);
		cout << (char)Board_Symbols::HEART;
	}
	if (isColorOn)
	{
		setTextColor(Color::WHITE);
	}
}
//--------------------------------------------------------------------------------------//
void Game::printMessegeWin(int score) // prints the win messege
{
	printBorder();

	gotoxy(2, 2); cout << "  db    db  .d88b.  db    db      db   d8b   db d888888b d8b   db ";
	gotoxy(2, 3); cout << "  `8b  d8' .8P  Y8. 88    88      88   I8I   88   `88'   888o  88 ";
	gotoxy(2, 4); cout << "   `8bd8'  88    88 88    88      88   I8I   88    88    88V8o 88 ";
	gotoxy(2, 5); cout << "     88    88    88 88    88      Y8   I8I   88    88    88 V8o88 ";
	gotoxy(2, 6); cout << "     88    `8b  d8' 88b  d88      `8b d8'8b d8'   .88.   88  V888 ";
	gotoxy(2, 7); cout << "     YP     `Y88P'  ~Y8888P'       `8b8' `8d8'  Y888888P VP   V8P ";


	gotoxy(2, 9); cout << " Your score is: " << score;

	gotoxy(2, 12); cout << "Press any key to return to the menu.";
}
//--------------------------------------------------------------------------------------//
void Game::printMessegeLose(int score) // prints the lose messege
{
	printBorder();

	gotoxy(2, 1); cout << "   _______       ___      .___  ___.  _______  ";
	gotoxy(2, 2); cout << "  /  _____|     /   \\     |   \\/   | |   ____| ";
	gotoxy(2, 3); cout << "  |  |  __     /  ^  \\    |  \\  /  | |  |__    ";
	gotoxy(2, 4); cout << "  |  | |_ |   /  /_\\  \\   |  |\\/|  | |   __|   ";
	gotoxy(2, 5); cout << "  |  |__| |  /  _____  \\  |  |  |  | |  |____  ";
	gotoxy(2, 6); cout << "   \\______| /__/     \\__\\ |__|  |__| |_______| ";
	gotoxy(2, 7); cout << "                                               ";
	gotoxy(2, 8); cout << "   ______   ____    ____  _______ .______      ";
	gotoxy(2, 9); cout << "  /  __  \\  \\   \\  /   / |   ____||   _  \\     ";
	gotoxy(2, 10); cout << " |  |  |  |  \\   \\/   /  |  |__   |  |_)  |    ";
	gotoxy(2, 11); cout << " |  |  |  |   \\      /   |   __|  |      /     ";
	gotoxy(2, 12); cout << " |  '--'  |    \\    /    |  |____ |  |\\  \\----.";
	gotoxy(2, 13); cout << "  \\______/      \\__/     |_______|| _| `._____|";

	gotoxy(2, 15); cout << " Your score is: " << score;

	gotoxy(2, 17); cout << " Press any key to return to the menu.";
}
//--------------------------------------------------------------------------------------//
void Game::printMessegePause() // prints the pause messege
{
	gotoxy(board.getLegendPos().getX(), board.getLegendPos().getY() + 2);
	cout << " ESC to continue   ";
}
//--------------------------------------------------------------------------------------//
void Game::printMessegeContinue() // prints the messege that is presented while the game is running
{
	gotoxy(board.getLegendPos().getX(), board.getLegendPos().getY() + 2);
	cout << " Press ESC to pause ";
}
//--------------------------------------------------------------------------------------//
void Game::printStartPositions() // prints the pacman and the ghosts in their initial positions
{
	goAndPrint(pacman.getStartPos().getX(), pacman.getStartPos().getY(), PACMAN_SYMBOL, pacman.getColor());
	for (int k = 0; k < numOfGhosts; k++)
	{
		goAndPrint((*ghosts[k]).getStartPos().getX(), (*ghosts[k]).getStartPos().getY(), GHOST_SYMBOL, (*ghosts[k]).getColor());
	}

}
//--------------------------------------------------------------------------------------//
void Game::pauseMode(char& key) // handles the game pause
{
	printMessegePause();
	goAndPrint(pacman.getCurPos().getX(), pacman.getCurPos().getY(), PACMAN_SYMBOL, pacman.getColor());
	for (int k = 0; k < numOfGhosts; k++)
	{
		goAndPrint((*ghosts[k]).getCurPos().getX(), (*ghosts[k]).getCurPos().getY(), GHOST_SYMBOL, (*ghosts[k]).getColor());
	}
	key = 0; // making 'key' different than 'ESC'

	while (key != (int)Keys::ESC)
	{
		key = _getch();
		if (isColorOn)
		{
			setTextColor(Color::LIGHTRED);
			printMessegePause();
			setTextColor(Color::WHITE);
		}
	}
	key = 0; // making 'key' different than 'ESC'
	printMessegeContinue();
}
//--------------------------------------------------------------------------------------//
bool Game::foodCheck(const Coord& pacmanCurPos) // checks if the pacman has 'eaten' food, returns true if all the food was eaten
{
	if (board.getCell(pacmanCurPos.getY(), pacmanCurPos.getX()) == (char)Board_Symbols::BREAD) //food check
	{
		pacman.increaseScore();
		if (!isSilent)
		{
			printScore(pacman.getScore());
		}
		board.setCell(pacmanCurPos.getY(), pacmanCurPos.getX(), (char)Board_Symbols::SPACE);
		board.decreaseCountBread();

		if (board.getCountBread() == 0) // if the user has 'eaten' all the food
		{
			return true;
		}
	}

	return false;
}
//--------------------------------------------------------------------------------------//
void Game::portalCheck() // checks if the pacman passed through a portal
{
	int pacmanX = pacman.getCurPos().getX();
	int pacmanY = pacman.getCurPos().getY();

	if (pacmanX == 0)  // portal in the left wall 
	{
		if (board.getCell(pacmanY, board.getWidth() - 1) == (char)Board_Symbols::WALL)
		{
			pacman.setDirection((char)Keys::RIGHT);
		}
		else
		{
			pacman.setDirection((char)Keys::LEFT);
			pacman.getCurPos().setX(board.getWidth() - 1);
		}

	}
	else if (pacmanX == board.getWidth() - 1)// portal in the right wall 
	{
		if (board.getCell(pacmanY, 0) == (char)Board_Symbols::WALL)
		{
			pacman.setDirection((char)Keys::LEFT);
		}
		else
		{
			pacman.setDirection((char)Keys::RIGHT);
			pacman.getCurPos().setX(0);
		}

	}
	else if (pacmanY == 0)// portal in the up wall 
	{
		if (board.getCell(board.getHight() - 1, pacmanX) == (char)Board_Symbols::WALL)
		{
			pacman.setDirection((char)Keys::DOWN);
		}
		else
		{
			pacman.setDirection((char)Keys::UP);
			pacman.getCurPos().setY(board.getHight() - 1);
		}

	}
	else if (pacmanY == board.getHight() - 1)// portal in the down wall 
	{
		if (board.getCell(0, pacmanX) == (char)Board_Symbols::WALL)
		{
			pacman.setDirection((char)Keys::UP);
		}
		else
		{
			pacman.setDirection((char)Keys::DOWN);
			pacman.getCurPos().setY(0);
		}

	}
}
//--------------------------------------------------------------------------------------//
void Game::printAnnouncment(string messege1, string messege2) // prints a messege to the user
{
	system("cls");

	printBorder();

	gotoxy(1, 1); cout << messege1;
	gotoxy(1, 2); cout << messege2;

	gotoxy(2, 22); cout << "Press any key to continue." << endl;

	pause();
}
//--------------------------------------------------------------------------------------//
void Game::loseLife(bool& isFruitFound, bool& isGhostMove, int& fruitIteration) // handles the pacman's lost of a life
{
	pacman.setNumOfLives(pacman.getNumOfLives() - 1);

	if (!isSilent)
	{
		printLives(pacman.getNumOfLives());
	}

	for (int k = 0; k < numOfGhosts; k++)
	{
		if (!isSilent)
		{
			goAndPrint((*ghosts[k]).getCurPos().getX(), (*ghosts[k]).getCurPos().getY(), board.getCell((*ghosts[k]).getCurPos().getY(), (*ghosts[k]).getCurPos().getX()));
		}
		(*ghosts[k]).setCountSteps(0);
	}

	resetPositions();

	isGhostMove = false;

	if (gameMode == (int)Game_Mode::LOAD)
	{
		fruit.resetFruit(fruitIteration);
	}
	else
	{
		fruit.resetFruit(isFruitFound, fruitIteration);
	}


	if (!isSilent)
	{
		goAndPrint(fruit.getCurPos().getX(), fruit.getCurPos().getY(), board.getCell(fruit.getCurPos().getY(), fruit.getCurPos().getX()));
	}
	fruit.setIsAppear(false);

	if (!isSilent)
	{
		printStartPositions();
	}
}
//--------------------------------------------------------------------------------------//
void Game::goAndPrint(int x, int y, char symbol) // go to the x,y postion on the screen and print the symbol 
{
	gotoxy(x, y);

	if (symbol == (char)File_Symbols::LIMITS)
	{
		cout << (char)Board_Symbols::SPACE;
	}
	else
	{
		cout << symbol;
	}

}
//--------------------------------------------------------------------------------------//
void Game::goAndPrint(int x, int y, char symbol, Color color) // go to the (x,y) postion on the screen and print the symbol in the color
{
	gotoxy(x, y);
	if (isColorOn)
	{
		setTextColor(color);
	}

	if (symbol == (char)File_Symbols::LIMITS)
	{
		cout << (char)Board_Symbols::SPACE;
	}
	cout << symbol;

	if (isColorOn)
	{
		setTextColor(Color::WHITE);
	}
}
//--------------------------------------------------------------------------------------//
void Game::colorMode()// set the color to the game 
{
	gotoxy(18, 13);

	if (!isColorOn)
	{
		pacman.setColor(Color::YELLOW);
		(*ghosts[0]).setColor(Color::LIGHTRED);
		(*ghosts[1]).setColor(Color::CYAN);
		(*ghosts[2]).setColor(Color::LIGHTMAGENTA);
		(*ghosts[3]).setColor(Color::BROWN);
		cout << "ON ";
	}
	else
	{
		pacman.setColor(Color::WHITE);
		(*ghosts[0]).setColor(Color::WHITE);
		(*ghosts[1]).setColor(Color::WHITE);
		(*ghosts[2]).setColor(Color::WHITE);
		(*ghosts[3]).setColor(Color::WHITE);
		cout << "OFF";
	}

	isColorOn = !isColorOn;
}
//--------------------------------------------------------------------------------------//
void Game::readResultFile(bool& isOpened) // opened the next result file, read the data from it, and writes it to the matching data members
{
	int count = 0;
	int currRes;
	ifstream resultFile(resultName[curScreen]);

	if (!resultFile.is_open()) // fails to open
	{
		isOpened = false;
		resultFile.close();
		throw resultName[curScreen];
	}

	resultFile >> currRes;

	if (resultFile.eof())// the file is empty
	{
		isOpened = false;
		resultFile.close();
		throw resultName[curScreen];
	}

	while (count < 4)
	{
		if (currRes == END_OF_INT_VEC)
		{
			resultFile >> currRes;

			if (resultFile.eof())// finish time is missing.
			{
				isOpened = false;
				resultFile.close();
				throw resultName[curScreen];
			}
			else
			{
				pacman.setResultsFinishScreen(currRes);
				isOpened = true;
				resultFile.close();
				reverse(pacman.getResultsLoseLifeVec().begin(), pacman.getResultsLoseLifeVec().end());
				return;
			}
		}
		else
		{
			count++;
			pacman.addResLoseLife(currRes);
			resultFile >> currRes;
		}

	}

	isOpened = false;
	resultFile.close();
	throw resultName[curScreen];
}
//--------------------------------------------------------------------------------------//
void Game::loadScreenByFileName()
{
	string buffer;
	ifstream screenFile;

	system("cls");

	printBorder();

	screenName.clear();

	gotoxy(1, 1); cout << "Enter a file name: ";
	getline(cin, buffer);

	if (!buffer.ends_with(".screen")) // check vaild file name
	{
		customScreenMode = false;
		printAnnouncment((string)"File is illigal. Enter a '.screen' file type only.");

		return;
	}

	screenFile.open(buffer); // checks if the file is valid and can be opened

	if (!screenFile.is_open()) // fails to open
	{
		customScreenMode = false;
		printAnnouncment((string)"Failed to open the file.");

		return;
	}

	screenFile.close();

	customScreenMode = true;

	screenName.push_back(buffer);

}
//--------------------------------------------------------------------------------------//
void Game::writeResultFile() // opened the next result file, writes the data to it, from the matching data members
{
	ofstream resultFile(resultName[curScreen]);

	if (!resultFile.is_open()) // fails to open
	{
		throw false;
	}

	for (auto i : pacman.getResultsLoseLifeVec())
	{
		resultFile << i << (char)Board_Symbols::SPACE;
	}
	resultFile << END_OF_INT_VEC << (char)Board_Symbols::SPACE << pacman.getResultsFinishScreen() << (char)Board_Symbols::SPACE;

	resultFile.close();

}
//--------------------------------------------------------------------------------------//
void Game::writeStepsFile() // opened the next steps file, writes the data to it, from the matching data members
{
	int k;
	ofstream stepsFile(stepsName[curScreen]);

	if (!stepsFile.is_open()) // fails to open
	{
		throw false;
	}

	stepsFile << pacman.getSteps() << END_LINE;// write pacman steps

	for (k = 0; k < numOfGhosts; k++)
	{
		stepsFile << (*ghosts[k]).getSteps() << END_LINE;// write ghost steps
	}

	stepsFile << fruit.getSteps() << END_LINE;// write fruit steps

	for (auto i : fruit.getHideAndShowVec())
	{
		if (i != 0)
		{
			stepsFile << i << (char)Board_Symbols::SPACE;// write hide and show time 
		}
	}
	stepsFile << END_OF_INT_VEC << END_LINE;

	stepsFile << fruit.getFruitValues() << END_LINE;// write the value of the fruit

	for (auto i : fruit.getPositionsVec())
	{
		stepsFile << i.getX() << (char)Board_Symbols::SPACE << i.getY() << (char)Board_Symbols::SPACE;
	}
	stepsFile.close();
}
//--------------------------------------------------------------------------------------//
void Game::readStepsFile(bool& isOpened) // opened the next steps file, read the data from it, and writes it to the matching data members
{
	int currNum, currX, currY, count = 0;
	ifstream stepsFile(stepsName[curScreen]);

	if (!stepsFile.is_open()) // fails to open
	{
		isOpened = false;
		stepsFile.close();
		throw stepsName[curScreen];
	}
	try {
		pacman.addMoves(stepsFile, isOpened); // gets pacman's moves from file

		for (int i = 0; i < numOfGhosts; i++)
		{
			ghosts[i]->addMoves(stepsFile, isOpened); // gets ghosts' moves from file
		}

		fruit.addMoves(stepsFile, isOpened); // gets fruit's moves from file

		//to add get value of the fruits
		//to do revers to all fruit vectors

		stepsFile >> currNum;
		while (currNum != END_OF_INT_VEC) // gets hide and show times for fruit
		{
			fruit.addHideAndShow(currNum);
			stepsFile >> currNum;

			if (stepsFile.eof())
			{
				isOpened = false;
				stepsFile.close();
				throw "input error";
			}
		}
		reverse(fruit.getHideAndShowVec().begin(), fruit.getHideAndShowVec().end());
		stepsFile.get(); // gets the 'endl'

		fruit.addFruitValues(stepsFile, isOpened); // gets the fruit values

		stepsFile >> currX;
		if (!stepsFile.eof())
		{
			count++;
		}
		stepsFile >> currY;
		if (!stepsFile.eof())
		{
			count++;
		}
		while (!stepsFile.eof())
		{
			fruit.addPositions(Coord(currX, currY));
			stepsFile >> currX;
			if (!stepsFile.eof())
			{
				count++;
			}
			stepsFile >> currY;
			if (!stepsFile.eof())
			{
				count++;
			}
		}

		if (count % 2 != 0)
		{
			isOpened = false;
			stepsFile.close();
			throw stepsName[curScreen];
		}
		reverse(fruit.getPositionsVec().begin(), fruit.getPositionsVec().end());
	}

	catch (const char*)
	{
		isOpened = false;
		stepsFile.close();
		throw stepsName[curScreen];
	}

}
//--------------------------------------------------------------------------------------//
void Game::ghostLevelMode() // opens the ghosts mode selection menu
{
	char choice;

	printBorder();

	gotoxy(1, 1); cout << "Select ghost level: ";

	gotoxy(1, 2); cout << "(1) BEST    (2) GOOD    (3) NOVICE ";

	choice = _getch();// to avoid if the user insert char

	while (choice != (char)Choices::ONE && choice != (char)Choices::TWO && choice != (char)Choices::THREE)
	{
		gotoxy(1, 4);
		cout << "Invalid choice. " << "Please enter a valid choice." << endl;

		if (isColorOn) { setTextColor(Color::WHITE); }

		choice = _getch();

		if (isColorOn) { setTextColor(Color::LIGHTRED); }
	}
	if (isColorOn) { setTextColor(Color::WHITE); }

	this->ghostsLevel = (int)choice - (char)Choices::ONE;

}
//--------------------------------------------------------------------------------------//
void Game::printBorder() // prints the borders of the screen
{
	int i, j;
	system("cls");

	if (isColorOn)
	{
		setTextColor(Color::MAGENTA);
	}

	for (i = 1; i <= MAX_ROWS; i++)
	{
		for (j = 1; j <= MAX_COLS; j++)
		{
			if (i == 1 || i == MAX_ROWS)
			{
				cout << (char)Board_Symbols::FRAME;
			}
			else if (j == 1 || j == MAX_COLS)
			{
				cout << (char)Board_Symbols::FRAME;
			}
			else
				cout << (char)Board_Symbols::SPACE;
		}
		cout << endl;
	}

	if (isColorOn)
	{
		setTextColor(Color::WHITE);
	}
}
//--------------------------------------------------------------------------------------//
void Game::printBackground() // prints the 25x80 rectangle background of the screen
{
	int i, j;
	system("cls");

	if (isColorOn)
	{
		setTextColor(Color::MAGENTA);
	}

	for (i = 1; i <= MAX_ROWS; i++)
	{
		for (j = 1; j <= MAX_COLS; j++)
		{
			cout << (char)Board_Symbols::FRAME;
		}
		cout << endl;
	}
	if (isColorOn)
	{
		setTextColor(Color::WHITE);
	}
}
//--------------------------------------------------------------------------------------//
void Game::printGameInfoSideBar() // prints the structure of the legend 
{
	gotoxy(board.getLegendPos().getX(), board.getLegendPos().getY());
	cout << " Score:             ";

	gotoxy(board.getLegendPos().getX(), board.getLegendPos().getY() + 1);
	cout << " Lives:             ";
}
//--------------------------------------------------------------------------------------//
void Game::printAnnouncment(string messege) // prints a messege to the user
{
	system("cls");

	printBorder();

	gotoxy(1, 1); cout << messege;

	gotoxy(2, 22); cout << "Press any key to continue." << endl;

	pause();
}
//--------------------------------------------------------------------------------------//
void Game::resetPositions() // reset pacman and ghost if the user lose life 
{
	pacman.getCurPos().setX(pacman.getStartPos().getX());
	pacman.getCurPos().setY(pacman.getStartPos().getY());
	pacman.setDirection((char)Keys::STAY);
	for (int k = 0; k < numOfGhosts; k++)
	{
		(*ghosts[k]).getCurPos().setX((*ghosts[k]).getStartPos().getX());
		(*ghosts[k]).getCurPos().setY((*ghosts[k]).getStartPos().getY());
		(*ghosts[k]).setDirection((*ghosts[k]).randomDirection());
	}
}
//--------------------------------------------------------------------------------------//
void Game::resetGame() // reset the score, life, pacman, ghosts and the board for new game 
{
	customScreenMode = false;
	screenName.clear();
	resultName.clear();
	stepsName.clear();
	pacman.setNumOfLives(3);
	pacman.setScore(0);
	resetPositions();
}
//--------------------------------------------------------------------------------------//
void Game::move() // handles the movment of the ghosts, fruit and the pacman during the game
{
	char key = (char)Keys::STAY;
	bool isGhostMove = true;
	bool isFruitFound = false;
	bool isWin = false;
	bool isLose = false;
	bool isInFiveSteps = false;
	int k = 0;
	Coord temp;
	int sleepTime = 100;
	int iterationCount = 0;
	int fruitIteration = 0;
	int fruitMoveCount = 0;

	if (gameMode == (int)Game_Mode::LOAD)
	{
		setSleepTime(sleepTime, isFruitFound);
	}

	initPositions(fruitIteration);

	while ((!_kbhit() || key != (int)Keys::ESC) && (!isWin) && (!isLose))
	{
		if (gameMode != (int)Game_Mode::LOAD)
		{
			if (_kbhit())
			{
				key = _getch();

				if (key == (int)Keys::ESC) // pause mode
				{
					pauseMode(key);
				}
				pacman.setDirection(key);
			}

			pacman.addMove(pacman.getDirection());

			if (!fruit.getIsAppear() && fruit.getHideTime() == 0) // shows the fruit
			{
				nextShowFruitSAVE(isFruitFound, fruitIteration);
			}

			if (fruit.getIsAppear() && fruit.getShowTime() == 0) // hides the fruit
			{
				nextHideFruitSAVE(isFruitFound, fruitIteration);
			}
		}
		else
		{
			pacman.setDirection(pacman.nextMove());

			if (!fruit.getIsAppear() && fruitIteration == 0) // shows the fruit
			{
				nextShowFruitLOAD(isFruitFound, fruitIteration);
			}

			if (fruit.getIsAppear() && fruitIteration == 0) // hides the fruit
			{
				nextHideFruitLOAD(fruitIteration);
			}
		}

		if (!isSilent)
		{
			printCreatures(fruitMoveCount, isGhostMove, isFruitFound);
		}

		isGhostMove = !isGhostMove;// "tic on tic off" (makes the ghosts' movement work in each second iteration) for the move of the ghost

		if (fruit.getIsAppear() && isFruitFound) // makes the fruit move once in every 3 moves of the pacman
		{
			fruitMoveCount++;

			if (fruitMoveCount == 3)
			{
				fruitMoveCount = 0;
			}
		}

		Sleep(sleepTime);//delay for the user to see the pacman 

		if (!isSilent)
		{
			eraseCreatures(fruitMoveCount, isGhostMove, isFruitFound);
		}

		isWin = foodCheck(pacman.getCurPos());

		portalCheck();

		pacman.pacmanMove(board); //pacman's move

		checkMeetPG(iterationCount, fruitIteration, fruitMoveCount, isGhostMove, isFruitFound);

		checkMeetPF(isFruitFound, fruitIteration, fruitMoveCount);

		if (isGhostMove) //ghosts' move
		{
			for (k = 0; k < numOfGhosts; k++)
			{
				if (gameMode == (int)Game_Mode::LOAD)
				{
					(*ghosts[k]).setDirection((*ghosts[k]).nextMove());
				}
				(*ghosts[k]).ghostMove(board, pacman.getCurPos(), ghostsLevel, ghosts, numOfGhosts, isInFiveSteps, gameMode);

				if (gameMode == (int)Game_Mode::LOAD)
				{
					(*ghosts[k]).setCountSteps(0);
				}

			}
		}

		checkMeetPG(iterationCount, fruitIteration, fruitMoveCount, isGhostMove, isFruitFound);



		checkMeetFG(isFruitFound, fruitIteration, fruitMoveCount);

		if (gameMode != (int)Game_Mode::LOAD)
		{
			if (fruit.getIsAppear() && fruit.getShowTime() != 0 && fruitMoveCount == 0 && isFruitFound) //fruit's move
			{
				fruit.fruitMove(board, gameMode);

			}
		}
		else // in load mode.
		{
			if (fruit.getIsAppear() && fruitIteration != 0 && fruitMoveCount == 0)
			{
				char currStep = fruit.nextMove();

				if (currStep == EMPTY_STR)
				{
					fruit.setIsAppear(false);
				}
				else
				{
					fruit.setDirection(currStep);
					fruit.fruitMove(board, gameMode);
				}

			}
		}

		if (pacman.getNumOfLives() == 0) // if the user has lost all his lives
		{
			isLose = true;
		}

		checkMeetPF(isFruitFound, fruitIteration, fruitMoveCount);

		checkMeetFG(isFruitFound, fruitIteration, fruitMoveCount);

		if (gameMode != (int)Game_Mode::LOAD)
		{
			incDecTime();
		}

		if (!isWin && !isLose)
		{
			iterationCount++;

			if (gameMode == (int)Game_Mode::LOAD)
			{
				fruitIteration--;
			}
			else
			{
				fruitIteration++;
			}

		}
	}

	if (isWin) // checks if the user has won the game (has eaten all the breads in the current screen)
	{
		if (gameMode == (int)Game_Mode::SAVE)
		{
			fruit.addHideAndShow(fruitIteration);
			fruitIteration = 0;

			pacman.setResultsFinishScreen(iterationCount);

			if (fruitMoveCount == 0 && fruit.getIsAppear())
			{
				fruit.addMove(fruit.getDirection());
			}
		}

		if (gameMode == (int)Game_Mode::LOAD)
		{
			if (iterationCount != pacman.getResultsFinishScreen())
			{
				throw (string)"Test failed. Finish screen value is incorrect.";
			}
		}

		if (curScreen == screenName.size() - 1)
		{
			if (gameMode != (int)Game_Mode::LOAD)
			{
				printMessegeWin(pacman.getScore());
			}

		}
	}

	if (isLose) // checks if the user has lost the game (lose all the lives)
	{
		if (gameMode == (int)Game_Mode::SAVE)
		{
			pacman.setResultsFinishScreen(iterationCount);
		}

		if (gameMode == (int)Game_Mode::LOAD)
		{
			if (iterationCount != pacman.getResultsFinishScreen())
			{
				throw (string)"Test failed. Finish screen value is incorrect.";
			}
		}
		if (gameMode != (int)Game_Mode::LOAD)
		{
			printMessegeLose(pacman.getScore());
		}

		curScreen = screenName.size() - 1;
	}
}
//--------------------------------------------------------------------------------------//
void Game::setSleepTime(int& sleepTime, bool& isFruitFound) // sets the game sleep time according to the game mode
{
	isFruitFound = true;
	if (isSilent)
	{
		sleepTime = 0;
	}
	else
	{
		sleepTime = 20;
	}
}
//--------------------------------------------------------------------------------------//
void Game::initPositions(int& fruitIteration)  // sets the creatures' positions to the starting positions
{
	int k;

	pacman.setCurPos(pacman.getStartPos()); // initials the positions
	for (k = 0; k < numOfGhosts; k++)
	{
		(*ghosts[k]).setCurPos((*ghosts[k]).getStartPos());
	}

	if (gameMode != (int)Game_Mode::LOAD)
	{
		fruit.setHideTime(fruit.randomBetween(10, 120));
	}
	else // in load mode
	{
		fruitIteration = fruit.nextTime();
	}
}
//--------------------------------------------------------------------------------------//
void Game::nextShowFruitSAVE(bool& isFruitFound, int& fruitIteration) // shows the next fruit, in SAVE mode
{
	int k;
	char currCell;

	fruit.setCurPosX(fruit.randomBetween(0, board.getWidth() - 1));
	fruit.setCurPosY(fruit.randomBetween(0, board.getHight() - 1));

	currCell = board.getCell(fruit.getCurPos().getY(), fruit.getCurPos().getX());

	for (k = 0; (k < 50) && !isFruitFound; k++) // tries to find a position to the fruit (50 tries)
	{
		if (currCell == (char)Board_Symbols::WALL || currCell == (char)File_Symbols::LIMITS)
		{
			fruit.setCurPosX(fruit.randomBetween(0, board.getWidth() - 1));
			fruit.setCurPosY(fruit.randomBetween(0, board.getHight() - 1));
		}
		else
		{
			isFruitFound = true;
		}
		currCell = board.getCell(fruit.getCurPos().getY(), fruit.getCurPos().getX());
	}

	if (isFruitFound)
	{
		fruit.setIsAppear(!fruit.getIsAppear());
		fruit.addHideAndShow(fruitIteration);
		fruitIteration = 0;

		fruit.setShowTime(fruit.randomBetween(120, 200));
		fruit.setFruitVal(fruit.randomBetween(5, 9) + '0');
		fruit.setDirection(fruit.randomDirection());
		goAndPrint(fruit.getCurPos().getX(), fruit.getCurPos().getY(), fruit.getFruitVal());
		fruit.addFruitValue(fruit.getFruitVal());
		fruit.addPositions(fruit.getCurPos());
	}
}
//--------------------------------------------------------------------------------------//
void Game::nextHideFruitSAVE(bool& isFruitFound, int& fruitIteration) // hides the  fruit, in SAVE mode
{
	fruit.setIsAppear(!fruit.getIsAppear());
	fruit.addHideAndShow(fruitIteration);
	fruitIteration = 0;

	isFruitFound = false;
	fruit.setHideTime(fruit.randomBetween(10, 120));
	goAndPrint(fruit.getCurPos().getX(), fruit.getCurPos().getY(), board.getCell(fruit.getCurPos().getY(), fruit.getCurPos().getX()));

}
//--------------------------------------------------------------------------------------//
void Game::nextShowFruitLOAD(bool& isFruitFound, int& fruitIteration) // shows the next fruit, in LOAD mode
{
	Coord temp;

	temp = fruit.nextPos();
	fruit.setCurPos(temp);

	isFruitFound = true;
	fruit.setIsAppear(!fruit.getIsAppear());
	fruitIteration = fruit.nextTime();
	fruit.setFruitVal(fruit.nextVal());

	if (!isSilent)
	{
		goAndPrint(fruit.getCurPos().getX(), fruit.getCurPos().getY(), fruit.getFruitVal());
	}
}
//--------------------------------------------------------------------------------------//
void Game::nextHideFruitLOAD(int& fruitIteration) // hides the fruit, in LOAD mode
{
	fruit.setIsAppear(!fruit.getIsAppear());
	fruitIteration = fruit.nextTime();

	if (!isSilent)
	{
		goAndPrint(fruit.getCurPos().getX(), fruit.getCurPos().getY(), board.getCell(fruit.getCurPos().getY(), fruit.getCurPos().getX()));
	}
}
//--------------------------------------------------------------------------------------//
void Game::printCreatures(int& fruitMoveCount, bool& isGhostMove, bool& isFruitFound) // prints all the creatures in their current positions
{
	int k;

	goAndPrint(pacman.getCurPos().getX(), pacman.getCurPos().getY(), PACMAN_SYMBOL, pacman.getColor());// print the pacman in the current postion

	if (isGhostMove)
	{
		for (k = 0; k < numOfGhosts; k++)
		{
			goAndPrint((*ghosts[k]).getCurPos().getX(), (*ghosts[k]).getCurPos().getY(), GHOST_SYMBOL, (*ghosts[k]).getColor());// print the ghosts in the current postion
		}

	}

	if (fruit.getIsAppear() && fruitMoveCount == 0 && isFruitFound) // print the fruit in the current postion
	{
		goAndPrint(fruit.getCurPos().getX(), fruit.getCurPos().getY(), fruit.getFruitVal());
	}
}
//--------------------------------------------------------------------------------------//
void Game::eraseCreatures(int& fruitMoveCount, bool& isGhostMove, bool& isFruitFound) // ereases all the creatures from their current positions
{
	int k;

	goAndPrint(pacman.getCurPos().getX(), pacman.getCurPos().getY(), (char)Board_Symbols::SPACE);//delete pacman for the current postion

	if (isGhostMove) //delete the ghosts in the current postion
	{
		for (k = 0; k < numOfGhosts; k++)
		{
			goAndPrint((*ghosts[k]).getCurPos().getX(), (*ghosts[k]).getCurPos().getY(), board.getCell((*ghosts[k]).getCurPos().getY(), (*ghosts[k]).getCurPos().getX()));
		}
	}

	if (fruit.getIsAppear() && fruitMoveCount == 0 && isFruitFound) //delete the fruit in the current postion
	{
		goAndPrint(fruit.getCurPos().getX(), fruit.getCurPos().getY(), board.getCell(fruit.getCurPos().getY(), fruit.getCurPos().getX()));
	}
}
//--------------------------------------------------------------------------------------//
void Game::checkMeetPG(int& iterationCount, int& fruitIteration, int& fruitMoveCount, bool& isGhostMove, bool& isFruitFound) // handles pacman and ghost meeting
{
	int k;

	for (k = 0; k < numOfGhosts; k++)
	{
		if (pacman.getCurPos().samePlace((*ghosts[k]).getCurPos())) //check if the pacman meets a ghost "accidentally" after update pacman postion
		{
			if (gameMode == (int)Game_Mode::LOAD)
			{
				if (iterationCount != pacman.nextValLoseLife())//getValLoseLifeInCell(3 - pacman.getNumOfLives()))
				{
					throw (string)"Test failed. Lose life values are incorrect.";
				}
			}

			if (gameMode == (int)Game_Mode::SAVE)
			{
				fruit.addHideAndShow(fruitIteration);
				fruitIteration = 0;

				pacman.addResLoseLife(iterationCount);

			}
			loseLife(isFruitFound, isGhostMove, fruitIteration);
			fruitMoveCount = 0;

			k = numOfGhosts;
		}
	}
}
//--------------------------------------------------------------------------------------//
void Game::checkMeetPF(bool& isFruitFound, int& fruitIteration, int& fruitMoveCount) // handles pacman and fruit meeting
{
	if (pacman.getCurPos().samePlace(fruit.getCurPos()) && isFruitFound) //check if the pacman meets the fruit after update pacman postion
	{
		pacman.increaseScore(fruit.getFruitVal() - CHAR_ZERO);
		if (!isSilent)
		{
			printScore(pacman.getScore());
		}

		if (gameMode == (int)Game_Mode::LOAD)
		{
			fruit.resetFruit(fruitIteration);
			isFruitFound = false;
		}
		else
		{
			fruit.resetFruit(isFruitFound, fruitIteration);
		}
		fruitMoveCount = 0;
	}
}
//--------------------------------------------------------------------------------------//
void Game::checkMeetFG(bool& isFruitFound, int& fruitIteration, int& fruitMoveCount) // handles fruit and ghost meeting
{
	int k;

	for (k = 0; k < numOfGhosts; k++) //check if the fruit meets a ghost after update the ghosts postion
	{
		if ((*ghosts[k]).getCurPos().samePlace(fruit.getCurPos()) && isFruitFound)
		{
			if (gameMode == (int)Game_Mode::LOAD)
			{
				fruit.resetFruit(fruitIteration);
				isFruitFound = false;
			}
			else
			{

				fruit.resetFruit(isFruitFound, fruitIteration);

			}
			fruitMoveCount = 0;
		}
	}
}
//--------------------------------------------------------------------------------------//
void Game::incDecTime() // decreases hide time or show time, by one
{
	if (!fruit.getIsAppear()) // decrease show or hide time of the fruit
	{
		if (fruit.getHideTime() > 0)
		{
			fruit.decreaseHideTime();
		}
	}
	else
	{
		if (fruit.getShowTime() > 0)
		{
			fruit.decreaseShowTime();
		}
	}
}
//--------------------------------------------------------------------------------------//