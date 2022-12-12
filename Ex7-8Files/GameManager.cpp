#include "GameManager.h"

GameManager::GameManager(std::string startingBoard) : turn(whitePlayer), gameEnded(false), board(startingBoard) {} // CTOR

GameManager::~GameManager() {} // DTOR

void GameManager::changeTurn() // Changes turn to the other player
{
	//checkWin();
	if (this->gameEnded) return;
	if (this->turn == whitePlayer) this->turn = blackPlayer;
	else this->turn = whitePlayer;
}

std::string GameManager::updateBoard(const std::string& msg)
{
	Move move = parseMsg(msg);
	int srcX = move.getSrc().getX(), srcY = move.getSrc().getY(), destX = move.getDest().getX(), destY = move.getDest().getY();
	std::string returnMsg;
	if (srcX > MAX_X || srcY > MAX_Y || destX > MAX_X || destY > MAX_Y)
	{
		returnMsg = composeMsg(INVALID_POINT_OUTSIDE_BOARD);
		return returnMsg;
	}
	else if(srcX == destX && srcY == destY)
	{
		returnMsg = composeMsg(INVALID_SRC_EQU_DST);
		return returnMsg;
	}
	Figure* tempSrc = this->board.getFigure(srcX, srcY);
	Figure* tempDest = this->board.getFigure(destX, destY);
	if (tempSrc->getColor() != "White" && this->turn == whitePlayer || tempSrc->getColor() != "Black" && this->turn == blackPlayer)
	{
		returnMsg = composeMsg(INVALID_SRC_DOESNT_HAVE_PLAYER_FIGURE);
	}
	else if (tempSrc->getType() == "EmptySlot")
	{
		returnMsg = composeMsg(INVALID_SRC_DOESNT_HAVE_PLAYER_FIGURE);
	}
	else if (tempSrc->getColor() == tempDest->getColor())
	{
		returnMsg = composeMsg(INVALID_DST_TAKEN);
	}
	else if(tempSrc->canMove(this->board.getBoard(), move))
	{
		this->board.setFigure(tempSrc, destX, destY);
		Figure* emptySlot = new EmptySlot();
		this->board.setFigure(emptySlot, srcX, srcY);
		int moveMsgId = checkWin(tempSrc->getColor());
		returnMsg = composeMsg(moveMsgId);
		switch (moveMsgId)
		{
		case VALID_MOVE:
			tempSrc->setStepsTaken(tempSrc->getStepsTaken() + 1);
			changeTurn();
			delete tempDest;
			break;
		case INVALID_CAUSES_CHESS:
			this->board.setFigure(tempSrc, srcX, srcY);
			this->board.setFigure(tempDest, destX, destY);
			break;
		case VALID_MOVE_CHESS:
			changeTurn();
			delete tempDest;
			break;
		case VALID_CHECKMATE:
			gameEnded = true;
			delete tempDest;
			break;
		}
	}
	else
	{
		returnMsg = composeMsg(INVALID_MOVEMENT);
	}

	//ADD CHECK FOR CHECKMATE
	return returnMsg;
}

int GameManager::checkWin(const std::string& color) // Returns INVALID_CAUSES_CHESS, VALID_MOVE_CHESS or VALID_MOVE
{
	if (checkCheck(color)) return INVALID_CAUSES_CHESS;
	if (color == "White") 
	{
		if (checkCheckmate("Black")) return VALID_CHECKMATE;
		else
		{
			if (checkCheck("Black")) return VALID_MOVE_CHESS;
		}
	}
	else
	{
		if (checkCheckmate("White")) return VALID_CHECKMATE;
		else
		{
			if (checkCheck("White")) return VALID_MOVE_CHESS;
		}
	}
	return VALID_MOVE;
}

bool GameManager::checkCheck(const std::string& color) // Checks if there's check on	the player with the color 'color' and return true / false
{
	Figure* king = nullptr;
	Figure* temp = nullptr;

	int kingX = 0;
	int kingY = 0;

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int x = 0; x < BOARD_SIZE; x++)
		{
			temp = this->board.getFigure(x, i);
			if (temp->getType() == "King")
			{
				if (temp->getColor() == color)
				{
					king = temp;
					kingX = x;
					kingY = i;
					break;
				}
			}
		}
	}

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int x = 0; x < BOARD_SIZE; x++)
		{
			temp = this->board.getFigure(x, i);
			if (temp->getColor() != color)
			{
				Move toKing(Point(x, i), Point(kingX, kingY));
				if (temp->canMove(this->board.getBoard(), toKing))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool GameManager::checkCheckmate(const std::string& color)
{
	return false;
}



Move GameManager::parseMsg(const std::string& msg) // Parses the pipe's message and returns a move
{
	char str[2];
	int srcX = msg[0] - 'a' + 1;
	int srcY = msg[1] - '0';
	int destX = msg[2] - 'a' + 1;
	int destY = msg[3] - '0';
	Move move(Point(srcX - 1, srcY - 1), Point(destX - 1, destY - 1));
	std::cout << move.returnMoveStr() << std::endl;
	return move;
}

std::string GameManager::composeMsg(int moveMsgId) // composes the board string for the pip
{
	std::string msg = std::to_string(moveMsgId);
	std::cout << "Msg: " << msg << std::endl;
	return msg;
}