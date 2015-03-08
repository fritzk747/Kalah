#ifndef KALAH_H
#define KALAH_H

#include "board.h"
#include "AI.h"
using namespace std;

class Kalah
{
 private:
 	Board *board;
 	AI *ai;
 	int **curBoard;
 	int player1;
 	int player2;
 	int difficulty;
		
 public:
  Kalah();

  void run();
  
  int startGame();
  
  void printBoard();
  
  void printMove(int player, int house, int resultFlag, int captureAmout);
  
  void printComputersTurn(int player);
  
  int promptPlayer(int player);
  
  int endGame();

};

#endif
