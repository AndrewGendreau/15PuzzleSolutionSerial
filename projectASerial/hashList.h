/*NAME: hashList.h
DATE: Oct 14th 2016
VERSION: 2.0
FILENAME: hashList.h
PURPOSE: This is the header file for hashList.c.
AUTHOR: Andrew Gendreau
*/

#define BUCKETSIZE 10						//bucket size for the hash table
#define WINNINGHASH 1311768467463790320				//integer of the winning position

#ifndef PATH_H_
#define PATH_H_
#include "pathfinder.h"
#endif
/*
* This struct will be a bucket in the hash table, it will hold an array 
* of BUCKETSIZE, so this way, values can be represented as flags, for example,
* value 25 will be in index 4 in bucket 2. the value in bucketArray[i] will be
* -1 if the value has not been seen, and will be 1 if it has. It will also have pointers
* to its neighbors, there will be no wraparound link.
*/
struct hashBucket					
{
	long int bucketArray[BUCKETSIZE];			//array to hold the flags for this bucket, the values will be either -1 or 1
								//-1 will signify a value we have not seen and 1 represent one we have.
	long int bucketID;					//ID for this bucket
	struct hashBucket *lastBucket;				//pointer to the bucket behind this one.
	struct hashBucket *nextBucket;				//pointer to the bucket in front of this one.
};

/*
* This struct will contain a pointer to the head of the 
* bucket list.
*/
struct hashList
{
	struct hashBucket *firstBucket;				//header of the bucket list
};

//testing method, will probably erase this
void traverseList();

/**
*Name: createEmptyHashList
*Purpose: This function will create a new hash table.
	It will initalize the head pointer and initally set the
	firstBucket pointer to null to signify it is empty.
*Parameters: 
	void
*Returns:
	void
*Author: Andrew Gendreau
**/
void createEmptyHashList();

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
*Author: Andrew Gendreau
**/
int haveISeenThisPosition(long int hashVal);

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
*Author: Andrew Gendreau
**/
struct hashBucket *createBucket(long int bucketID, long int valIndex);

/**
*Name: insertBucket
*Purpose: This function will, given a pointer to a hashbucket,
	insert that bucket into the bucketList. It will have 2 cases to handle:
	1: inserting into the ends of the list (will need to handle head and tail)
	2: Inserting into the middle of the list
*Parameters: 
*	bucket --bucket to insert
*Returns:
*Author: Andrew Gendreau
**/
void insertBucket(struct hashBucket *BucketPointer);

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
*Author: Andrew Gendreau
**/
long int computeHashValue(int board[BOARDSIZE]);

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
*Author: Andrew Gendreau
**/
long int findBucket(long int hashVal);

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
*Author: Andrew Gendreau
**/
long int findIndex(long int hashVal);
