/*NAME: mys15p.c
DATE: Oct 14th 2016
VERSION: 2.0
FILENAME: mys15p.h
PURPOSE: This module is the main invoker module for mys15p.
	It will prompt the user for input, parse the input to 
	remove the newline character, then invoke the solvability
	tester upon the input. It will inform the user if he/she has
	entered an unsolvable case, if so it will tell them then die,
	if not, it will run the algorithm to locate the shortest path to 
	a solution.
AUTHOR: Andrew Gendreau
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef SOLVER_H_
#define SOLVER_H_
#include "puzzleSolver.h"
#endif

#ifndef ARRAYREADER_H_
#define ARRAYREADER_H_
#include "arrayReader.h"
#endif

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include "functions-2.h"
#endif

#define LINESIZE 200							//arbitary limit on user fileName

/**
*Name: main
*Purpose: This function will run the serial version. It will do this by first
	reading in the input from a given input file. It will then run the solvability
	test module on the given input after converting it to a 1D form. If the given 
	board is not solvable, it will tell the user and die. If it is solvable, it will
	run the algorithm on the given board.
	
*Parameters: 
	int argc --Main parameter, this is a count of command line arguements
	char *argv[] --Main parameter, this is a pointer to an array of pointers.
*Returns:
	0
*Author: Andrew Gendreau
**/
int main(int argc, char *argv[])
{
	int twoDInputBoard[BOARDWIDTH][BOARDHEIGHT];			//variable to hold the 2D form of the input board, given by the user
									//in the form of a file.
	FILE *filePointer;						//pointer to the input file
	int solvableTest;						//return from the solvability test
	int oneDInputBoard[BOARDWIDTH];					//variable to hold the 1D form of the given input board
	char userInput[LINESIZE];					//character array to hold the user's given fileNames
	char currentChar;						//current character in the user input
	int i;								//loop control variable
	clock_t start;							//clock_t variable to hold the starting time information
	clock_t end;							//clock_t variable to hold the ending time information
	clock_t timeDiff;						//clock_t variable to hold the difference between previous 2
	double millSeconds;						//time it took in milliseconds
	int blankPositionFromBottom;
	double seconds;

	printf("Welcome to this 15 puzzle solver\n");
	printf("Enter an input fileName\n");

	fgets(userInput, LINESIZE, stdin);				//read from stdin

	for(i = 0; i < LINESIZE; i++)					//clip off the newline character
	{
		currentChar = userInput[i];
		
		if(currentChar == '\n')
		{
			userInput[i] = '\0';
			break;
		}
	}

	filePointer = openFile(userInput);
	
	if(filePointer == NULL)
	{
		printf("That input file does not exist!\n");
		printf("Exiting now\n");
		return 1;
	}

	start = clock();

	fillTwoDArray(twoDInputBoard, filePointer);

	fclose(filePointer);

	blankPositionFromBottom = rowPosition(twoDInputBoard);
	
	mapTwoDArrayToOneD(twoDInputBoard, oneDInputBoard);

	solvableTest = solvable(blankPositionFromBottom, oneDInputBoard);

	if(solvableTest == 0)
	{
		printf("You're unsolvable!\n");
		printf("Exiting now.......\n");
		return 0;
	}
	else
	{
		printf("It is solvable!\n");
		printf("Running test\n");
		runTest(oneDInputBoard);
		end = clock();
		timeDiff = end - start;
		millSeconds = timeDiff * 1000 / CLOCKS_PER_SEC;
		seconds = millSeconds / 1000.0;

		printf("Solving this puzzle took %f seconds\n", seconds);
	}
	return 0;
}
