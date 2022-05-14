#include "Ghost.h"
Ghost::Ghost() :GameObject(), startPos()
{
}
//--------------------------------------------------------------------------------------//
Ghost::Ghost(int startPosX, char direction) : GameObject(startPosX, 0, direction), startPos()
{
}
//--------------------------------------------------------------------------------------//
Color Ghost::getColor() const // returns 'color'
{
	return this->color;
}
//--------------------------------------------------------------------------------------//
Coord& Ghost::getStartPos() // returns 'startPos'
{
	return this->startPos;
}
//--------------------------------------------------------------------------------------//
void Ghost::setColor(Color color) // sets a new value to 'color'
{
	this->color = color;
}
//--------------------------------------------------------------------------------------//
void Ghost::setStartPos(const Coord& startPos) // sets a new value to 'startPos'
{
	this->startPos = startPos;
}
//--------------------------------------------------------------------------------------//
void Ghost::ghostMove(const Board& board, Coord& pacmanPos, const int level, Ghost** ghosts, const int numOfGhosts, bool& isInFiveSteps, int gameMode) // handles the ghost movement
{
	switch (level)
	{
	case (int)level::BEST:
		smartGhost(board, pacmanPos, ghosts, numOfGhosts, gameMode);
		this->countSteps = 0;
		break;
	case (int)level::GOOD:
		goodGhost(board, pacmanPos, ghosts, numOfGhosts, isInFiveSteps, gameMode);
		break;
	case (int)level::NOVICE:
		noviceGhost(board, ghosts, numOfGhosts, gameMode);
		break;
	}

}
//--------------------------------------------------------------------------------------//
void Ghost::smartGhost(const Board& board, const Coord& pacmanPos, Ghost** ghosts, const int numOfGhosts, int gameMode) // handles the BEST case of ghosts mode
{

	string path;

	if (gameMode != (int)Game_Mode::LOAD)
	{
		BFS(board, pacmanPos, path);

		direction = path[0];
	}

	noviceGhost(board, ghosts, numOfGhosts, gameMode);
}
//--------------------------------------------------------------------------------------//
void Ghost::goodGhost(const Board& board, const Coord& pacmanPos, Ghost** ghosts, const int numOfGhosts, bool& isInFiveMode, int gameMode) // handles the GOOD case of ghosts mode
{
	if (isInFiveMode && (this->countSteps == 5))
	{
		this->countSteps = 0;
		isInFiveMode = false;
		smartGhost(board, pacmanPos, ghosts, numOfGhosts, gameMode);
	}
	else if (!isInFiveMode && ((this->countSteps) < (this->changeDirInStep)))
	{
		smartGhost(board, pacmanPos, ghosts, numOfGhosts, gameMode);
	}
	else if (!isInFiveMode)
	{
		noviceGhost(board, ghosts, numOfGhosts, gameMode);
		this->changeDirInStep = randomBetween(DEFAULT_CHANGE_TIME - 2, DEFAULT_CHANGE_TIME);
		isInFiveMode = true;
	}
	else
	{
		noviceGhost(board, ghosts, numOfGhosts, gameMode);
	}

}
//--------------------------------------------------------------------------------------//
void Ghost::noviceGhost(const Board& board, Ghost** ghosts, const int numOfGhosts, int gameMode) // handles the NOVICE case of ghosts mode
{
	int x = this->curPos.getX();
	int y = this->curPos.getY();
	int k;
	bool toAdd = true;

	switch (this->direction)// handle ghost 1 movement
	{
	case (char)Keys::UP:
		if (board.getCell(this->curPos.getY() - 1, this->curPos.getX()) == (char)Board_Symbols::WALL || this->curPos.getY() - 1 == 0)
		{
			this->direction = randomDirection();
			toAdd = false;
		}
		else if (this->countSteps >= this->changeDirInStep)
		{
			this->direction = randomDirection();
			this->countSteps = 0;
			toAdd = false;
		}
		else
		{
			this->curPos.setY(this->curPos.getY() - 1);
			increaseStep();
		}
		break;
	case (char)Keys::RIGHT:
		if (board.getCell(this->curPos.getY(), this->curPos.getX() + 1) == (char)Board_Symbols::WALL || this->curPos.getX() + 1 == board.getWidth() - 1)
		{
			this->direction = randomDirection();
			toAdd = false;
		}
		else if (this->countSteps >= this->changeDirInStep)
		{
			this->direction = randomDirection();
			this->countSteps = 0;
			toAdd = false;
		}
		else
		{
			this->curPos.setX(this->curPos.getX() + 1);
			increaseStep();
		}
		break;
	case (char)Keys::DOWN:
		if (board.getCell(this->curPos.getY() + 1, this->curPos.getX()) == (char)Board_Symbols::WALL || this->curPos.getY() + 1 == board.getHight() - 1)
		{
			this->direction = randomDirection();
			toAdd = false;
		}
		else if (this->countSteps >= this->changeDirInStep)
		{
			this->direction = randomDirection();
			this->countSteps = 0;
			toAdd = false;
		}
		else
		{
			this->curPos.setY(this->curPos.getY() + 1);
			increaseStep();
		}
		break;
	case (char)Keys::LEFT:
		if (board.getCell(this->curPos.getY(), this->curPos.getX() - 1) == (char)Board_Symbols::WALL || this->curPos.getX() - 1 == 0)
		{
			this->direction = randomDirection();
			toAdd = false;
		}
		else if (this->countSteps >= this->changeDirInStep)
		{
			this->direction = randomDirection();
			this->countSteps = 0;
			toAdd = false;
		}
		else
		{
			this->curPos.setX(this->curPos.getX() - 1);
			increaseStep();
		}

		break;
	case (char)Keys::STAY:
		if (gameMode == (int)Game_Mode::LOAD)
		{
			break;
		}
		else
		{
			this->direction = randomDirection();
		}
		increaseStep();
		break;
	}

	for (k = 0; k < numOfGhosts; k++)
	{
		if (this != (ghosts[k]) && this->curPos.samePlace((*ghosts[k]).curPos))
		{
			this->curPos.setX(x);
			this->curPos.setY(y);
			toAdd = false;
		}

	}

	if (gameMode == (int)Game_Mode::SAVE)
	{
		if (toAdd)
		{
			this->addMove(this->getDirection());
		}
		else
		{
			this->addMove((char)Keys::STAY);
		}

	}
}
//--------------------------------------------------------------------------------------//
void Ghost::BFS(const Board& board, const Coord& pacmanPos, string& path) // uses the BFS algorithem to find a 'path' from the ghost to the pacman
{
	Coord copyP = this->curPos;
	Coord nodeP;

	vector <Coord> neighbours;

	Coord prev[MAX_ROWS][MAX_COLS];

	queue<Coord> QC;
	QC.push(copyP);

	bool visited[MAX_ROWS][MAX_COLS] = { false };

	visited[copyP.getY()][copyP.getX()] = true;

	while (!QC.empty())
	{
		nodeP = QC.front();
		QC.pop();

		if (nodeP == pacmanPos)
		{
			break;
		}

		exploreNeighbours(board, nodeP, neighbours);

		for (int i = 0; i < neighbours.size(); i++)
		{
			if (!visited[neighbours[i].getY()][neighbours[i].getX()])
			{
				QC.push(neighbours[i]);
				visited[neighbours[i].getY()][neighbours[i].getX()] = true;
				prev[neighbours[i].getY()][neighbours[i].getX()] = nodeP;
			}
		}
	}

	while (!(nodeP == this->curPos))
	{
		path.push_back(convertDirection(nodeP, prev[nodeP.getY()][nodeP.getX()]));

		nodeP = prev[nodeP.getY()][nodeP.getX()];
	}

	stringReverse(path);

}
//--------------------------------------------------------------------------------------//
char Ghost::convertDirection(const Coord& child, const Coord& parent) // compares two close coords and returns the direction between the child and the parent
{
	int dr[4] = { -1, 1, 0, 0 };
	int dc[4] = { 0, 0, 1 ,-1 };
	char res[4] = { (char)Keys::UP,(char)Keys::DOWN, (char)Keys::RIGHT, (char)Keys::LEFT };

	for (int t = 0; t < 4; t++)
	{
		if ((parent.getX() - child.getX() == dc[t]) && (parent.getY() - child.getY() == dr[t]))
		{
			return res[t];
		}
	}
	return (char)Keys::STAY;
}
//--------------------------------------------------------------------------------------//
void Ghost::exploreNeighbours(const Board& board, const Coord& nodeP, vector<Coord>& neighbours) // find all accessible neighbours of a coord on board
{
	int i, X, Y;
	int dr[4] = { -1, 1, 0, 0 };
	int dc[4] = { 0, 0, 1 ,-1 };

	neighbours.clear();

	X = nodeP.getX();
	Y = nodeP.getY();
	for (i = 0; i < 4; i++)
	{
		Y = Y + dr[i];
		X = X + dc[i];
		if ((Y > 0) && (Y < board.getHight()) && (X > 0) && (X < board.getWidth()) && board.getCell(Y, X) != (char)Board_Symbols::WALL)
		{
			neighbours.push_back(Coord(X, Y));
		}
		X = nodeP.getX();
		Y = nodeP.getY();
	}
}
//--------------------------------------------------------------------------------------//
void Ghost::stringReverse(string& path) // converts the directions to their opposites and reverses the string of them
{
	for (int i = 0; i < path.size(); i++)
	{
		switch (path[i])
		{
		case(char)Keys::UP:
			path[i] = (char)Keys::DOWN;
			break;
		case(char)Keys::RIGHT:
			path[i] = (char)Keys::LEFT;
			break;
		case(char)Keys::LEFT:
			path[i] = (char)Keys::RIGHT;
			break;
		case(char)Keys::DOWN:
			path[i] = (char)Keys::UP;
			break;
		default:
			break;
		}
	}
	reverse(path.begin(), path.end());
}
//--------------------------------------------------------------------------------------//