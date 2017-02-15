/*NAME: pathfinder.h
DATE: Oct 14th 2016
VERSION: 2.0
FILENAME: hashList.h
PURPOSE: This is a module to find the possible moves and the indexes of the tiles
	we can swap. It will assume the input is a 1d array representing the game board.
	It will have a function to compute the x and y coordinate of the index of the blank,
	find the directions that tiles around the blank can move in from, then compute the
	1D array index of those tiles that can move into the blank space.
AUTHORS:  Andrew Gendreau
*/

#include <stdlib.h>
#include <stdio.h>


#include "pathfinder.h"

/**
*Name: findBlankIndex
*Purpose: This function will, given a 1d array representing the board,
	find the index of the blank. It will do this by assuming the blank 
	is 0 and running through the array until it finds 0. At which point,
	it will return the index that contains 0.
*Parameters: 
	board --1d array containing the game board
*Returns:
	index of the blank
**/
int findBlankIndex(int board[BOARDSIZE])
{
	int i;						//loop control variable
	
	i = 0;

	if(board[i] == 0)
	{
		return i;
	}
	else
	{
		while(board[i] != 0)
		{
			i++;
		}
	return i;
	}
}

/**
*Name: findX
*Purpose: This function will, given the 1d index of the game board, find the 
	x coordinate or the column of the blank in a 2d plane. It will do 
	this by taking the modulus of the index by 4, the number of columns
	in the board.
*Parameters: 
	OneDblankIndex --1d array index of the blank
*Returns:
	x coordinate of the blank
**/
int findX(int OneDblankIndex)
{
	int blankX;						//blank's x coordinate

	blankX = OneDblankIndex % 4;

	return blankX;
}

/**
*Name: findY
*Purpose: This function will, given the 1d index of the game board, find the 
	y coordinate or the row of the blank in a 2d plane. It will do 
	this by dividing the blank index by 4, the number of rows
	in the board.
*Parameters: 
	OneDblankIndex --1d array index of the blank
*Returns:
	y coordinate of the blank
**/
int findY(int OneDblankIndex)
{
	int blankY;						//blank's y coordinate

	blankY = OneDblankIndex / 4;
	
	return blankY;
}

/**
*Name: numberofMovesPossible
*Purpose: This function will, given the x and y coordinate of the blank,
	compute the number of moves possible. It will do this by using a switch,
	using the following heuristic:
		We will start assuming 4 moves are possible, if either the x or y
		coordinate is 0 or 3, we cannot make at least one move in that dimension.
		This assumes that we can make at least 2 moves, which we always should be
		able to, the worst case is when the blank is in a corner, in which case we have
		2 moves.
*Parameters: 
	blankX --x coordinate of the blank
	blankY --y coordinate of the blank
*Returns:
	the number of moves possible
**/
int numberofMovesPossible(int blankX, int blankY)
{
	int movesPossible;					//moves possible
	
	movesPossible = 4;

	switch(blankX)
	{
		case 0:
			movesPossible--;
			break;
		case 3:
			movesPossible--;
			break;
	}
	switch(blankY)
	{
		case 0:
			movesPossible--;
			break;
		case 3:
			movesPossible--;
			break;
	}

	return movesPossible;
}

/**
*Name: findMoveDirections
*Purpose: This function will, given the number of moves possible,
	the x and y coordinates of the blank, find the 
	directions tiles can move to trade positions with the blank.
	It will represent the directions as a char array, using the following
	representation:
		U -UP -tile can move up
		R -Right -tile can move right
		L -Left	-tile can move left
		D -Down	-tile can move down
*Parameters: 
	movesPossible --number of moves possible
	blankX --x coordinate of the blank
	blankY --y coordinate of the blank
*Returns:
	char array representing the possible directions
**/
char *findMoveDirections(int movesPossible, int blankX, int blankY)
{
	char *moveDirections;					//character string to represent the move directions
	int i;							//loop control variable

	moveDirections = malloc((movesPossible) * sizeof(char) + 1);
	i = 0;

	if((blankY - 1) >= 0)					//check the Ys
	{
		moveDirections[i] = 'D';
		i++;
	}
	if((blankY + 1) <= 3)
	{
		moveDirections[i] = 'U';
		i++;
	}

	if((blankX - 1) >= 0)					//check the Xs
	{
		moveDirections[i] = 'R';
		i++;
	}
	if((blankX + 1) <= 3)
	{
		moveDirections[i] = 'L';
		i++;
	}
	moveDirections[i] = '\0';
	return 	moveDirections;
}

/**
*Name: tilesToMoveIn
*Purpose: This function will, given the number of moves possible,
	the x and y coordinates of the blank, and the move directions,
	find the 1D array indexes of the tiles that can move in.
	it will compute the 1d array index as follows:
		1d array index: Y * 4 + X
		This gives us the 1d mapping of a given 2d coordinate.
*Parameters: 
	moveDirections --directions tiles can move in from
	movesPossible --number of moves possible
	blankX --x coordinate of the blank
	blankY --y coordinate of the blank
*Returns:
	int array representing the indexes of the tiles that can be moved in
**/
int *tilesToMoveIn(int movesPossible, int blankX, int blankY, char *moveDirections)
{
	int *tile1Dindexes;						//int array representing the 1d indexes
	int i;								//loop control variable
	int currentValue;						//current index

	tile1Dindexes = malloc(movesPossible * sizeof(int));
	
	for(i = 0; i < movesPossible; i++)
	{
		switch(moveDirections[i])
		{
			case 'U':					//case when a tile can move up into the blank space
				currentValue = ((blankY+1) * 4) + blankX;
				tile1Dindexes[i] = currentValue;
				break;
			case 'D':					//case when a tile can move down into the blank space
				currentValue = ((blankY-1) * 4) + blankX;
				tile1Dindexes[i] = currentValue;
				break;		
			case 'R':					//case when a tile can move right into the blank space
				currentValue = (blankY * 4) + (blankX-1);
				tile1Dindexes[i] = currentValue;
				break;
			case 'L':					//case when a tile can move left into the blank space
				currentValue = (blankY * 4) + (blankX+1);
				tile1Dindexes[i] = currentValue;
				break;
		}
	}	
	return tile1Dindexes;
}

/**
*Name: swap
*Purpose: This function will, given the 1d index of the blank,
	the 1d index of a tile to swap with, and the board
	to swap on, will swap the blank space with the given tile
*Parameters: 
	blankIndex --1d index of the blank
	tileToSwapIndex --1d index of a tile to swap with
	board --board to swap on
*Returns:
	void
**/
void swap(int blankIndex, int tileToSwapIndex, int board[BOARDSIZE])
{
	int temp;							//temporary variable to hold the blank value
	
	temp = board[blankIndex];

	board[blankIndex] = board[tileToSwapIndex];

	board[tileToSwapIndex] = temp;
}
