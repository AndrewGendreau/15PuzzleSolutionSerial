/*NAME: hashList.c
DATE: Oct 14th 2016
VERSION: 2.0
FILENAME: hashList.h
PURPOSE: This module will be a bucket based hash
	table for the game board. The hash table will consist
	of a linked list of buckets, these buckets will be indexed
	by the bucket size, so if our bucket size is 10, bucket 0
	will contain values 0-9, bucket 1 values 10-19, etc.
	Each bucket will have an array of size bucket size that 
	corresponds to every value in that bucket.
	The idea will work like this:
	We'll compute a hash value for the current game board,
	if that value is WINNINGHASH (defined in hashList.h),
	we know we've won due to the design of the hash function. 
	If it is not that, we will mark that board as 
	having been seen before using the following heuristic:
	Check the buckets of the hash table to see which bucket it will
	go to, then either create that bucket if we have not seen it 
	and mark the value in that bucket so we know we've seen it before.
	Or, go to that bucket and check the needed spot to see if we've 
	seen it, if we have, let the caller know, if not, mark that value,
	then tell the caller.
AUTHORS:  Andrew Gendreau
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef PATH_H_
#define PATH_H_
#include "pathfinder.h"
#endif


#include "hashList.h"

struct hashList *head = NULL;							//head of the doubly linked list of buckets

/**
*Name: createEmptyHashList
*Purpose: This function will create a new hash table.
	It will initalize the head pointer and initally set the
	firstBucket pointer to null to signify it is empty.
*Parameters: 
	void
*Returns:
	void
**/
void createEmptyHashList()
{
	struct hashList *newHead;						//variable to hold the new head

	newHead = malloc(sizeof(struct hashList));

	newHead->firstBucket = NULL;

	head = newHead;
}

/**
*Name: InitalizeBucketArray
*Purpose: This function will initalize a newly created bucket's
	array to -1 to signify so far unseen values. It'll just run
	through that bucket's array filling it with -1
*Parameters: 
*	BucketPointer --pointer to the bucket we need to fill
*Returns:
	void
**/
void InitalizeBucketArray(struct hashBucket *bucketPointer)
{
	int i;									//loop control variable


	for(i = 0; i < BUCKETSIZE; i++)
	{
		bucketPointer->bucketArray[i] = -1;
	}
}

/**
*Name: createBucket
*Purpose: This function will, given an ID for a bucket, and
	the index of the value that caused this bucket's creation,
	create said bucket and return it. It will also call a function
	to initalize the array in the bucket.
*Parameters: 
*	bucketID --ID for this bucket
	valIndex --this is the index of the value that caused this bucket's creation
*Returns:
	pointer to the created bucket.
**/
struct hashBucket *createBucket(long int bucketID, long int valIndex)
{
	struct hashBucket *newBucket;						//pointer to the newly created bucket
	
	newBucket = malloc(sizeof(struct hashBucket));
if(newBucket == NULL)
{
	printf("I ran out of hash bucket memory\n");
}
	newBucket->bucketID = bucketID;
	newBucket->lastBucket = NULL;
	newBucket->nextBucket = NULL;
	InitalizeBucketArray(newBucket);

	newBucket->bucketArray[valIndex] = 1;
	return newBucket;
}

/**
*Name: insertBucket
*Purpose: This function will, given a pointer to a hashbucket,
	insert that bucket into the bucketList. It will have 2 cases to handle:
	1: inserting into the ends of the list (will need to handle head and tail)
	2: Inserting into the middle of the list
*Parameters: 
*	bucket --bucket to insert
*Returns:
**/
void insertBucket(struct hashBucket *bucket)
{
	struct hashBucket *currentBucket;				//bucket we're working with now
	struct hashBucket *rightNeighborTemp;				//pointer to the new bucket's right neighbor

	currentBucket = head->firstBucket;

	if(currentBucket == NULL)					//if we're inserting into an empty list, version 1 of case 1
	{								//in this case, we can just leave the neighbor pointers null
		head->firstBucket = bucket;

		return;
	}
	else if(currentBucket->bucketID > bucket->bucketID)		//check if we're inserting at the head of a non-empty list
	{
		head->firstBucket = bucket;

		currentBucket->lastBucket = bucket;

		bucket->nextBucket = currentBucket;

		return;
	}
	else								//otherwise, we are dealing with either case 2
	{
		while(currentBucket != NULL)
		{
									//if we're inserting in the middle of the list

			if(currentBucket->bucketID < bucket->bucketID && currentBucket->nextBucket != NULL)
			{
				rightNeighborTemp = currentBucket->nextBucket;

									//check if we've found the spot to insert into

				if(rightNeighborTemp->bucketID > bucket->bucketID)
				{
					currentBucket->nextBucket = bucket;

					bucket->nextBucket = rightNeighborTemp;

					rightNeighborTemp->lastBucket = bucket;

					return;
				}
			}
									//if we're inserting into the tail of the list

			else if(currentBucket->bucketID < bucket->bucketID && currentBucket->nextBucket == NULL)
			{
				currentBucket->nextBucket = bucket;

				return;
			}
		currentBucket = currentBucket->nextBucket;
		}
	}
}

/**
*Name: findBucket
*Purpose: This function will, given a hash value, compute the
	bucket that value should be in. It will do this as follows:
		hashVal/bucketSize
	this will be integer division so the remainder will be truncated.
*Parameters: 
*	hashVal --hash value for a given board position.
*Returns:
	ID of the bucket to insert into.
**/
long int findBucket(long int hashVal)
{
	long int whichBucket;					//variable to hold the bucket the hashvalue belongs in

	whichBucket = hashVal / BUCKETSIZE;

	return whichBucket;
}

/**
*Name: findIndex
*Purpose: This function will, given a hash value, compute the
	index that value will be inside in the bucket. It will do this as follows:
		(hashVal mod bucketSize) - 1
	this will return the index of the value in the bucket.
*Parameters: 
*	hashVal --hash value for a given board position.
*Returns:
	index of the hash value
**/
long int findIndex(long int hashVal)
{
	long int bucketIndex;					//index of the element in the bucket

	bucketIndex = hashVal % BUCKETSIZE;

	return bucketIndex;
}

/**
*Name: haveISeenThisPosition
*Purpose: This function will, given a hash value,
	check the hash table for if that hash value exists in the
	hash table already, if so, it will return 1 and change
	the flag in that bucket for that index, if not,
	return -1. We'll do this by traversing the list until
	we find the target bucket and then check index i of that
	bucket's array and return its value.
*Parameters: 
*	hashVal --hash value for a given board position.
*Returns:
	1 if given hash already existed
	-1 if it did not.
**/
int haveISeenThisPosition(long int hashVal)
{
	struct hashBucket *currentBucket;			//bucket we're working with now
	long int targetBucket;					//bucket of the target value
	long int targetIndex;					//index of the target value

	targetBucket = findBucket(hashVal);
	targetIndex = findIndex(hashVal);

	if(head->firstBucket == NULL)				//check for the case where I'm the first hash value computed
	{
		//printf("I'm in the null head if\n");
		currentBucket = createBucket(targetBucket, targetIndex);
		insertBucket(currentBucket);
		return -1;
	}

	currentBucket = head->firstBucket;

	while(currentBucket->bucketID != targetBucket)
	{
		if(currentBucket->nextBucket == NULL && currentBucket->bucketID != targetBucket)
		{
			currentBucket = NULL;
			break;
		}
		currentBucket = currentBucket->nextBucket;
	}

	if(currentBucket == NULL)				//if that bucket did not exist
	{
			currentBucket = createBucket(targetBucket, targetIndex);
			insertBucket(currentBucket);
			return -1;
	}
	else
	{
		if(currentBucket->bucketArray[targetIndex] == -1)	//if I haven't seen this value
		{
			currentBucket->bucketArray[targetIndex] = 1;	//set its flag so that I know I have seen it now
			return -1;
		}
		else							//otherwise I have seen it before
		{
			return 1;
		}
	}
}

/**
*Name: computeHashValue
*Purpose: This function will, given a 1d array to
	represent the board, compute the hash value representing that
	board. The hash value will be as follows:
		Compute a string of hexidecimal digits, each digit will
		be the the hex digit for the number in that place. As an example,
		the winning state as a hex string is:
			123456780abcdef0
		It will then make use of strtol to turn this string into a long
		int representing that board state. This has the advantage of being assured to be
		unique, as no two different board states will ever have the same hex string and thus,
		never have the same hash value
*Parameters: 
*	board --array of ints of size BOARDSIZE to represent the board.
*Returns:
	hash value of the board as a long int
**/
long int computeHashValue(int board[BOARDSIZE])
{
	int i;							//loop control variable, will also represent the current board position
	char hexBoard[BOARDSIZE];				//char array to hold the hex form of the current board
	long int hashValue;					//long int to store the hash value
	char *endPtr;						//character pointer for use in strtol

	for(i = 0; i < BOARDSIZE; i++)				//for all 16 positions
	{
		char hexChar[2]; 				//array of length 2 to hold this hex digit, length 2 to make it a string 		
		sprintf(hexChar, "%x", board[i]);
		hexBoard[i] = hexChar[0];
	}
	hexBoard[i] = '\0';

	hashValue = strtol(hexBoard, &endPtr, 16);

	return hashValue;
}
