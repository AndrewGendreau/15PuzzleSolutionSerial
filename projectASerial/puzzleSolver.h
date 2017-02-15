/*NAME: puzzleSolver.h
DATE: Oct 14th 2016
VERSION: 2.0
FILENAME: puzzleSolver.h
PURPOSE: This is a module for puzzleSolver.c
AUTHOR: Andrew Gendreau
*/

#ifndef YGGDRASIL_H_
#define YGGDRASIL_H_
#include "yggdrasil.h"
#endif

#ifndef HASH_H_
#define HASH_H_
#include "hashList.h"
#endif

#ifndef QUEUE_H_
#define QUEUE_H_
#include "queue.h"
#endif

#ifndef PATH_H_
#define PATH_H_
#include "pathfinder.h"
#endif

#define BOARDWIDTH 4							//how many columns it has
#define BOARDHEIGHT 4							//how many rows the board has
#define BOARDSIZE 16							//the size of the board, 4x4, so 16 elements total.

/**
*Name: createRoot
*Purpose: This function will, given an array of ints to
	represent the board, create the node for the root of 
	Yggdrasil and insert it. It will also run a check for if the
	given state is a winning one, if so it will return 1 to signal that.
*Parameters: 
*	board --inital board
*Returns:
	int to signify if the given board is a winner or not
	1 if it is
	0 if it is not	
*Author: Andrew Gendreau
**/
int createRoot(int board[BOARDSIZE]);

/**
*Name: processNode
*Purpose: This function will, given a node to process, run steps 5-8
	of the above algorithm. It will find the x and y of the blank,
	find how many moves are possible, the 1d indexes of the tiles we can
	move in. Then, for every move we can make, it will copy the original board
	into another variable to hold that node's board. Then, for each move we can,
	it will swap the current index we can with the blank, then compute the hash value 
	of that	new board. If that board is new, we will insert it as a child of the given
	input node, if not, we won't bother making another node. We'll then check the hash,
	for a winning position, if it is a winning position, we'll return an int to signal that.
	otherwise, we'll enqueue the newly made node and increment the counter for the number of
	nodes on the next level.	
*Parameters: 
	currentNode --current node to process
*Returns:
	Index in current node's children array representing the winnning node
	-1 if we haven't found a winner yet.
*Author: Andrew Gendreau
**/
int processNode(struct treeNode *currentNode);

/**
*Name: processLevel
*Purpose: This function will, given the number of nodes at this level,
	will deQueue that many nodes from the queue and call processNode on them 
	one at a time. It will wait for a return from processNode, if that return
	is >= 0 that means we have found our winning hash and can stop. If it gets
	all the way through that level and has not found a winner, it will move the second
	node counter to equal the first and set the second to 0. Then increment currentDepth.
*Parameters: 
	nodesAtThisLevel --nodes at the this level, needed so we only deQueue that many nodes 
	from the queue
*Returns:
	1 if we've won
	-1 if we have not
*Author: Andrew Gendreau
**/
int processLevel(int nodesAtThisLevel);

/**
*Name: runTest
*Purpose: This function will, given a 1d array that represents the inital board state,
	first use that board to create the root node, it will check the return from createRoot
	to check for the case if we're given a solved puzzle already. It will call processLevel
	with 1 to signify calling it with only the root node. It will keep a variable to keep track of
	if we have found a winner or not, if we get to the end of a call of processLevel and have not
	gotten the winner signal, we'll call it again for the next level.
*Parameters: 
	board --1d Int array to hold the root board
*Returns:
	void
*Author: Andrew Gendreau
**/
void runTest(int board[BOARDSIZE]);
