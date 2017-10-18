#include <iostream>
#include "player.h"
#include "game.h"
#include "roll.h"

using namespace std;

/* This class creates a human player */
player::player(int playerType){
	setPlayer(playerType);
}

/* This function sets the playerType. */
void player::setPlayer(int playerType){
	if ( playerType <= 3 ){
		currPlayer = (player::PLAYERTYPE)playerType;
	}
	else {
		cout << "ERROR player::setPlayer : Incorrect player type int passed in" <<endl;
		exit(-1);
	}
}

/* This function returns the playerType. */
player::PLAYERTYPE player::getPlayer(){
	return currPlayer;
}

void player::setRoll(int* roll){
	playerRoll = roll;
#ifdef DEBUG
	cout << "SECOND CHECK" << endl;
	for(int i=0; i<5; i++){
		cout << "Die" << i+1 << ":: " << playerRoll[i] << endl;
	}
#endif
}
