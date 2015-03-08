#ifndef AI_H
#define AI_H

#include "board.h"
using namespace std;

class AI
{
 private:
  
		
 public:
  AI();

  int getMove(int **board, int player, int difficulty);
  
  int getMoveBeginner(int **board, int player);
  
  int getMoveIntermediate(int **board, int player);
  
  int getMoveAdvanced(int **board, int player);
  
  int** recursiveHelp(int **board, int player, int curLevel, int maxLevel, int& move);
  
  int compareBoards(int **board1, int **board2, int player);
  
  int validMove(int **board, int move, int player);

};

#endif
