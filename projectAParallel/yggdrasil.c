/*NAME: yggdrasil.c
DATE: Oct 14th 2016
VERSION: 2.0
FILENAME: yggdrasil.c
PURPOSE: This module will be a tree to use to find the shortest path.
	In each node of the tree, what will be stored will be:
		-node depth (int)
		-current board in 1d array form (array of ints)
		-move made from the parent (character string of such form as: 4U (4 moved up)
		-hash value of the board (long int)
		-Node ID: (hash value + node depth)
		-Pointer to the parent
		-Array of pointers to the children
	Yggdrasil's nodes are allowed at max 4 children, in Yggdrasil, a leaf node
	specifies when all the moves from that node results in a board position we have already
	seen in this attempt, so making that move would be backtracking and as such
	that path is unlikely to be shortest, so we do not go any farther down that path.
	This module will have functions to do the following:
		-Create a node
		-Insert it into Yggdrasil
		-traverse up from the winning node to all the way to the root to find
		what the shortest path was.
	We'll know what the winning board position is because its hash value will be WINNINGHASH,
	once we find a position with that hash we stop.
	Yggdrasil will be filled breath-first, so this way we can be assured the first time
	we hit WINNINGHASH, there is no shorter path because we've checked every move from the root
	and the number of moves corresponds to the node depth.
AUTHORS:  Andrew Gendreau
*/

#ifndef YGGDRASIL_H_
#define YGGDRASIL_H_
#include "yggdrasil.h"
#endif

#ifndef PATH_H_
#define PATH_H_
#include "pathfinder.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct tree *treeRoot = NULL;							//root of Yggdrasil
extern int currentDepth;							//depth of the nodes we're working with right now

/**
*Name: initializeMiniTree
*Purpose: This function will set the root of a mini Yggdrasil
	as the given node struct. It will also set the root global
	variable.
*Parameters: 
*	newRoot --node to be made the root of Yggdrasil
*Returns:
	void
**/
struct tree *initializeMiniTree(struct treeNodeParentMPI newRootNode)
{
	struct tree *newRoot;							//newly formed Yggdrasil root
	struct treeNode *newTreeNode;						//treeNode form of the given parent struct
	
	newRoot = malloc(sizeof(struct tree));

	newTreeNode = createNode(newRootNode.depth, newRootNode.hashVal, newRootNode.board, 0, 0);

	strcpy(newTreeNode->moveFromParent, newRootNode.moveFromParent);

	newRoot->root = newTreeNode;

	treeRoot = newRoot;

	return newRoot;
}

/**
*Name: initializeTree
*Purpose: This function will set the root of Yggdrasil
	as the given node struct. It will also set the root global
	variable.
*Parameters: 
*	newRoot --node to be made the root of Yggdrasil
*Returns:
	void
**/
void initializeTree(struct treeNode *newRootNode)
{
	struct tree *newRoot;							//newly formed Yggdrasil root
	
	newRoot = malloc(sizeof(struct tree));

	newRoot->root = newRootNode;

	treeRoot = newRoot;

	currentDepth = 0;
}

/**
*Name: createNode
*Purpose: This function will, given a hash value, a node depth,
	a board, the tile moved and the direction moved from the parent,
	create that node and return a pointer to it.
*Parameters: 
*	hashVal --hash value for a given board position.
	depth --depth of this node
	board --board of this node
	tileMoved --the tile we've moved
	directionMoved --the direction we moved
*Returns:
	pointer to a new node
**/
struct treeNode *createNode(int depth, long int hashVal, int board[BOARDSIZE], int tileMoved, char directionMoved)
{
	struct treeNode *newNode;						//new node to be returned
	int i;									//loop control variable
	char moveChar[2];							//character array to hold the char form of the tile we moved
	char *move;								//string to hold the actual move, like 4U
	char direction[2];							//char array to hold the direction we moved in

	newNode = malloc(sizeof(struct treeNode));

	newNode->hashVal = hashVal;
	newNode->depth = depth;
	newNode->nodeID = hashVal + depth;
	move = malloc(sizeof(char) * 4);					//because it will be a length 4 length string at max

	memcpy(newNode->board, board, BOARDSIZE *sizeof(int));

	if(tileMoved != 0)							//if we actually moved, if we're the root, tileMoved is 0
	{
		sprintf(moveChar, "%d", tileMoved);
		sprintf(direction, "%c", directionMoved);
		strcpy(move, moveChar);
		strcat(move, direction);
		strcpy(newNode->moveFromParent, move);
	}
	else
	{
		newNode->moveFromParent[0] = -1;
	}

	for(i = 0; i < MAXCHILDREN; i++)					//set the children array to NULL
	{
		newNode->children[i] = NULL;
	}

	return newNode;
}

/**
*Name: insertIntoYggdrasil
*Purpose: This function will, given a pointer to a treeNode, and its
	parent, insert that node into Yggdrasil. It will have to set the
	parent pointer of that node and will have to cycle through the
	children pointer array of the parent until it hits the first one that
	is not null and insert there.
*Parameters: 
*	nodeToInsert --node to insert into the tree.
	nodeToBeParent --The to be parent of this node
*Returns:
*	integer of the index in children the newly inserted node was inserted into
	if we inserted the root, the return will be -1
**/
int insertIntoYggdrasil(struct treeNode *nodeToInsert, struct treeNode *nodeToBeParent)
{
		int i;							//loop control variable
		struct treeNodeParentMPI *parentCopy;			//variable to hold an actual parentCopy value to use for MPI

		if(nodeToBeParent == NULL)				//if we're inserting at the root
		{
			initializeTree(nodeToInsert);
			return -1;
		}
		else
		{
			parentCopy = malloc(sizeof(struct treeNodeParentMPI));
			memcpy(parentCopy->board, nodeToBeParent->board, sizeof(parentCopy->board));
			parentCopy->hashVal = nodeToBeParent->hashVal;
			parentCopy->depth = nodeToBeParent->depth;
			
			if(nodeToBeParent->moveFromParent[0] == -1)
			{
				//do nothing, there is no move to copy
			}
			else
			{
				strcpy(parentCopy->moveFromParent, nodeToBeParent->moveFromParent);
			}
			parentCopy->nodeID = nodeToBeParent->nodeID;

			nodeToInsert->parent = nodeToBeParent;
			nodeToInsert->parentCopy = *parentCopy;
			i = 0;
			while(nodeToBeParent->children[i] != NULL)
			{
				i++;
			}

			nodeToBeParent->children[i] = nodeToInsert;
			return i;
		}		
}

/**
*Name: traverseWinningPath
*Purpose: This function will, given a pointer to 
	the winning node, traverse the tree up to the root and 
	create a string to hold the moves made by appending the move in
	node i to the result string in reverse.
*Parameters: 
*	winningNode --Node that had a hash of 0
*Returns:
*	character pointer to the string that is the winning path
**/
char *traverseWinningPath(struct treeNode *winningNode)
{
	char *resultString;						//character string to hold the winning path
	struct treeNode *currentNode;					//current node we are looking at
	char *temp;							//tempory storage for the reverse append

	currentNode = winningNode;
	
	resultString = malloc(sizeof(char) * (2*winningNode->depth)); 

	strcat(resultString, currentNode->moveFromParent);

	while(currentNode->depth > treeRoot->root->depth)				//append until we hit the root
	{
		currentNode = currentNode->parent;
		temp = strdup(resultString);
		strcpy(resultString, currentNode->moveFromParent);
		strcat(resultString, " ");
		strcat(resultString, temp);
	}
return resultString;
}
