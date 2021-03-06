#include <iostream>
#include <time.h>
#include "Game/playGame.h"

using namespace std;

/* DEFINE PLAYERS
 * 0: BLUFFER
 * 1: PROBABILISTIC
 * 2: SMART */
#define PLAYER1 2
#define PLAYER2 1

/* DEFINE WHICH PLAYER GOES FIRST FOR FIRST GAME
 * TURN = 1 --> FOR PLAYER 1 above
 * TURN = 2 --> FOR PLAYER 2 above */
#define TURN 1

/* ENTER TOTAL NUMBER OF DICE PER PLAYER */
#define DICEPERPLAYER 5

/* ENTER INT NUMBER FOR TOTAL TRAINGING GAMES */
#define TRAININGGAMES 10000

/* ENTER INT NUMBER FOR TOTAL TRAINGING GAMES */
#define REALGAMES 10000

int main(){
	//Set the seeding to generate random rolls
	srand( time(0) );

#ifdef DEBUG
	cout << "DEBUG MODE" << endl;
#else
	cout << "RELEASE MODE" << endl;
#endif

	//Create playGame object that plays TOTALGAMES between PLAYER1 vs PLAYER2 with DICEPERPLAYER dice per player
	playGame playGame(PLAYER1, PLAYER2, DICEPERPLAYER, TURN, TRAININGGAMES, REALGAMES);

	return 0;
}

