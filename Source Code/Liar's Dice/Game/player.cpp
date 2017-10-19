#include <iostream>
#include <algorithm>
#include <cstddef>
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

/*
 * This function organizes the roll and sets it to diceCount.
 * It does this by:
 * 1. Iterate over the dice roll & find its identicals
 * 2. Increments counter for all matches and adds to vector tuple
 * 3. Repeats the process for all roll values except for the
 *    ones that have already been processed.
 */
void player::setRoll(std::vector <int> roll){
	int currDie;
	int prevDie = 0;
	int count;
#ifdef DEBUG
	cout << "ORDERED DICE" << endl;
	int idx = 0;
#endif

	for( size_t i = 0; i < roll.size(); i++ ){
		count = 1;
		currDie = roll[i];

		if ( currDie != prevDie ){
			for( size_t x = i+1; x < roll.size(); x++ ){
				if ( roll[x] == currDie ){
					count++;
				}
			}
			diceCount.push_back( tuple<int, int>(count, currDie) );
			prevDie = currDie;
	#ifdef DEBUG
			cout << get<0>(diceCount[idx]) << " " << get<1>(diceCount[idx]) << "s"  <<endl;
			idx++;
	#endif
		}

		prevDie = currDie;
	}
}

/* This function determines call the appropriate
 * function to make the call for the player. */
std::tuple <int, int> player::getCall(){

	if ( currPlayer == player::PLAYERTYPE::BLUFFER ){
		void blufferCall();
	}
	else if ( currPlayer == player::PLAYERTYPE::PROBABILISTIC ){
		void probableCall();
	}
	else if ( currPlayer == player::PLAYERTYPE::RANDOM ){
		void randomCall();
	}
	//else if ( currPlayer == player::PLAYERTYPE::SMART ){}
	return currCall;
}

/* This function determines call the appropriate
 * function to respond for the player. */
void player::getResponse(){

	if ( currPlayer == player::PLAYERTYPE::BLUFFER ){
		void blufferResponse();
	}
	else if ( currPlayer == player::PLAYERTYPE::PROBABILISTIC ){
		void probableResponse();
	}
	else if ( currPlayer == player::PLAYERTYPE::RANDOM ){
		void randomResponse();
	}
	//else if ( currPlayer == player::PLAYERTYPE::SMART ){}
}

/* This function determines the call for the bluffer. */
void player::blufferCall(){}

/* This function determines the call for probable player. */
void player::probableCall(){

}

void player::randomCall(){}
void player::blufferResponse(){}
void player::probableResponse(){}
void player::randomResponse(){}



