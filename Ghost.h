#pragma once

#include "GameObject.h"

const char GHOST_SYMBOL = (char)0036;
const int DEFAULT_CHANGE_TIME = 20;
const string levelArr[] = { "BEST", "GOOD", "NOVICE" };

class Ghost : public GameObject
{
private:
	int countSteps = 0;
	int changeDirInStep = DEFAULT_CHANGE_TIME;
	Coord startPos;
	Color color = Color::WHITE;

public:
	enum class level { BEST = 0, GOOD = 1, NOVICE = 2 };

	//--constructor-------------------------------------------------------------------------//
	Ghost();
	Ghost(int startPosX, char direction);
	//---------getter-----------------------------------------------------------------------//
	Color  getColor() const; // returns 'color'
	int getCountSteps() const { return this->countSteps; } // returns 'countSteps'
	Coord& getStartPos(); // returns 'startPos'
//-------setter-------------------------------------------------------------------------//
	void setColor(Color color); // sets a new value to 'color'
	void setCountSteps(int count) { this->countSteps = count; } // sets a new value to 'countSteps'
	void setStartPos(const Coord& startPos); // sets a new value to 'startPos'
//-------funcion to class ghost---------------------------------------------------------//
	void increaseStep() { ++countSteps; } // increases the 'countStep' value by one
	void ghostMove(const Board& board, Coord& pacmanPos, const int level, Ghost** ghosts, const int numOfGhosts, bool& isInFiveSteps, int gameMode); // handles the ghost movement
	void smartGhost(const Board& board, const Coord& pacmanPos, Ghost** ghosts, const int numOfGhosts, int gameMode); // handles the BEST case of ghosts mode
	void goodGhost(const Board& board, const Coord& pacmanPos, Ghost** ghosts, const int numOfGhosts, bool& isInFiveSteps, int gameMode); // handles the GOOD case of ghosts mode
	void noviceGhost(const Board& board, Ghost** ghosts, const int numOfGhosts, int gameMode); // handles the NOVICE case of ghosts mode
	void BFS(const Board& board, const Coord& pacmanPos, string& path); // uses the BFS algorithem to find a 'path' from the ghost to the pacman
	char convertDirection(const Coord& child, const Coord& parent); // compares two close coords and returns the direction between the child and the parent
	void exploreNeighbours(const Board& board, const Coord& nodeP, vector<Coord>& neighbours); // find all accessible neighbours of a coord on board
	void stringReverse(string& path); // converts the directions to their opposites and reverses the string of them
};