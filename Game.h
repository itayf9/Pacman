#pragma once

#include <iostream>
#include <conio.h>
#include <windows.h>
#include "Pacman.h"
#include "Ghost.h"
#include "Fruit.h"
#include "GameObject.h"

namespace fs = std::filesystem;

//--------------------------------------------------------------------------------------//
class Game
{
private:
	Board          board;
	Pacman         pacman;
	Ghost** ghosts;
	int            numOfGhosts = 0;
	int            ghostsLevel = -1;
	Fruit          fruit;
	bool           isColorOn = false;
	bool           customScreenMode = false;
	int		   	   gameMode = (int)Game_Mode::SIMPLE;
	bool		   isSilent = false;
	vector<string> screenName;
	vector<string> resultName;
	vector<string> stepsName;
	int            curScreen;

public:
	//--constructor and destructor----------------------------------------------------------//
	Game();
	Game(const Game& other);
	~Game();

	Game& operator=(const Game& g) {
		if (&g != this)
		{
			delete ghosts[0];
			delete ghosts[1];
			delete ghosts[2];
			delete ghosts[3];
			delete[] ghosts;
			copyGame(g);
		}

		return *this;
	}

	void copyGame(const Game& other);
	void readCMDParameters(int& argc, char** argv); // reads the parameters from the CMD

//-------function using to start the game ----------------------------------------------//
	bool startProgram(); // presents the menu, and handles the menu options 
	int  checkVaildChoice(); // gets an input from the user, and checks the validation of it
	void startGame(); // starts a new game, prints the game board and handles a running game
	void readScreenFile(bool& isOpened); // opened the next screen file, read the data from it, and writes it to the 'board'
	void readResultFile(bool& isOpened); // opened the next result file, read the data from it, and writes it to the matching data members
	void readStepsFile(bool& isOpened); // opened the next steps file, read the data from it, and writes it to the matching data members
	void writeResultFile(); // opened the next result file, writes the data to it, from the matching data members
	void writeStepsFile(); // opened the next steps file, writes the data to it, from the matching data members
	void colorMode(); // set the color to the game
	void loadScreenByFileName(); // gets a name of a screen file, and in the next game opens only that screen
	void getBySuffix(vector<string>& name, string subfix); // gets all files ending with 'subfix' from the directory
	void createResultAndStepsFile(); // creates new '.steps' and new '.results' files with the same name
	void ghostLevelMode(); // opens the ghosts mode selection menu
//-------print function----------------------------------------------------------------//
	void printMenu(); // prints the menu
	void printIstructions(); // prints the instructions menu
	void printBoard(); // print the game board
	void printScore(int score); // prints the current score status
	void printLives(int lives); // prints the current lives status
	void printMessegeWin(int score); // prints the win messege
	void printMessegeLose(int score); // prints the lose messege
	void printMessegePause(); // prints the pause messege
	void printMessegeContinue(); // prints the messege that is presented while the game is running
	void printStartPositions(); // prints the pacman and the ghosts in their initial positions
	void printBorder(); // prints the borders of the screen
	void printBackground(); // prints the 25x80 rectangle background of the screen
	void printGameInfoSideBar(); // prints the structure of the legend
	void printAnnouncment(string messege); // prints a messege to the user
	void printAnnouncment(string messege1, string messege2); // prints a messege to the user
//-------function using in the game ----------------------------------------------------//
	void pauseMode(char& key); // handles the game pause
	bool foodCheck(const Coord& pacmanCurPos); // checks if the pacman has 'eaten' food, returns true if all the food was eaten
	void portalCheck(); // checks if the pacman passed through a portal
	void loseLife(bool& isFruitFound, bool& isGhostMove, int& fruitIteration); // handles the pacman's lost of a life
	void goAndPrint(int x, int y, char symbol); // go to the x,y postion on the screen and print the symbol 
	void goAndPrint(int x, int y, char symbol, Color color); // go to the (x,y) postion on the screen and print the symbol in the color

	void move(); // handles the movment of the ghosts, fruit and the pacman during the game
	void setSleepTime(int& sleepTime, bool& isFruitFound); // sets the game sleep time according to the game mode
	void initPositions(int& fruitIteration); // sets the creatures' positions to the starting positions
	void nextShowFruitSAVE(bool& isFruitFound, int& fruitIteration); // shows the next fruit, in SAVE mode
	void nextHideFruitSAVE(bool& isFruitFound, int& fruitIteration); // hides the  fruit, in SAVE mode
	void nextShowFruitLOAD(bool& isFruitFound, int& fruitIteration); // shows the next fruit, in LOAD mode
	void nextHideFruitLOAD(int& fruitIteration); // hides the fruit, in LOAD mode
	void printCreatures(int& fruitMoveCount, bool& isGhostMove, bool& isFruitFound); // prints all the creatures in their current positions
	void eraseCreatures(int& fruitMoveCount, bool& isGhostMove, bool& isFruitFound); // ereases all the creatures from their current positions
	void checkMeetPG(int& iterationCount, int& fruitIteration, int& fruitMoveCount, bool& isGhostMove, bool& isFruitFound); // handles pacman and ghost meeting
	void checkMeetPF(bool& isFruitFound, int& fruitIteration, int& fruitMoveCount); // handles pacman and fruit meeting
	void checkMeetFG(bool& isFruitFound, int& fruitIteration, int& fruitMoveCount); // handles fruit and ghost meeting
	void incDecTime(); // decreases hide time or show time, by one
//-------function to reset the game-----------------------------------------------------//
	void resetPositions(); // reset pacman and ghost if the user lose life 
	void resetGame(); // reset the score, life, pacman, ghosts and the board for new game 
};