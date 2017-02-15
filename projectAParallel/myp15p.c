/*NAME: mys15p.c
DATE: Oct 14th 2016
VERSION: 2.0
FILENAME: mys15p.h
PURPOSE: This module is the main invoker module for myp15p.
	It will prompt the user for input, parse the input to 
	remove the newline character, then invoke the solvability
	tester upon the input. It will inform the user if he/she has
	entered an unsolvable case, if so it will tell them then die,
	if not, it will run the algorithm to locate the shortest path to 
	a solution. In this version, there will be a definition of an MPI
	struct for transmission of Yggdrasil treeNodes. This uses a master-slave
	paradigm, there is one "master" node, this is process 0, and the others
	are "slaves." The master node coordinates the other nodes. The parallel logic
	works as follows:
	Master Node logic:
		1. The master node will apply the serial algorithm until the next level
		has enough nodes to pass at least one to each executing process. 
		2. If there is a remainder, the master node will spread it out as evenly as 
		possible among the processes by going through them all and giving them one extra
		until we run out of remainder. 
		3. Next, it will send the number of processes each process will recieve to each slave process,
		this is to ensure each one knows how many to expect.
		4. Next, the master node will send each process the nodes it will work on. 
		5. The master will wait until a process finishes and then use use reduce to find the 
		process with the smallest winning depth. 
		6. Once it finds this, it will issue a recieve for the character string and length and build the winning path.
	Slave Node logic:
		The slaves will basically apply the serial algorithm using the parent of the first node it recieved as a root.
			1. The slaves will recieve the number of nodes to expect and the nodes themselves.
			2. The slaves will then apply the idea of the serial algorithm, however, they will issue a non-blocking recieve
				this is so each one can recieve the depth of a process that won. 
			3. If a process wins, it will send the depth of the node that won to all the other slaves, if a slave gets a winning
				depth and it is smaller than its current one, it will set its own winning depth to a negative number to 
				signify it is a "loser" process.
			4. A process may end up a dead end, in which it may not be able to spawn any more new board positions, the process
			will then do as it would in step 3, use a negative value to signify a "loser."
		
AUTHORS:  Andrew Gendreau
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <time.h>
#include "myp15p.h"

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
	int j;								//another loop control variable
	int nodesToSendToThisProcess;					//nodes we are going to send to this process	
	int my_PE_num;							//my processing element number
	int numprocs;							//the number of executing processes
	int haveWeFoundAWinner;						//integer to signal us if we've won
	int leftOver;							//remainder of the number of nodes in the queue once we've generated
									//enough
	int numberOfNodesToSend;					//number of nodes to send to each process from the master
	int originalNumberOfNodes;					//original number of nodes from the queue to send assuming no
									//remainder
	MPI_Status status;						//MPI status variable, needed for non-blocking sends and recieves
	int winningDepth;						//Winning depth
	char *victoryPath;						//character pointer, used to point to the moves that won
	clock_t start;							//clock_t variable to hold the starting time information
	clock_t end;							//clock_t variable to hold the ending time information
	clock_t timeDiff;						//clock_t variable to hold the difference between previous 2
	double millSeconds;						//time it took in milliseconds
	int blankPositionFromBottom;
	double seconds;

	victoryPath = NULL;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);			//this places the size of the world group (# of PEs) into
									//numprocs
	MPI_Comm_rank(MPI_COMM_WORLD, &my_PE_num );			//places all the PEs in the world group and gives them #s

	int nodesPerProcess[numprocs];					//array to hold how many nodes each process will recieve

	struct								//This is called an anonymous struct, it is used in this case
									//for MAXLOC to implement MPI_2INT 
	{
		int myValue;						//the winning depth of each process
		int myRank;						//the process rank
	} buff, out;

	buff.myRank = my_PE_num;

	const int nitems = 6;									//number of fields in the struct
	int blockLengths[6] = {16,1,1,4,1,(int)sizeof(struct treeNodeParentMPI)};		//number of things in each field
	MPI_Datatype types[6] = {MPI_INT, MPI_LONG, MPI_INT, MPI_CHAR, MPI_INT, MPI_BYTE};	//the types of each field
	MPI_Datatype MPI_YGGDRASILNODE;								//the name of the new datatype
	MPI_Aint offsets[6];									//an array to hold the memory offsets of
												//each field, easily obtained via offsetof
	
	//The following assignments set an array of offset values for each field in the struct
	offsets[0] = offsetof(struct treeNode, board);
	offsets[1] = offsetof(struct treeNode, hashVal);
	offsets[2] = offsetof(struct treeNode, depth);
	offsets[3] = offsetof(struct treeNode, moveFromParent);
	offsets[4] = offsetof(struct treeNode, nodeID);
	offsets[5] = offsetof(struct treeNode, parentCopy);

	MPI_Type_create_struct(nitems, blockLengths, offsets, types, &MPI_YGGDRASILNODE);
	MPI_Type_commit(&MPI_YGGDRASILNODE);

	if(my_PE_num == 0)						//if we're the master node
	{
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

			i = 1;

			createRoot(oneDInputBoard);

			haveWeFoundAWinner = processLevel(1, victoryPath);

			while(currentLevelNodeCount < numprocs && haveWeFoundAWinner < 0)		
			{
				haveWeFoundAWinner = processLevel(currentLevelNodeCount, victoryPath);
			}

			if(haveWeFoundAWinner > 0)
			{
				victoryPath = traverseWinningPath(winner);
				printf("Our winning path needs %d moves\n", (winner->depth));
				printf("The moves required are: %s\n", victoryPath);
				return 0;
			}
			else								
			{
				buff.myValue = -10;

				originalNumberOfNodes = currentLevelNodeCount / (numprocs-1);
				numberOfNodesToSend = originalNumberOfNodes;
				leftOver = currentLevelNodeCount % (numprocs-1);

				while(i < numprocs)
				{
					if(leftOver > 0)					//if we have leftovers
					{
						numberOfNodesToSend++;
						MPI_Send(&numberOfNodesToSend, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
						nodesPerProcess[i-1] = numberOfNodesToSend;
						i++;
						leftOver--;
						numberOfNodesToSend = originalNumberOfNodes;
					}
					else
					{
						nodesPerProcess[i-1] = originalNumberOfNodes;
						MPI_Send(&numberOfNodesToSend, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
						i++;
					}
				}

				i = 1;

				while(i < numprocs)		//send to all the processes
				{
					nodesToSendToThisProcess = nodesPerProcess[i-1];
					struct treeNode treeNodeArray[nodesToSendToThisProcess];
					struct treeNode *nodePtr;	//temporary variable

					for(j = 0; j < nodesToSendToThisProcess; j++)
					{
						nodePtr = deQueue();
						treeNodeArray[j] = *nodePtr;

					}
					MPI_Send(&treeNodeArray, nodesToSendToThisProcess, MPI_YGGDRASILNODE, i, 1, MPI_COMM_WORLD);
					i++;
				}
			}

		}
		fclose(filePointer);
	}
	else
	{
		int j;							//loop control variable
		struct treeNode *currentNodePtr;			//pointer to the current treeNode
		struct tree *branchRoot;				//root of this process' Yggdrasil tree
		int haveWeFoundAWinner;					//integer to test if we've won
		char *pathOfVictory;					//character pointer to the winning path
		int depthRecieved;					//the depth from the last winner we've recieve
		MPI_Request request;					//MPI_Request variable
		int messageFlag;					//flag to signify we've recieved a messaged
		int nodesToRecieve;

		messageFlag = 0;
		depthRecieved = 99999;					//set to a random really high number at first so we know we
									//haven't recieved anything
		haveWeFoundAWinner = -1;
		pathOfVictory = NULL;

		MPI_Recv(&nodesToRecieve, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		struct treeNode treeNodeArray[nodesToRecieve];

		MPI_Recv(&treeNodeArray, nodesToRecieve, MPI_YGGDRASILNODE, 0, 1, MPI_COMM_WORLD, &status);

		for(j = 0; j < nodesToRecieve; j++)				//insert all the obtained nodes as 
											//the root node's children and enqueue them
		{
			currentNodePtr = &treeNodeArray[j];

			if(j == 0)							//i don't want to enqueue this node
			{
				currentDepth = 1;

				branchRoot = initializeMiniTree(treeNodeArray[0].parentCopy);

				createEmptyHashList();

				haveISeenThisPosition(branchRoot->root->hashVal);

				insertIntoYggdrasil(currentNodePtr, branchRoot->root);
			}
			else
			{
				insertIntoYggdrasil(currentNodePtr, branchRoot->root);
			}
			enQueue(currentNodePtr);
		}

		haveWeFoundAWinner = processLevel(nodesToRecieve, pathOfVictory);

		MPI_Irecv(&depthRecieved, 1, MPI_INT, MPI_ANY_SOURCE, 10, MPI_COMM_WORLD, &request);

		MPI_Test(&request, &messageFlag, &status);

		while(haveWeFoundAWinner < 0 && depthRecieved > currentDepth)
		{
			MPI_Test(&request, &messageFlag, &status);

			if(messageFlag > 0)				//if we've recieved a message, check if we need to do another level
			{
				MPI_Wait(&request, &status);

				if(depthRecieved < currentDepth)
				{
					continue;
				}
				else
				{
					MPI_Irecv(&depthRecieved, 1, MPI_INT, MPI_ANY_SOURCE, 10, MPI_COMM_WORLD, &request);
				}
			}
			else						
			{
				haveWeFoundAWinner = processLevel(currentLevelNodeCount, pathOfVictory);
			}
		}
		winningDepth = haveWeFoundAWinner;
		buff.myValue = winningDepth;

		if(winner != NULL && winningDepth > 0 && winningDepth < depthRecieved)		//This is when a process finds a winner
		{
			MPI_Cancel(&request);							//cancel a request if we have one
			pathOfVictory = traverseWinningPath(winner);
			int pathLength;								//this is the length of the winning path
			pathLength = (int) strlen(pathOfVictory);
			int pathAndDepth[2];							//this array holds the winning depth
												//and the path length (number of moves)

			pathAndDepth[0] = pathLength;
			pathAndDepth[1] = currentDepth;

			for(j = 1; j < numprocs; j++)					//tell everybody except myself I found a winner
			{
				if(j != my_PE_num)
				{
					MPI_Send(&winningDepth, 1, MPI_INT, j, 10, MPI_COMM_WORLD);
				}
			}
			MPI_Send(&pathAndDepth, 2, MPI_INT, 0, 20, MPI_COMM_WORLD);
			MPI_Send(pathOfVictory, pathLength, MPI_CHAR, 0, 30, MPI_COMM_WORLD);
		}
	}

	MPI_Reduce(&buff, &out, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);

	if(my_PE_num == 0)
	{
		int depthAndLength[2];								//Variable to hold the send from the winner
												//from process 0.
		end = clock();
		timeDiff = end - start;
		millSeconds = timeDiff * 1000 / CLOCKS_PER_SEC;

		seconds = millSeconds / 1000.0;

		MPI_Recv(&depthAndLength, 2, MPI_INT, out.myRank, 20, MPI_COMM_WORLD, &status);

		printf("The shortest path has %d moves\n", (currentDepth + depthAndLength[1]));
		printf("Solving this puzzle took %f seconds when measured from process 0\n", seconds);
	}
	MPI_Finalize();
}
