/*NAME: puzzleSolver.c
DATE: Oct 14th 2016
VERSION: 2.0
FILENAME: puzzleSolver.c
PURPOSE: This module will be the algorithm implementer module.
	It will use hashList, Yggdrasil and pathfinder to solve the puzzle.
	It will use the following algorithm:
		1. It will initalize Yggdrasil and the hashList.
		2. It will hash the inital board state and enter it in the hashList.
		3. Then, beginning at the root, for every node i at level l:
			4. Set l to be the depth we are working with, (the number of moves from the root)
			5. calculate the possible moves m from that board position
			6. Check each of the moves calculated for board positions we have seen before to find m' (# of new moves)
			6. spawn m' new nodes
			7. Insert them into Yggdrasil as node i's children
			8. Do this for every node on level l.
			8a: If the hash for child c is the winning one, stop, we've found our winning state.
			8b: else, we have exhausted this level, increment l and move down.
	The breadth first traversal needed for this algorithm to work will be implemented using a queue as follows:
		Keep two counters, one will hold the number of nodes at the current level
		de queue nodes one by one until we've done all for that level, when we insert new nodes into Yggdrasil,
		we will increment the second counter meant to hold the nodes in the next level.
		Once we have dealt with all the nodes at a given level, we will put the second counter's value into the first
		one and set the second counter to 0.
AUTHOR: Andrew Gendreau
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "puzzleSolver.h"

int currentDepth = 0;							//depth of the nodes we're working with right now
extern int currentLevelNodeCount;					//node count at the current level
extern int nextLevelNodeCount;						//node count at the next level
extern struct tree *treeRoot;						//root of the tree of board positions and moves

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
int createRoot(int board[BOARDSIZE])
{
	long int rootHash;						//hash of the root of Yggdrasil
	struct treeNode *node;						//root node

	createEmptyHashList();						//create a new hashList

	rootHash = computeHashValue(board);				//compute the root's hash

	if(rootHash == WINNINGHASH)
	{
		printf("You gave me a winning state!\n");
		return 1;
	}

	node = createNode(currentDepth, rootHash, board, -1, 0);

	insertIntoYggdrasil(node, NULL);

	enQueue(node);

	currentDepth++;

	return 0;
}

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
int processNode(struct treeNode *currentNode)
{
	int blankIndex;								//index of the current blank position
	int blankX;								//x coordinate of the blank
	int blankY;								//y coordinate of the blank
	int movesPossible;							//the number of moves possible from this position
	char *moveDirections;							//character string of the move directions
	int *indexesToSwap;							//1d indexes of the tiles we can move into the blank spot
	int i;									//loop control variable
	int currentIndex;							//current index we are swapping in
	long int hashVal;							//hash value of the board after the swap
	int tileWeAreMoving;							//The actual value of the tile we are moving
	struct treeNode *newNode;						//the next node we are going to insert
	int lastNodeInsertedPosition;						//the index in the children array of the last node we
										//inserted
	int seenBeforeSignal;

	lastNodeInsertedPosition = -1;

	blankIndex = findBlankIndex(currentNode->board);

	blankX = findX(blankIndex);
	blankY = findY(blankIndex);

	movesPossible = numberofMovesPossible(blankX, blankY);

	moveDirections = findMoveDirections(movesPossible, blankX, blankY);

	indexesToSwap = tilesToMoveIn(movesPossible, blankX, blankY, moveDirections);

	for(i = 0; i < 	movesPossible; i++)
	{
		int nodeBoardCopy[BOARDSIZE];					//variable to hold the board post switch for this node

		currentIndex = indexesToSwap[i];

		memcpy(nodeBoardCopy, currentNode->board, sizeof(nodeBoardCopy));	

		tileWeAreMoving = nodeBoardCopy[currentIndex];

		swap(blankIndex, currentIndex, nodeBoardCopy);

		hashVal = computeHashValue(nodeBoardCopy);

		seenBeforeSignal = haveISeenThisPosition(hashVal);

		if(seenBeforeSignal < 0)
		{	
			newNode = createNode(currentDepth, hashVal, nodeBoardCopy, tileWeAreMoving, moveDirections[i]);

			lastNodeInsertedPosition = insertIntoYggdrasil(newNode, currentNode);
			
			if(newNode->hashVal == WINNINGHASH)
			{
				return lastNodeInsertedPosition;
			}	
			else
			{
				enQueue(newNode);
				nextLevelNodeCount++;
			}
		}
		else
		{
			//do nothing
		}
	}
	return -1;
}

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
int processLevel(int nodesAtThisLevel)
{
	int nodesToProcess;					//nodes we need to process at this level
	struct treeNode *currentNode;				//node we're processing right now
	int i;							//loop control variable
	//char *winningPath;					//string that represents the winning path
	int haveIWon;						//integer to keep track of if we've won

	haveIWon = -1;

	nodesToProcess = nodesAtThisLevel;

	for(i = 0; i < nodesToProcess; i++)
	{
		currentNode = deQueue();

		if(currentNode == NULL)
		{
			printf("Null dequque again?\n");
			return -100;
		}

		haveIWon = processNode(currentNode);
		
		if(haveIWon >= 0)
		{
			//winningPath = traverseWinningPath(currentNode->children[haveIWon]);

			//printf("The winning path is: %s\n", winningPath);
			printf("It took %d moves\n", currentNode->children[haveIWon]->depth);
			return 1;
		}
	}
	currentLevelNodeCount = nextLevelNodeCount;
	nextLevelNodeCount = 0;
	currentDepth++;
	return -1;
}

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
void runTest(int board[BOARDSIZE])
{
	int haveWeFoundAWinner;					//integer to signal us if we've won
	int wereWeGivenAWinner;					//integer to signal if we were given a winning board
	
	wereWeGivenAWinner = createRoot(board);

	if(wereWeGivenAWinner == 1)
	{
		return;
	}
	else
	{
		haveWeFoundAWinner = processLevel(1);
		
		while(haveWeFoundAWinner != 1)
		{
			haveWeFoundAWinner = processLevel(currentLevelNodeCount);
		}
	}
}
