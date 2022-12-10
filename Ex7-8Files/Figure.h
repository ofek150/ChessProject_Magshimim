#pragma once
#include <string>
#include "Point.h"
#include "GameManager.h"
#include <cmath>

class Figure
{
public:
	Figure(const std::string type, const std::string color);
	~Figure();
	virtual bool canMove(const Board& board, const Move move) const = 0;
	virtual std::string getType(); // Returns the type of the figure
	virtual std::string getColor(); // Returns the color of the figure
	virtual int getStepsTaken(); // Returns the amount of steps taken
protected:
	const std::string type;//the type of the figure, changes only in the c'tor
	const std::string color;
	int stepsTaken;//true if figure did not move, else false
};

