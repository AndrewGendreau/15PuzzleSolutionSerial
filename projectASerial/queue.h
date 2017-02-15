/*NAME: queue.h
DATE: Oct 14th 2016
VERSION: 2.0
FILENAME: queue.h
PURPOSE: This is a header for queue.c
AUTHOR: Andrew Gendreau
*/


/*
	This is a struct that will be a node in the queue. The struct will have a treeNode pointer and pointer to 
	the queue node following it.
*/
struct queue 				
{
	struct treeNode *node;				//node in this queue node
	struct queue *next;				//pointer to the next node in the queue
};


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
*Author: Andrew Gendreau
**/
void enQueue(struct treeNode *inputNode);

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
*Author: Andrew Gendreau
**/
struct treeNode *deQueue(void);	

/**
*Name: queueEmpty
*Purpose: This function will check if the queue is empty, if it is
	it will return 1, if not, it will return 0.
*Parameters: 
*	void
*Returns:
	1 if queue is empty
	0 if it is not
*Author: Andrew Gendreau
**/
int queueEmpty(void);
