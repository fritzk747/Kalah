#include "kalah.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include <iostream>
#include <sstream>
using namespace std;



//default constructor
Kalah::Kalah(){	
	board = new Board();
	ai = new AI();
	
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
	
	//stores 1 if player is human and 2 if it is a computer player
	player1 = 0;
	player2 = 0;
	
	//1 for beginner (computer selects randomly), 2 for intermediate
	int difficulty = 0;
	
}


//"driver" method that controls the action of the program
void Kalah::run()
{
	bool done = false;
	int move = -1;
	int moveResult = 0;
	int resultFlag = 0;	//set to 1 if player earned extra turn, 2 if capture and 0 otherwise
	int captureAmount = 0; //set to amount captured if a capture occured. Doesn't matter otherwise
	int endResult = 0;
	
	//keeps track of next player's turn
	int curPlayer = 0;
	
	//start the game (ask user for game input, initilize board, decide who goes first), print board and prompt first player to make a move
	curPlayer = startGame();
	printBoard();
	if(curPlayer == 1){
		if(player1 == 2){
			printComputersTurn(curPlayer);
		}
	}else{
		if(player2 == 2){
			printComputersTurn(curPlayer);
		}
	}
	
	
	//this loop drives the program. Does not end until a game finishes and user decides to quit
	while(!done){
		int moveResult = -1;
		//loop until user provides a valid move
		while(moveResult == -1){
			move = promptPlayer(curPlayer);
			moveResult = board->move(curPlayer,move, resultFlag, captureAmount);
		}
		
		//display move and current board
		printMove(curPlayer,move,resultFlag,captureAmount);
		printBoard();
		
		//game is over
		if(moveResult == 0){
			//display result of game to user and get their input on whether they want to play again
			endResult = endGame();
			
			//if user does not want to play again, end loop which will then end program
			if(endResult == 0){
				done = true;
			//otherwise user wants to play again, so start a new game
			}else{
				curPlayer = startGame();
				printBoard();
			}
		//if game is not over yet, update current player
		}else{
			curPlayer = moveResult;
			if(curPlayer == 1){
				if(player1 == 2){
					printComputersTurn(curPlayer);
				}
			}else{
				if(player2 == 2){
					printComputersTurn(curPlayer);
				}
			}
		}
	}
				
		
	
}

//prompts the user for instructions on setting up the game
int Kalah::startGame()
{
	double randNum;
	int randNumInt;
	int numHumans = 0;
	int numSeeds;
	string input = "";
	bool done = false;
	
	
	
	cout << "How many seeds do you want to play with (must be between 10 and 40)?" << endl;
	while(!done){
		getline(cin,input);
		numSeeds = atoi(input.c_str());
		
		if(numSeeds >= 10 && numSeeds <= 40){
			done = true;
		}
	}
	
	cout << "How many human players (must be 1 or 2)?" << endl;
	done = false;
	while(!done){
		getline(cin,input);
		numHumans = atoi(input.c_str());
		
		if(numHumans == 1 || numHumans == 2){
			done = true;
		}
	}
	
	if(numHumans == 1){
		cout << "Computer difficulty (1 for beginner, 2 for intermediate or 3 for advanced)?:" << endl;
		done = false;
		while(!done){
			getline(cin,input);
			difficulty = atoi(input.c_str());
			
			if(difficulty == 1 || difficulty == 2 || difficulty == 3){
				done = true;
			}
		}
		
		player1 = 2;
	}else{
		player1 = 1;
	}
	
	player2 = 1;
	
	board->startGame(numSeeds,1);	
	
	//srand ( time(NULL) );
	
	//randomly select which player is to go first
	randNum = rand();
	randNum = randNum / (RAND_MAX + 1.0);
	randNum = floor(2*randNum+1);
	randNumInt = (int)randNum;
	
	cout << endl;
	if (numHumans == 1){
		cout << "Player 1 is the computer. Player 2 is human." << endl;
	}else{
		cout << "Player 1 is human. Player 2 is human." << endl;
	}
	cout << "Player 1 is the top row. Player 2 is the bottom row." <<endl;
	cout << "Player 1's store is on the left side. Player 2's store is on the right side." << endl;
	cout << "Houses are numbered in a counterclockwise manner." << endl;
	cout << "Player 1's house number 1 is the top right of the board and house number 6 is the top left of the board." << endl;
	cout << "Player 2's house number 1 is the bottom left of the board and house number 6 is the bottom right of the board." << endl << endl;
	
	
	cout << "Player " << randNumInt << " has been selected to go first." << endl;
	
	//return which player is to go first
	return randNumInt;
}
  

//prints the current state of the board to the user
void Kalah::printBoard()
{
	curBoard = board->getBoard();
	
	
	cout << endl << "The current state of the board is as follows:" << endl << endl;
	cout << "Player 1 side..." << endl;
	
	for(int i = 0; i < 6; i++){
		cout << "\t" << curBoard[0][5-i];
	}
	cout << endl << endl;
	
	cout << curBoard[0][6] << "\t\t\t\t\t\t\t";
	cout << curBoard[1][6] << endl << endl;
		
	for(int i = 0; i < 6; i++){
		cout << "\t" << curBoard[1][i];
	}
	cout << endl;
	cout << "Player 2 side..." << endl << endl << endl; 
	
}
  

//prints the given move to the user
void Kalah::printMove(int player, int house, int resultFlag, int captureAmount)
{
	cout << "Player " << player << " (";
	if(player == 1){
		if(player1 == 1){
			cout << "human";
		}else{	
			cout << "computer";
		}
	}else{
		if(player2 == 1){
			cout << "human";
		}else{	
			cout << "computer";
		}
	}
	cout << ") moved from house " << house << "." << endl;
	if(resultFlag == 1){
		cout << "Player " << player << " earned an extra move." << endl;
	}else if(resultFlag == 2){
		cout << "Player " << player << " performed a capture. " << captureAmount << " seeds captured in total." << endl;
	}
	
	
}

//lets the user know it is the computers turn and to press "enter" to continue
void Kalah::printComputersTurn(int player)
{
	string input = "";
	cout << "It is the Computer's turn (Player " << player << "). Press enter to continue..." << endl;
	getline(cin,input);
}

 
 
//prompts the user (either person or computer) for their next move and returns that move as an int
int Kalah::promptPlayer(int player)
{
	int move = 0;
	curBoard = board->getBoard();
	string input = "";
	bool done = false;
	
	if(player == 1){
		//if human user prompt for next move
		if(player1 == 1){
			cout << "Player 1, enter a number 1 through 6 for the house to sow seeds from." << endl;
			
			while(!done){
				getline(cin,input);
				move = atoi(input.c_str());
				
				if(move >= 1 && move <= 6){
					done = true;
				}
			}
			cout << endl;
		}else{
			//prompt AI for move
			move = ai->getMove(curBoard,1,difficulty);
		}
	}else{
		//if human user prompt for next move
		if(player2 == 1){
			cout << "Player 2, enter a number 1 through 6 for the house to sow seeds from." << endl;
			
			while(!done){
				getline(cin,input);
				move = atoi(input.c_str());
				
				if(move >= 1 && move <= 6){
					done = true;
				}
			}
			cout << endl;
		}else{
			//prompt AI for move
			move = ai->getMove(curBoard,2,difficulty);
		}
	}
	
	return move;
	
	
}

//displays message to user about the game ending. Asks user if they want to play again
//returns 1 if user wants to play again, 0 otherwise
int Kalah::endGame()
{
	curBoard = board->getBoard();
	int numSeedsP1 = curBoard[0][6];
	int numSeedsP2 = curBoard[1][6];
	char playAgain[1];
	string input = "";
	bool done = false;
	
	cout << "The game has ended. Player 1 finished with " << numSeedsP1 << " seeds. Player 2 finished with " << numSeedsP2 << "." << endl;
	
	if(numSeedsP1 > numSeedsP2){
		cout << "Player 1 is the winner!" << endl;
	}else if(numSeedsP2 > numSeedsP1){
		cout << "Player 2 is the winner!" << endl;
	}else{
		cout << "The game has ended in a tie!" << endl;
	}
	
	
	//prompt the user if they want to play again
	cout << endl << "Would you like to play again(y/n)?" << endl;
	while(!done){
		getline(cin,input);
		
		if((input.compare("y") == 0) || (input.compare("n") == 0)){
			done = true;
		}
	}
	
	
	if(input.compare("y") == 0){
		cout << endl;
		return 1;
	}else{
		return 0;
	}
	
	
}