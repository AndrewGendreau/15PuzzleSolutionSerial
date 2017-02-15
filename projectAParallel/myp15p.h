/*NAME: mys15p.c
DATE: Oct 14th 2016
VERSION: 2.0
FILENAME: mys15p.h
PURPOSE: This is a header for myp15p.c	
AUTHOR: Andrew Gendreau
 */
#ifndef MPI_H_
#define MPI_H_
#include "mpi.h"
#endif

#ifndef YGGDRASIL_H_
#define YGGDRASIL_H_
#include "yggdrasil.h"
#endif

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

#ifndef QUEUE_H_
#define QUEUE_H_
#include "queue.h"
#endif

extern int nextLevelNodeCount;						//inital value of the next level counter
extern int currentLevelNodeCount;					//node count at the current level
extern struct tree *treeRoot;						//root of the tree of board positions and moves
struct treeNode *winner = NULL;						//winning node
int endingSignal;							//global integer to signify we've ended (I might remove it)
extern int currentDepth;						//depth of the nodes we're working with right now

#define LINESIZE 200							//arbitary limit on user fileName
