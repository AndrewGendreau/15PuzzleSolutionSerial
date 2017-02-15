/*NAME: arrayReader.c
DATE: Oct 14th 2016
VERSION: 2.0
PURPOSE: This module will contain functions to read an instance of the 15 puzzle problem
	from a given input file. It will also contain a function to open a file, as well as one
	to map a 2D version of the problem instance to a 1D array.
AUTHORS:  Andrew Gendreau, Thana Alamro, Mohammed Asiri
*/

#include <stdio.h>
#include <stdlib.h>

#ifndef HASH_H_
#define HASH_H_
#include "hashList.h"
#endif

#include "arrayReader.h"

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
**/
void mapTwoDArrayToOneD(int array[BOARDWIDTH][BOARDHEIGHT], int oneDForm[BOARDSIZE])
{
	int currentX;					//current row of the 2D form
	int currentY;					//current column of the 2D form
	int currentInt;					//current integer value we are looking at
	int currentOneDIndex;				//current 1D index we are filling
	
	currentX = 0;
	currentY = 0;

	currentOneDIndex = 0;

	for(currentX = 0; currentX < 4; currentX++)
	{
		for(currentY = 0; currentY < 4; currentY++)
		{
			currentInt = array[currentX][currentY];
			oneDForm[currentOneDIndex] = currentInt;
			currentOneDIndex++;
		}
	}
}

/**
*Name: openFile
*Purpose: This function will simply open the file corresponding to the
	given filename and return the file pointer.
*Parameters: 
	fileName --name of the file to open
*Returns:
	pointer to the desired file or NULL
**/
FILE *openFile(char *fileName)
{
	FILE *filePointer;					//file pointer

	filePointer = fopen(fileName, "r");

	return filePointer;
}

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
**/
void fillTwoDArray(int array[BOARDWIDTH][BOARDHEIGHT], FILE *filePointer)
{
	int i;							//current row
	int j;							//current column/place in currentLineInts
	char *currentLine = NULL;				//current line read from the file
	int currentLineInts[BOARDWIDTH];			//temporary array to hold the integers read
	size_t len = 0;						//size_t variable for getLine
	
	i = 0;

	while(getline(&currentLine, &len, filePointer) != -1)
	{
		sscanf(currentLine, "%d %d %d %d", &currentLineInts[0], &currentLineInts[1], &currentLineInts[2], &currentLineInts[3]);
		
		for(j = 0; j < BOARDWIDTH; j++)
		{
			array[i][j] = currentLineInts[j];
		}
		i++;
	}	
}
