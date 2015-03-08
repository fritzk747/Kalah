#ifndef BOARD_H
#define BOARD_H

using namespace std;

class Board
{
 private:
	int **curBoard;
		
 public:
  	Board();
  	
  	Board(int **board);
  
	void startGame(int numSeeds, int randomize);
  
	int move(int player, int house, int& resultFlag, int& captureAmount);

	int** getBoard();

	void setBoard(int **board);
};

#endif
