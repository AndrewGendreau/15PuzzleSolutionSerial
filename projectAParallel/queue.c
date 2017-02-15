/*NAME: queue.c
DATE: Oct 14th 2016
VERSION: 2.0
FILENAME: queue.c
PURPOSE: This module will be a queue to be used in the breadth first traversal.
	It will consist of a list of queue structs that each hold a tree node.
	It will keep two variables to track the number of nodes in the queue that 
	on the current level we're working with and the nodes on the next level.
AUTHORS:  Andrew Gendreau
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef YGGDRASIL_H_
#define YGGDRASIL_H_
#include "yggdrasil.h"
#endif

#ifndef QUEUE_H_
#define QUEUE_H_
#include "queue.h"
#endif


struct queue *Queuehead = NULL; 					//head of the queue
struct queue *Queuetail = NULL;						//tail of the queue

int currentLevelNodeCount = 0;						//inital value of the current level counter
int nextLevelNodeCount = 0;						//inital value of the next level counter

/**
*Name: enQueue
*Purpose: This function will, given a treeNode to insert into
	the queue, will insert it. It will encase the given node in
	a queue node and will insert at the tail of the queue.
	It will also handle the case of an empty queue by setting the new
	node as both the tail and head of the queue, otherwise, it'll 
	set the new node to be the tail.
*Parameters: 
*	inputNode --treeNode to insert
*Returns:
	void
**/
void enQueue(struct treeNode *inputNode)
{
	struct queue *newNode; 							//new node to create

	newNode = malloc(sizeof(struct queue));

	if(newNode == NULL)							//check for malloc errors
	{
		printf("Malloc error!\n");
		exit(1);
	}

	newNode->node = inputNode;
	newNode->next = NULL;
	
	if(Queuehead == NULL && Queuetail == NULL)
	{
		Queuehead = newNode;
		Queuetail = newNode;
		return;
	}

	Queuetail->next = newNode;
	Queuetail = newNode;
	
}

/**
*Name: deQueue
*Purpose: This function will remove a node from the queue. If the queue is empty,
	it will return NULL. Otherwise, it will return the node at the head of the queue.
	If the node to be returned was the only one in the queue, it will set the head and 
	tail pointers to NULL. Otherwise, it will set the head to be the next node after the head
*Parameters: 
*	void
*Returns:
	treeNode that was at the head of the queue
**/
struct treeNode *deQueue(void)
{
	struct treeNode *output;

	if(Queuehead == NULL)
	{
		return NULL;
	}

	if(Queuehead == Queuetail)
	{
		output = Queuehead->node;
		Queuehead = NULL;
		Queuetail = NULL;
	}
	else
	{
		output = Queuehead->node;
		Queuehead = Queuehead->next;
	}

	return output;
}

/**
*Name: queueEmpty
*Purpose: This function will check if the queue is empty, if it is
	it will return 1, if not, it will return 0.
*Parameters: 
*	void
*Returns:
	1 if queue is empty
	0 if it is not
**/
int queueEmpty(void)
{
	if(Queuehead == NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
