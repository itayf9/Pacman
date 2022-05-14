#pragma once

#include "GameObject.h"

const  char PACMAN_SYMBOL = (char)002;
const int MAX_LIVES = 3;

class Pacman : public GameObject
{
private:

	int numOfLives;
	int score;
	Coord startPos;
	vector<int> resultsLoseLife;
	int resultsFinishScreen;
	Color color = Color::WHITE;

public:

	//--constructor-------------------------------------------------------------------------//
	Pacman();
	//---------getter-----------------------------------------------------------------------//
	int    getNumOfLives() const; // returns 'numOfLives'
	int    getScore() const; // returns 'score'
	Coord& getStartPos(); // returns 'startPos'
	Color  getColor() const; // returns 'color'
	int getResultsFinishScreen() const; // returns the results Finish Screen
	vector<int>& getResultsLoseLifeVec(); // returns the results Lose Life vector
//-------setter-------------------------------------------------------------------------//
	void setNumOfLives(int numOfLives); // sets a new value to 'numOfLives'
	void setScore(int score); // sets a new value to 'score'
	void setStartPos(const Coord& startPos); // sets a new value to 'startPos'
	void setColor(Color color); // sets a new value to 'color'
	void setResultsFinishScreen(int finish); // sets a new value to 'resultsFinishScreen'
//-------funcion to class pacman--------------------------------------------------------//
	void increaseScore(); //increase score by 1
	void increaseScore(int score); //increase score by 'score'
	void addResLoseLife(int lose); // add a new lose life value to the vector
	int nextValLoseLife(); // pop the next value from 'loseLife' vector
	void pacmanMove(const Board& board); // handles the pacman's movement
};