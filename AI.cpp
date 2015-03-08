#include "ai.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include <iostream>
using namespace std;



//default constructor
AI::AI(){	
	
	
}


int AI::getMove(int **board, int player, int difficulty)
{
	int move = 0;
	
	if(difficulty == 1){
		move = getMoveBeginner(board,player);
	}else if(difficulty == 2){
		move = getMoveIntermediate(board,player);
	}else if(difficulty == 3){
		move = getMoveAdvanced(board,player);
	}else{
		//shouldnt come to this point, but default will be beginner AI
		move = getMoveBeginner(board,player);
	}
	
	return move;
}
  


//randomly selects a house to sow from
int AI::getMoveBeginner(int **board, int player)
{
	
	double randNum = 0;
	int randNumInt = 0;
	
	//srand ( time(NULL) );
	
	while(randNumInt == 0){
		//randomly select which house to sow from
		randNum = rand();
		randNum = randNum / (RAND_MAX + 1.0);
		randNum = floor(6*randNum+1);
		randNumInt = (int)randNum;
		
		//check if valid move
		if(validMove(board, randNumInt, player) == 0){
			//set to 0 so that we find a new number
			randNumInt = 0;
		}
	}
	
	return randNumInt;
	
	
}

//tries to pick a house to sow from where the computer either gets an extra move, captures opposition pieces, or prevents their own pieces from being captured
//looks to score when there are no other options and also looks to hoard pieces towards the end of the game
int AI::getMoveIntermediate(int **board, int player)
{
	int move = 0;
	
	bool hasExtraMove = false;
	bool hasCaptureMove = false;
	bool hasDefensiveMove = false;
	bool hasScoreMove = false;
	bool hasHoardMove = false;
	
	int numExtraMove = 0;
	int maxCaptureMove = 0;
	int maxDefensiveMove = 0;
	int bestScoreMove = 1000000000;
	
	int nearestExtraHouse = 0;
	int maxCaptureHouse = 0;
	int maxDefensiveHouse = 0;
	int bestScoreHouse = 0;
	int bestHoardHouse = 0;
	
	int maxMove = 0;
	
	int *extraMove;
	extraMove = new int[6];
	int *captureMove = new int[6];
	int *defensiveMove = new int[6];
	int *scoreMove = new int[6];
	int *hoardMove = new int[6];
	
	int numSeeds = 0;
	
	player = player-1;
	int otherPlayer;
	if(player == 0){
		otherPlayer = 1;
	}else{
		otherPlayer = 0;
	}
		
	
	for(int i = 0; i < 6; i++){
		extraMove[i] = 0;
		captureMove[i] = 0;
		defensiveMove[i] = 0;
		scoreMove[i] = -1;
		hoardMove[i] = 0;
		
		//extra move
		if(board[player][i] == 6-i){
			extraMove[i] = 1;
			hasExtraMove = true;
			numExtraMove++;
			nearestExtraHouse = i;
		}else{
			extraMove[i] = 0;	
		}
		
		
		//capture move
		if(i + board[player][i] < 6){
			if(board[otherPlayer][5-(i+board[player][i])] > 0 && board[player][i] > 0 && board[player][i+board[player][i]] == 0){
				hasCaptureMove = true;
				
				captureMove[i] = board[otherPlayer][5-(i+board[player][i])] + 1;
				
				if(captureMove[i] > maxCaptureMove){
					maxCaptureMove = captureMove[i];
					maxCaptureHouse = i;
				}
			}else{
				captureMove[i] = 0;
			}
		}else if(board[player][i] == 13){
			hasCaptureMove = true;
				
			captureMove[i] = board[otherPlayer][5 - (i + board[player][i] - 13)] + 2;
		
			if(captureMove[i] > maxCaptureMove){
				maxCaptureMove = captureMove[i];
				maxCaptureHouse = i;
			}
		}else if(i + board[player][i] > 12 && board[player][i] <= 13){
			if(board[player][i + board[player][i] - 13] == 0){
				if(board[otherPlayer][5 - (i + board[player][i] - 13)] > 0){
					hasCaptureMove = true;
					
					captureMove[i] = board[otherPlayer][5 - (i + board[player][i] - 13)] + 2;
				
					if(captureMove[i] > maxCaptureMove){
						maxCaptureMove = captureMove[i];
						maxCaptureHouse = i;
					}
				}else{
					captureMove[i] = 0;
				}
			}else{
				captureMove[i] = 0;
			}
		}else{
			captureMove[i] = 0;
		}
		
		
		//defensive move
		if(i + board[otherPlayer][i] < 6){
			if(board[player][5-(i+board[otherPlayer][i])] > 0 && board[otherPlayer][i] > 0){
				if(board[player][5-(i+board[otherPlayer][i])] > defensiveMove[5-(i+board[otherPlayer][i])]){
					if(board[otherPlayer][i+board[otherPlayer][i]] == 0){
						hasDefensiveMove = true;
						
						defensiveMove[5-(i+board[otherPlayer][i])] = board[player][5-(i+board[otherPlayer][i])];
					
						if(defensiveMove[5-(i+board[otherPlayer][i])] > maxDefensiveMove){
							maxDefensiveMove = 	defensiveMove[5-(i+board[otherPlayer][i])];
							maxDefensiveHouse = 5-(i+board[otherPlayer][i]);
						}
					}
				}
			}
			
			
		}else if(board[otherPlayer][i] == 13){
			if(board[player][5 - (i + board[otherPlayer][i] - 13)] > 0){
				hasDefensiveMove = true;
				
				defensiveMove[5 - (i + board[otherPlayer][i] - 13)] = board[player][5 - (i + board[otherPlayer][i] - 13)];
				
				if(defensiveMove[5 - (i + board[otherPlayer][i] - 13)] > maxDefensiveMove){
					maxDefensiveMove = defensiveMove[5 - (i + board[otherPlayer][i] - 13)];
					maxDefensiveHouse = 5 - (i + board[otherPlayer][i] - 13);
				}
			}
		}else if(i + board[otherPlayer][i] > 12 && board[otherPlayer][i] <= 13){
			if(board[otherPlayer][i + board[otherPlayer][i] - 13] == 0){
				if(board[player][5 - (i + board[otherPlayer][i] - 13)] > 0){
					hasDefensiveMove = true;
					
					defensiveMove[5 - (i + board[otherPlayer][i] - 13)] = board[player][5 - (i + board[otherPlayer][i] - 13)];
					
					if(defensiveMove[5 - (i + board[otherPlayer][i] - 13)] > maxDefensiveMove){
						maxDefensiveMove = defensiveMove[5 - (i + board[otherPlayer][i] - 13)];
						maxDefensiveHouse = 5 - (i + board[otherPlayer][i] - 13);
					}
				}
			}
		}
		
		
		//score move
		if(board[player][i] + i > 5){
			hasScoreMove = true;
			scoreMove[i] = board[player][i] + i - 6;
			if(scoreMove[i] < bestScoreMove){
				bestScoreMove = scoreMove[i];
				bestScoreHouse = i;
			}
		}
			
		
		//hoard move
		if(board[player][i] + i < 6){
			hasHoardMove = true;
			hoardMove[i] = 1;
			bestHoardHouse = i;
		}
		
	}
	
	//get best move(extra move, capture or defensive)
	if(hasExtraMove == true){
		maxMove = numExtraMove;
		move = -1;
	}
	
	if(hasCaptureMove == true){
		if(maxCaptureMove > maxMove){
			maxMove = maxCaptureMove;
			if(move == -1 && nearestExtraHouse > maxCaptureHouse){
				//leave move as taking extra turn
			}else{
				move = -2;
			}
		}
	}
	
	if(hasDefensiveMove == true){
		if(maxDefensiveMove > maxMove){
			maxMove = maxDefensiveMove;
			if(move == -1){
				//leave move as taking extra turn
			}else{
				move = -3;
			}
		}
	}
	
	
	
	//check if we want to hoard seeds (end game strat)
	for(int i = 0; i < 6; i++){
		numSeeds = numSeeds + board[player][i] + board[otherPlayer][i];
	}
	if(numSeeds < 20){
		if(hasHoardMove){
			//if we were going for extra move, hoard instead
			if(move == -1){
				move = -4;
			}else if(move == -2){
				if(hoardMove[maxCaptureHouse] == 1){
					//were good, moves line up, dont do anything
				}else{
					if(maxCaptureMove < 4){
						move = -4;
					}
				}
			}else if(move == -3){
				if(hoardMove[maxDefensiveHouse] == 1){
					//were good, moves line up, dont do anything
				}else{
					if(maxDefensiveMove < 4){
						move = -4;
					}
				}
			}else{
				move = -4;
			}
		}
	}
	
	
	if(move == -1){
		move = nearestExtraHouse+1;
	}else if(move == -2){
		move = maxCaptureHouse+1;
	}else if(move == -3){
		move = maxDefensiveHouse+1;
	}else if(move == -4){
		move = bestHoardHouse+1;
	}else if(move == 0){
		if(hasScoreMove){
			move = bestScoreHouse+1;
		}
	}	
	
	
	//check if we have a valid move. If not, use a random move. Also use a random move if cant decide on another move
	//only way to have an invalid move is if there was a "bug" in the above code. This prevents an error from occuring
	if(validMove(board,move,player+1) == 1){
		return move;
	}else{
		move = getMoveBeginner(board,player+1);
		return move;
	}
	
}



/*
the "advanced" AI searches a game tree for the best possible move it can find

call initial method getMoveAdvanced with board and players turn
returns computers move

recursive method will take board, current players turn, current level, max level and a reference variable for the move that is executed
returns the resulting board

in the recursive method, first call the method again with each possible move for current player
on the return of each of these calls need to determine if this is the best move for current player (this is done with some heuristic b/c do not search entire tree)
after done with calling recursive method for each possible move, should have best board for current player, return that and done with method
*/
int AI::getMoveAdvanced(int **board, int player)
{
	int move = 0;
	//***********************************************************************************************************************************************************
	int maxLevel = 8;	//this variable controls how deep AI searches tree. Currently set at 8 because that is the deepest AI can search without holding up game
	//***********************************************************************************************************************************************************
	int curLevel = 1;
	
	recursiveHelp(board,player,curLevel,maxLevel,move);
	
	//check if we have a valid move. If not, use a random move.
	//only way to have an invalid move is if there was a "bug" in the code. This prevents an error from occuring
	if(validMove(board,move,player) == 1){
		return move;
	}else{
		move = getMoveBeginner(board,player);
		return move;
	}
	
	return move;
}


int** AI::recursiveHelp(int **board, int player, int curLevel, int maxLevel, int& move)
{
	int **bestBoard;
	bestBoard = new int*[2];
	for(int i = 0; i < 2; i++){
		bestBoard[i] = new int[7];
		for(int j = 0; j < 7; j++){
			//initialize to -1 so that the compareBoards method can tell we have not found a "best board" yet
			bestBoard[i][j] = -1;	
		}
	}
	int **curBoard;
	curBoard = new int*[2];
	for(int i = 0; i < 2; i++){
		curBoard[i] = new int[7];
		for(int j = 0; j < 7; j++){
			curBoard[i][j] = 0;
		}
	}
	
	Board *tempBoard = new Board();
	int curPlayer = player-1;		 
	int result = 0;
	int resultFlag = 0;
	int capture = 0;
	int curMove = 0;
	int nextMove = 0;
	
	//base case. If AI has searched the maximum number of levels in game tree, return current game board
	if(curLevel == maxLevel){
		return board;
	//otherwise we have more levels to search. Make each possible move available to current player and then call recursive method
	}else{
		//loop through each house of current player
		for(int i = 0; i < 6; i++){
			tempBoard->setBoard(board);
			//if player has more than one seed in current house, we have an eligible move
			if(board[curPlayer][i] > 0){
				//make a move
				result = tempBoard->move(player,i+1,resultFlag,capture);
				curBoard = tempBoard->getBoard();
				
				//if game is not over (result would be 0 if game is over)
				if(result != 0){
					//need to call recursive method again. However if last move was an extra turn, it is same players turn (this is reflected in "result" variable)
					curBoard = recursiveHelp(curBoard,result,curLevel+1,maxLevel,nextMove);
				}
				//compare best board if we take current move with best board from any previous move. If current board is better, update as follows
				if(compareBoards(curBoard,bestBoard,player) == 1){
					//bestBoard = curBoard;
					move = i+1;
					//cout << "move = " << move << endl;
					for(int j = 0; j < 2; j++){
						for(int k = 0; k < 7; k++){
							bestBoard[j][k] = curBoard[j][k];
						}
					}
				}
			}
		}
		
		return bestBoard;
	}
}


//returns a 1 if player favors board1, 2 if player favors board2, and 0 if they are equal
//since we are not exploring the game tree until terminal nodes, will have to use some heuristic to evaluate how good a board is for the player deciding
//for now using number of seeds in each players store (the best spread in favor of the player deciding is the best board)
int AI::compareBoards(int **board1, int **board2, int player)
{
	//first check to see if board2 has not been set yet.
	//Next check to see if game is over on either board (this includes one player having the majority of the seeds)
	//if game is not over for both boards, need some heuristic to determine best board. For now we will say the difference between the number of seeds
	//in players stores decides. That is, for the player deciding, they want the greatest spread between their store and their opponents. If the opponent
	//is beating the player deciding, then the player deciding wants to be losing by as few seeds as possible
	
	//set to 1 if player favors board1, 2 if player favors board2 and 0 if they are indifferent
	int result = 0;
	
	int totalSeeds = 0;
	int seedsLeftB1P1 = 0;
	int seedsLeftB1P2 = 0;
	int seedsLeftB2P1 = 0;
	int seedsLeftB2P2 = 0;
	
	//set to 1 if board favors player 1, set to 2 if board favors player 2. Set to 0 if board is indifferent
	int board1Result = -1;
	int board2Result = -1;
	
	
	//this means best board has been initialized but we have not found a best board yet. Current board is best by default
	if(board2[0][0] == -1){
		result = 1;
	}else{
		//count number of seeds for each player and the total number of seeds in the game
		for(int i = 0; i < 6; i++){
			totalSeeds = totalSeeds + board1[0][i] + board1[1][i];
			seedsLeftB1P1 = seedsLeftB1P1 + board1[0][i];
			seedsLeftB1P2 = seedsLeftB1P2 + board1[1][i];
			seedsLeftB2P1 = seedsLeftB2P1 + board2[0][i];
			seedsLeftB2P2 = seedsLeftB2P2 + board2[1][i];
		}
		totalSeeds = totalSeeds + board1[0][6] + board1[1][6];
		
		
		
		//if the game is over on board1 (one player doesn't have any seeds left)
		if(seedsLeftB1P1 == 0 || seedsLeftB1P2 == 0){
			//if player 1 has more seeds than player 2, player 1 wins
			if(seedsLeftB1P1 + board1[0][6] > seedsLeftB1P2 + board1[1][6]){
				board1Result = 1;
			//if player 2 has more seeds than player 1, player 2 wins
			}else if(seedsLeftB1P1 + board1[0][6] < seedsLeftB1P2 + board1[1][6]){
				board1Result = 2;
			//if both players have an equal amount of seeds, it is a tie
			}else{
				board1Result = 0;
			}
		//if game is not over, but player 1 already has the majority of the seeds
		}else if(board1[0][6] > totalSeeds / 2){
			board1Result = 1;
		//if game is not over, but player 2 already has the majority of the seeds
		}else if(board1[1][6] > totalSeeds / 2){
			board1Result = 2;
		}
		
		//if the game is over on board2 (one player doesn't have any seeds left)
		if(seedsLeftB2P1 == 0 || seedsLeftB2P2 == 0){
			//if player 1 has more seeds than player 2, player 1 wins
			if(seedsLeftB2P1 + board2[0][6] > seedsLeftB2P2 + board2[1][6]){
				board2Result = 1;
			//if player 2 has more seeds than player 1, player 2 wins
			}else if(seedsLeftB2P1 + board2[0][6] < seedsLeftB2P2 + board2[1][6]){
				board2Result = 2;
			//if both players have an equal amount of seeds, it is a tie
			}else{
				board2Result = 0;
			}
		//if game is not over, but player 1 already has the majority of the seeds
		}else if(board2[0][6] > totalSeeds / 2){
			board2Result = 1;
		//if game is not over, but player 2 already has the majority of the seeds
		}else if(board2[1][6] > totalSeeds / 2){
			board2Result = 2;
		}
		
		
		//if the current best board ends in a win for the current player, then there is no need to update board
		if(board2Result == player){
			result = 2;
		//if the current board ends in a win for the current player, then make this the best board
		}else if(board1Result == player){
			result = 1;
		//if neither board ends in a win for the current player, then need to compare number of seeds in each store
		}else{
			//player 1
			if(player == 1){
				//if board 2 is as good or better for player 1 than board 1, keep current best board (board2)
				if(board2[0][6] - board2[1][6] >= board1[0][6] - board1[1][6]){
					result = 2;
				//otherwise need to switch best board
				}else{
					result = 1;
				}
			//player 2	
			}else{
				//if board 2 is as good or better for player 1 than board 1, keep current best board (board2)
				if(board2[1][6] - board2[0][6] >= board1[1][6] - board1[0][6]){
					result = 2;
				//otherwise need to switch best board
				}else{
					result = 1;
				}
			}
		}
	}	
	
	
	return result;
	
	
	
}


//returns 1 if a valid move, 0 otherwise
int AI::validMove(int **board, int move, int player)
{
	if(move < 1 || move > 6){
		return 0;
	}
	
	if(board[player-1][move-1] > 0){
		return 1;
	}else{
		return 0;
	}
}
