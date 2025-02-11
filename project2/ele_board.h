#ifndef ELE_BOARD_H
#define ELE_BOARD_H 1

#include "life_cell.h"
#include "life_board.h"
#include <string>
#include <iostream>
#include <stdlib.h>

/**
	One will be represented by alive cells and Zero will be represented by dead cells
**/
class ele_board : public life_board
{
	int rule;
public:
	ele_board(std::string name, int xmax, int xmin, int ymax, int ymin, char one, char zero, int rule);

	virtual ~ele_board();
	/**
	* Updates the cells one generation
	**/
	virtual void updateOne();
	/**
	*	Returns a string containing the board converted to a life file
	**/
	virtual std::string toFile();

	virtual board* Clone();
};


#endif