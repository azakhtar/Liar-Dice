#include <iostream>
#include <time.h>
#include <algorithm>
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
	roll::rollDice(playersDice);
	//TODO:: Uncomment this and fix seeding
	//srand( time(0) );
}

/* Roll all players dice and store the results */
void roll::rollDice(int playersDice){
	srand( time(0) + rand() % 100 );
	for ( int idx = 0; idx < playersDice; idx++ ) {
		rollResult.push_back(((rand() % 6) + 1));
/*#ifdef DEBUG
		cout << "Die" << idx+1 << ":: " << rollResult[idx] << endl;
#endif*/
	}

	std::sort(rollResult.begin(), rollResult.end());
}

/* Return the results of dice rolls */
std::vector <int> roll::getDiceRoll(){
	return rollResult;
}
