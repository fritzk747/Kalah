#include "board.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include <iostream>
using namespace std;



//default constructor
Board::Board(){	
	//first row is player 1, second row is player 2
	//for each row, house number 1 (index 0) is on the far left of the player when they look at the board. Therefore house 6 (index 5) is immediately before each player's store
	//the last index in each row is for corresponding players store
	curBoard = new int*[2];
	for(int i = 0; i < 2; i++){
		curBoard[i] = new int[7];
		for(int j = 0; j < 7; j++){
			curBoard[i][j] = 0;
		}
	}
}


Board::Board(int **board)
{
	//first row is player 1, second row is player 2
	//for each row, house number 1 (index 0) is on the far left of the player when they look at the board. Therefore house 6 (index 5) is immediately before each player's store
	//the last index in each row is for corresponding players store
	curBoard = new int*[2];
	for(int i = 0; i < 2; i++){
		curBoard[i] = new int[7];
		for(int j = 0; j < 7; j++){
			curBoard[i][j] = board[i][j];
		}
	}
	
	
}


//numSeeds = number of seeds per player (assumed equal number for each player)
//randomize set to 1 if seeds are placed randomly, 2 if they are placed equally
void Board::startGame(int numSeeds, int randomize){
	double randNum;
	int randNumInt;
	
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 7; j++){
			curBoard[i][j] = 0;
		}
	}		
	
	
	//if seeds are randomized to start
	if(randomize == 1){
		//seed random number generator
		//srand(1002);	//same seed everytime for debugging purposes
		srand ( time(NULL) );	//randomly generated seed for actual use
		
		for(int i = 0; i < 2; i++){
			for(int j = 0; j < numSeeds; j++){
				randNum = rand();
				randNum = randNum / (RAND_MAX + 1.0);
				randNum = floor(6*randNum);
				randNumInt = (int)randNum;
						
				curBoard[i][randNumInt] = curBoard[i][randNumInt] + 1;
			}
		}
	}else if(randomize == 2){
		//if numSeeds is a multiple of 6
		if(numSeeds % 6 == 0){
			for(int i = 0; i < 2; i++){
				for(int j = 0; j < 6; j++){
					curBoard[i][j] = numSeeds / 6;
				}
			}
		//otherwise numSeeds is not divisible by 6	
		}else{			
			for(int j = 0; j < 6; j++){
				curBoard[0][j] = (numSeeds - numSeeds % 6) / 6;
				curBoard[1][j] = (numSeeds - numSeeds % 6) / 6;
			}
			for(int j = 0; j < numSeeds % 6; j++){
				curBoard[0][j] = curBoard[0][j]+1;
				curBoard[1][j] = curBoard[1][j]+1;
			}
		}
	}else{
		//nothing right now
	}			
	
}

//input is the player who is making the move (1 or 2) and the house that player is sowing their seeds from
//an int is returned for which player moves next (1 or 2) or 0 if the game is over. -1 is returned if there was an error (move could not be completed)
int Board::move(int player, int house, int& resultFlag, int& captureAmount)
{
	int result = 0;
	//convert variables to corresponding indices
	player = player-1;
	house = house-1;
	//keeps track of number of seeds left to be placed. Intially total number of seeds in house being moved
	int seedsInHand = seedsInHand  = curBoard[player][house];
	//keeps track of current house and side of board (which player) when placing seeds
	int curHouse = house;
	int curPlayer = player;
	int otherPlayer;
	int seedsLeft = 0;	//used to check if game is over. sum of seeds on players side who just went. if 0, game is over
	
	
	//set to 1 if current player gets an extra turn. 2 if a capture occured. 0 otherwise
	resultFlag = 0;
	//stores the amount of seeds captured if a capture move takes place
	captureAmount = 0;
	
	//if move cannot be completed, return error code
	if(seedsInHand == 0){
		result = -1;
		
	//otherwise complete move
	}else{
		curBoard[curPlayer][curHouse] = 0;
		
		//while there are still seeds left to place
		while(seedsInHand > 0){
			//if we have not reached players store, continue
			if(curHouse < 5){
				curHouse++;
				curBoard[curPlayer][curHouse] = curBoard[curPlayer][curHouse]+1;
				seedsInHand--;
			//we have reached a store
			}else{
				curHouse = -1;
				//drop seed in players store who made move
				if(curPlayer == player){
					seedsInHand--;
					curBoard[curPlayer][6] = curBoard[curPlayer][6]+1;
				}
				//switch sides of board
				if(curPlayer == 0){
					curPlayer = 1;
				}else{
					curPlayer = 0;
				}
			}
		}
		
		//check where last seed was placed
		if(curHouse < 0){
			//player dropped last seed in their own store, they get an extra turn
			result = player+1;
			resultFlag = 1;
			
			//reset current player for determining if game is over
			if(curPlayer == 0){
				curPlayer = 1;
			}else{
				curPlayer = 0;
			}
		}else{
			//other players turn now
			if(player == 0){
				result = 2;
			}else{
				result = 1;
			}
			
			//check if last seed was placed in a previously empty house on players side
			if(curPlayer == player){
				if(curBoard[curPlayer][curHouse] == 1){
					
					if(curPlayer == 0){
						otherPlayer = 1;
					}else{
						otherPlayer = 0;
					}
						
					if(curBoard[otherPlayer][5-curHouse] > 0){
						resultFlag = 2;
						captureAmount = curBoard[otherPlayer][5-curHouse] + 1;
						
						curBoard[curPlayer][6] = curBoard[curPlayer][6] + curBoard[otherPlayer][5-curHouse] + 1;
						curBoard[otherPlayer][5-curHouse] = 0;
						curBoard[curPlayer][curHouse] = 0;
					}
				}
			}
		}
		
		//check if game is over
		for(int i = 0; i < 6; i++){
			seedsLeft = seedsLeft + curBoard[curPlayer][i];
		}
		if(seedsLeft == 0){
			//game over. Add up remaining seeds for other player
			if(curPlayer == 0){
				curPlayer = 1;
			}else{
				curPlayer = 0;
			}
			
			seedsLeft = 0;
			for(int i = 0; i < 6; i++){
				seedsLeft = seedsLeft + curBoard[curPlayer][i];
			}
			curBoard[curPlayer][6] = curBoard[curPlayer][6] + seedsLeft;
			
			//currently leave these seeds where they are instead of moving them into current players store. Should not matter because game is over
			
			result = 0;
			
			//check if other player doesnt have any seeds left
		}else{
			seedsLeft = 0;
			if(curPlayer == 0){
				curPlayer = 1;
			}else{
				curPlayer = 0;
			}
			
			for(int i = 0; i < 6; i++){
				seedsLeft = seedsLeft + curBoard[curPlayer][i];
			}
			if(seedsLeft == 0){
				//game over. Add up remaining seeds for other player
				if(curPlayer == 0){
					curPlayer = 1;
				}else{
					curPlayer = 0;
				}
				
				seedsLeft = 0;
				for(int i = 0; i < 6; i++){
					seedsLeft = seedsLeft + curBoard[curPlayer][i];
				}
				curBoard[curPlayer][6] = curBoard[curPlayer][6] + seedsLeft;
				
				//currently leave these seeds where they are instead of moving them into current players store. Should not matter because game is over
				
				result = 0;
			}
		}	
	}
	
	return result;	
	
	
}



//returns the current state of the board
int** Board::getBoard()
{
	return curBoard;
}


//sets curBoard to a certain board provided as input. Used by the AI
void Board::setBoard(int **board)
{
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 7; j++){
			curBoard[i][j] = board[i][j];
		}
	}	
}

