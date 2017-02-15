/*NAME: functions-2.h
DATE: Oct 14th 2016
VERSION: 2.0
FILENAME: functions-2.h
PURPOSE: This module is a header file for functions-2.c
AUTHOR: Andrew Gendreau
*/

/**
*Name: solvable
*Purpose: This function will check a given 2D instance of the 15
	puzzle problem for solvability. It will use the above methods to do so.
	As mentioned above, we know it is solvable in the following two cases:
		-The blank is on an even row counting from the bottom and the number of inversions is odd
		-The blank is on an odd row counting from the bottom and the number of inversions is even.
	Otherwise, we know it is not solvable.
*Parameters: 
	puzzle[BOARDHEIGHT][BOARDWIDTH] --15 puzzle
*Returns:
	1 if the puzzle is solvable
	0 if it is not
*Author: Andrew Gendreau
**/
int solvable(int blankSpot, int puzzle1D[BOARDSIZE]);

int rowPosition(int puzzle[BOARDHEIGHT][BOARDWIDTH]);
