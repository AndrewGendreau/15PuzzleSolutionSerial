/*NAME: pathfinder.h
DATE: Oct 14th 2016
VERSION: 2.0
FILENAME: hashList.h
PURPOSE: This is the header file for pathfinder.c
AUTHOR: Andrew Gendreau
*/

#define BOARDWIDTH 4
#define BOARDHEIGHT 4
#define BOARDSIZE 16							//the size of the board, 4x4, so 16 elements total.

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
*Author: Andrew Gendreau
**/
int findBlankIndex(int board[BOARDSIZE]);

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
*Author: Andrew Gendreau
**/
int findX(int OneDblankIndex);

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
*Author: Andrew Gendreau
**/
int findY(int OneDblankIndex);

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
*Author: Andrew Gendreau
**/
int numberofMovesPossible(int blankX, int blankY);

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
*Author: Andrew Gendreau
**/
char *findMoveDirections(int movesPossible, int blankX, int blankY);

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
*Author: Andrew Gendreau
**/
int *tilesToMoveIn(int movesPossible, int blankX, int blankY, char *moveDirections);

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
*Author: Andrew Gendreau
**/
void swap(int blankIndex, int tileToSwapIndex, int board[BOARDSIZE]);
