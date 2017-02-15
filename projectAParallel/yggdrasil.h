/*NAME: hashList.c
DATE: Oct 14th 2016
VERSION: 1.0
FILENAME: yggdrasil.h
PURPOSE: This module will be a tree to use to find the shortest path.
	In each node of the tree, what will be stored will be:
		-node depth (int)
		-current board in 1d array form (array of ints)
		-move made from the parent (character string of such form as: 4U (4 moved up)
		-hash value of the board (int)
		-Node ID: (hash value + node depth)
		-Pointer to the parent
		-Array of pointers to the children
	Yggdrasil's nodes are allowed at max 4 children, in Yggdrasil, a leaf node
	specifies when the move from that node results in a board position we have already
	seen in this solution, so making that move would be backtracking and as such
	that path is unlikely to be shortest, so we do not go any farther.
	This module will have functions to do the following:
		-Create a node
		-Insert it into Yggdrasil
		-Find how many children a node will have
		-traverse up from the winning node to all the way to the root to find
		what the shortest path was.
	We'll know what the winning board position is because its hash value will be 0,
	once we find a position with that hash we stop.
	Yggdrasil will be filled breath-first, so this way we can be assured the first time
	we hit 0, there is no shorter path because we've checked every move from that board position
	and the number of moves corresponds to the node depth.
AUTHORS:  Andrew Gendreau
*/

#ifndef HASH_H_
#define HASH_H_
#include "hashList.h"
#endif

#include <stdlib.h>

#define MAXCHILDREN 4


/*
	This is a struct to represent a node in Yggdrasil.
	Contains the above mentioned variables as well as a pointer to
	this node's parent and an array of pointers to its children.
	The parent pointer will be null for the root and the children that
	are not possible will be null as well.
*/

struct treeNodeParentMPI			//this is a definition I need simply to transfer the parent using an MPI datatype 
{
	int board[BOARDSIZE];
	long int hashVal;
	int depth;
	char moveFromParent[4];
	int nodeID;
};

struct treeNode 
{
	int board[BOARDSIZE];
	long int hashVal;
	int depth;
	char moveFromParent[4];
	int nodeID;
	struct treeNode *parent;
	struct treeNode *children[MAXCHILDREN];
	struct treeNodeParentMPI parentCopy;
};

struct tree
{
	struct treeNode *root;
};

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
struct tree *initializeMiniTree(struct treeNodeParentMPI newRootNode);

/**
*Name: createNode
*Purpose: This function will, given a hash value, a node depth,
	and a board, create that node and return a pointer to it.
*Parameters: 
*	hashVal --hash value for a given board position.
	depth --depth of this node
	board --board of this node
*Returns:
	pointer to a new node
**/
struct treeNode *createNode(int depth, long int hashVal, int board[BOARDSIZE], int tileMoved, char directionMoved);

/**
*Name: insertIntoYggdrasil
*Purpose: This function will, given a pointer to a treeNode,
	insert that node into the tree. It will have to set the
	parent pointer of that node and will have to cycle through the
	children pointer array until it hits the first one that
	is not null and insert there.
*Parameters: 
*	nodeToInsert --node to insert into the tree.
*Returns:
*	void
**/
int insertIntoYggdrasil(struct treeNode *nodeToInsert, struct treeNode *nodeToBeParent);

/**
*Name: traverseWinningPath
*Purpose: This function will, given a pointer to 
	the winning node, traverse the tree up to the root and 
	create a string to hold the moves made by appending the move in
	node i to the beginning of the current pathString.
*Parameters: 
*	winningNode --Node that had a hash of 0
*Returns:
*	character pointer to the string that is the winning path
**/
char *traverseWinningPath(struct treeNode *winningNode);

/**
*Name: traverseSerialPath
*Purpose: This function will, given a pointer to 
	the end of the serial executuion node, traverse the tree up to the root and 
	create a string to hold the moves made by appending the move in
	node i to the result string in reverse.
*Parameters: 
*	winningNode --Node that had a hash of 0
*Returns:
*	character pointer to the string that is the winning path
**/
char *traverseSerialPath(struct treeNode *winningNode);
