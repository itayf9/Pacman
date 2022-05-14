#include "Pacman.h"

Pacman::Pacman() : GameObject(INT_ZERO, INT_ZERO, (char)Keys::STAY), startPos(), numOfLives(MAX_LIVES), score(INT_ZERO), resultsFinishScreen(INT_ZERO)
{
	resultsLoseLife.clear();
}
//--------------------------------------------------------------------------------------//
int Pacman::getNumOfLives() const // returns 'numOfLives'
{
	return this->numOfLives;
}
//--------------------------------------------------------------------------------------//
int Pacman::getScore() const // returns 'score'
{
	return this->score;
}
//--------------------------------------------------------------------------------------//
Color Pacman::getColor() const // returns 'color'
{
	return this->color;
}
//--------------------------------------------------------------------------------------//
Coord& Pacman::getStartPos() // returns 'startPos'
{
	return this->startPos;
}
//--------------------------------------------------------------------------------------//
void Pacman::setNumOfLives(int numOfLives)  // sets a new value to 'numOfLives'
{
	this->numOfLives = numOfLives;
}
int Pacman::getResultsFinishScreen() const
{
	return this->resultsFinishScreen;
}
//--------------------------------------------------------------------------------------//
void Pacman::setScore(int score)  // sets a new value to 'score'
{
	this->score = score;
}
//--------------------------------------------------------------------------------------//
vector<int>& Pacman::getResultsLoseLifeVec() // returns the results Lose Life vector
{
	return this->resultsLoseLife;
}
//--------------------------------------------------------------------------------------//
void Pacman::setColor(Color color)  // sets a new value to 'color'
{
	this->color = color;
}
//--------------------------------------------------------------------------------------//
void Pacman::setStartPos(const Coord& startPos) // sets a new value to 'startPos'
{
	this->startPos = startPos;
}
//--------------------------------------------------------------------------------------//
void Pacman::increaseScore()  //increase score by 1
{
	this->score++;
}
//--------------------------------------------------------------------------------------//
void Pacman::increaseScore(int score) //increase score by 'score'
{
	this->score += score;
}
//--------------------------------------------------------------------------------------//
void Pacman::setResultsFinishScreen(int finish) // sets a new value to 'resultsFinishScreen'
{
	this->resultsFinishScreen = finish;
}
//--------------------------------------------------------------------------------------//
void Pacman::pacmanMove(const Board& board) // handles the pacman's movement
{
	switch (this->direction) // handle pacman's movement
	{
	case (char)Keys::UP:
		if (board.getCell(this->curPos.getY() - 1, this->curPos.getX()) == (char)Board_Symbols::WALL)
		{
			this->direction = (char)Keys::STAY;
		}
		else
		{
			this->curPos.setY(this->curPos.getY() - 1);
		}

		break;
	case (char)Keys::RIGHT:
		if (board.getCell(this->curPos.getY(), this->curPos.getX() + 1) == (char)Board_Symbols::WALL)
		{
			this->direction = (char)Keys::STAY;
		}
		else
		{
			this->curPos.setX(this->curPos.getX() + 1);
		}
		break;
	case (char)Keys::DOWN:
		if (board.getCell(this->curPos.getY() + 1, this->curPos.getX()) == (char)Board_Symbols::WALL)
		{
			this->direction = (char)Keys::STAY;
		}
		else
		{
			this->curPos.setY(this->curPos.getY() + 1);
		}
		break;
	case (char)Keys::LEFT:
		if (board.getCell(this->curPos.getY(), this->curPos.getX() - 1) == (char)Board_Symbols::WALL)
		{
			this->direction = (char)Keys::STAY;
		}
		else
		{
			this->curPos.setX(this->curPos.getX() - 1);
		}
		break;
	case (char)Keys::STAY:
		break;
	}
}
//--------------------------------------------------------------------------------------//
void Pacman::addResLoseLife(int lose) // add a new lose life value to the vector
{
	this->resultsLoseLife.push_back(lose);
}
//--------------------------------------------------------------------------------------//
int Pacman::nextValLoseLife() // pop the next value from 'loseLife' vector
{
	int val = resultsLoseLife.back();
	this->resultsLoseLife.pop_back();
	return val;
}
//--------------------------------------------------------------------------------------//