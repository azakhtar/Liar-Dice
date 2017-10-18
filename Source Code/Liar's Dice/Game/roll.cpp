#include <iostream>
#include <time.h>
#include "roll.h"
#include "game.h"
#include "player.h"

using namespace std;

/*
** This class does the following:
** 1. Set total dice to number of dice player has
** 2. Rolls all the dice and stores the result
*/
roll::roll(int playersDice){
	roll::setDice(playersDice);
	roll::rollDice();
}

/* Set the total number of dice to player's dice*/
void roll::setDice(int playersDice){
	totalDice = playersDice;	
}

/* Roll all players dice and store the results */
void roll::rollDice(){
	srand(time(0));
	for (int idx = 0; idx < 5; idx++){
		if ( idx < totalDice ){
			rollResult[idx] = ( rand() % 6 ) + 1;
		}
		else {
			rollResult[idx] = -1;
		}
#ifdef DEBUG
		cout << "Die" << idx+1 << ":: " << rollResult[idx] << endl;
#endif
	}
}

/* Return the results of dice rolls */
int* roll::getDiceRoll(){
	return rollResult;
}
