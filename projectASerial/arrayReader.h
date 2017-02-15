/*NAME: arrayReader.h
DATE: Oct 14th 2016
VERSION: 2.0
PURPOSE: This is a header file for arrayReader.c
AUTHOR: Andrew Gendreau
*/

#define BOARDWIDTH 4
#define BOARDHEIGHT 4

/**
*Name: mapTwoDArrayToOneD
*Purpose: This function will map the 2D form of the user given 15 puzzle problem into 
	a 1D array. It will do this simply by running through the 2D array and keeping a counter
	for the 1D version. This counter will be incremented for every value in the 2D form and 
	the current value in the 1D array will be the current 2D one we are looking at.
*Parameters: 
	array[BOARDWIDTH][BOARDHEIGHT] --2D form of the user input
	oneDForm[BOARDSIZE] --1D form of the user input
*Returns:
	void
*Author: Andrew Gendreau
**/
void mapTwoDArrayToOneD(int array[BOARDHEIGHT][BOARDWIDTH], int oneDForm[BOARDSIZE]);

/**
*Name: openFile
*Purpose: This function will simply open the file corresponding to the
	given filename and return the file pointer.
*Parameters: 
	fileName --name of the file to open
*Returns:
	pointer to the desired file or NULL
*Author: Andrew Gendreau
**/
FILE *openFile(char *fileName);

/**
*Name: fillTwoDArray
*Purpose: This function will read the user input from the given file and place
	it in a 2D array. It will do this by using i to track the current row we're filling,
	then using a length 4 array to hold the 4 values we need. It will read a line from the file,
	grab 4 integers using sscanf, then place those integers into the corresponding values 
	in the tempory array. It will then run through this array and place them into the 2D array.
*Parameters: 
	array[BOARDWIDTH][BOARDHEIGHT] --2D form of the user input
	filePointer --pointer to the file to read from
*Returns:
	void
*Author: Andrew Gendreau
**/
void fillTwoDArray(int array[BOARDHEIGHT][BOARDWIDTH], FILE *filePointer);
