#include <iostream>
#include <algorithm>
#include <cstddef>
#include <cmath>
#include <iomanip>
#include "player.h"
#include "game.h"
#include "empiricalData.h"

using namespace std;

/* This class creates a human player */
player::player(int playerType){
	setPlayer(playerType);

	/* If player is not BLUFFER then set Validity Threshold. */
	if ( currPlayer != player::PLAYERTYPE::BLUFFER ){
		validityThreshold = 25.0;
		/* If player is AI Agent then initialize the empirical model. */
		if ( currPlayer == player::PLAYERTYPE::SMART ){
			empiricalModel.initializeModel();
		}
	}
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

/*
 * This function sets the probabilities of
 * instances of a dice depending on the unknown
 * dice available.
 */
void player::setProbability(int unknownDice){
	int diceFactorial;
	int instanceFactorial;
	int diceMinusInstanceFactorial;
	double probability;

#ifdef DEBUG
	cout << "         DICE PROBABILITY TABLE        " << endl;
	cout << "########################################" << endl;
	cout << "n    0     1       2      3     4     5 " << endl;
#endif
	for ( int n = unknownDice; n > 0; n-- ){
		diceFactorial = factorial(n);
#ifdef DEBUG
		cout << n;
		std::cout << std::setprecision(2) << std::fixed;
#endif

		for ( int k = 0; k <= n; k++ ){
			instanceFactorial = factorial(k);
			diceMinusInstanceFactorial = factorial(n-k);
			probability = (diceFactorial/((instanceFactorial)*(diceMinusInstanceFactorial)))*(pow(1.0/6.0, double(k)))*(pow(5.0/6.0, double(n-k)))*100;
			diceProbabilities[n].push_back(probability);
#ifdef DEBUG
			cout << "  " << diceProbabilities[n].at(k);
#endif
		}
#ifdef DEBUG
		cout << endl;
#endif
	}

}

/* This function returns factorial for a value. */
int player::factorial(int n)
{
    if(n > 1)
        return n * factorial(n - 1);
    else
        return 1;
}

/* This function returns the playerType. */
player::PLAYERTYPE player::getPlayer(){
	return currPlayer;
}

/*
 * This function sets the dice of the both players.
 * Call the setProbability function to compute
 * probablities of unknown dice and computes the
 * max valid unknown index for the player type.
 * */
void player::setDice(int player1Dice, int player2Dice){
	currPlayerDice = player1Dice;
	opponentsDice = player2Dice;

	if ( currPlayer != player::PLAYERTYPE::BLUFFER ){
		if ( gameInProcess == 0 ){
			player::setProbability(opponentsDice);
			gameInProcess = 1;
		}

		for ( int i = 1; i <= player2Dice; i++ ){
			if ( diceProbabilities[player2Dice].at(i) > validityThreshold ){
				validUnknownDiceLimit = i;
				//TODO: Delete following lines
				/*cout << "Dice Prob is:: " << diceProbabilities[player2Dice].at(i) << endl;
				cout << "Validity Threshold is:: " << validityThreshold << endl;
				cout << "THRESHOLD:::" << validUnknownDiceLimit << endl;*/
			}
		}
	}
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
	diceCount.clear();
	currPlayerCall = (std::make_tuple(0, 0));

#ifdef DEBUG
	cout << "PLAYER " << currPlayer + 1 << " ROLL" << endl;
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
		blufferCall();
	}
	else if ( currPlayer == player::PLAYERTYPE::PROBABILISTIC ){
		probableCall();
	}
	else if ( currPlayer == player::PLAYERTYPE::SMART ){
		smartCall();
	}
	return currPlayerCall;
}

/* This function sets the current call to other players call. */
void player::setCall(std::tuple <int, int> call){
	otherPlayerCall = call;
}

/* This function determines the call for BLUFFER. */
void player::blufferCall(){
	int lastCallExists = 0;
	int lastCallCount = 0;
	int lastCallDie = 0;
	int currCallCount = 0;
	int currCallDie = 0;
	int betterCallFound = 0;

	/* Stores values from last call if its not 1st round of a game
	 * and set them equal to initial values for current call */
	if ( get<0>(otherPlayerCall) != 0 ){
		lastCallExists = 1;
		lastCallCount = get<0>(otherPlayerCall);
		lastCallDie = get<1>(otherPlayerCall);
		currCallCount = lastCallCount;
		currCallDie = lastCallDie;
	}

	/* Iterate over players dice to find better call to make.
	 * Example: Opponent Call = 3 3s
	 * IF:     currPlayer has count of 3 or higher then face value
	 *         for the die associated with that count has to be
	 *         greater than 3. e.g 3 4s, 4 4s is OK. 3 3s is !OK.
	 * ELSEIF: currPlayer has count of 4 or higher then face value
	 * 		   for the die associated with that count can be equal
	 * 		   to 3 or greater than 3. E.g 4 3s or 4 4s. */
	for( size_t i = 0; i < diceCount.size(); i++ ){
		if ( currCallCount <= get<0>(diceCount[i]) && currCallDie < get<1>(diceCount[i]) ){
			currCallCount = get<0>(diceCount[i]);
			currCallDie = get<1>(diceCount[i]);
			betterCallFound = 1;
		}
		else if ( currCallCount < get<0>(diceCount[i]) && currCallDie <= get<1>(diceCount[i]) ){
			currCallCount = get<0>(diceCount[i]);
			currCallDie = get<1>(diceCount[i]);
			betterCallFound = 1;
		}
	}

	/* If current Call is of count<=2 & dieValue<=2, players bluffs */
	if ( currCallCount <= 2 && currCallDie <= 2 ){
		currCallCount = 3;
		currCallDie = 3;
	}

	/* If best call to make is still less than call on table it
	 * uses the last call and increments the die value by 1. */
	if ( lastCallExists == 1 && betterCallFound == 0 ){
		if ( currCallDie <= lastCallDie && lastCallDie != 6 ){
			currCallCount = lastCallCount;
			currCallDie = lastCallDie + 1;
		}
		/* If player still can not find better call in steps above it calls a bluff. */
		else {
			currCallCount = -1;
			currCallDie = -1;
		}
	}

	get<0>(currPlayerCall) = currCallCount;
	get<1>(currPlayerCall) = currCallDie;

#ifdef DEBUG
	if (get<0>(currPlayerCall) != -1 ){
		cout << "BLUFFER Calls: " << get<0>(currPlayerCall) << " " << get<1>(currPlayerCall) << "s"  <<endl;
	}
#endif
}

/* This function determines the call for probable player. */
void player::probableCall(){
	int lastCallExists = 0;
	int lastCallCount = 0;
	int lastCallDie = 0;
	int currCallCount = 0;
	int currCallDie = 0;
	int callBluff = 1;
	int trumpedOpponentsCall = 0;
	int idxForMatch = 0;
	int i = 0;

	/* Stores values from last call if its not 1st round of a game
	 * and set them equal to initial values for current call */
	if ( get<0>(otherPlayerCall) != 0 ){
		lastCallExists = 1;
		lastCallCount = get<0>(otherPlayerCall);
		lastCallDie = get<1>(otherPlayerCall);
		currCallCount = lastCallCount;
		currCallDie = lastCallDie;
	}

	int diceCountSize = 0;

	/* Set the diceCountSize to players roll. Example:
	 * Player Roll: 1 2s, 3 3s, 1 6s.
	 * diceCountSize = 3 (Because player has 3 face values in roll. */
	for( size_t i = 0; i < diceCount.size(); i++ ){
		diceCountSize++;
	}

	/* If opponent made a call */
	if ( lastCallExists == 1 ){
		/* Loop through current players dice to find matching dice to opponent call */
		while ( i >= 0 ){
			if ( lastCallDie == get<1>(diceCount[i]) ){
				trumpedOpponentsCall = 1;
				idxForMatch = i;
				/* Once die is found check to see if opponent is lying or not by comparing
				 * (opponentDie < currPlayerDie + thresholdIdx). If opponent call is within
				 * or equal to defined threshold below than increment count of same face value
				 * dice by one and set callBluff flag to 0 */
				if ( lastCallCount <= (get<0>(diceCount[i]) + validUnknownDiceLimit) ){
					currCallCount = lastCallCount + 1;
					currCallDie = lastCallDie;
					callBluff = 0;
					i = -2;
				}
			}
			if ( i == diceCountSize - 1 ){
				i = -2;
			}
			i++;
		}
	}

	/* This is executed if currPlayer is going first or if the opponent
	 * is lying, i.e, a better call not found in while loop above. */
	if ( callBluff == 1 ){
		/* IF: A matching dice value was found in current player hand then
		 * the idx of iteration is set to the idx of found dice. Example:
		 * Opponent called 3 3s. Current player has 1 2s, 1 3s, 3 5s. In this case
		 * the idx (i) will be set to the idx of 1 3s which is 2. This is so we do
		 * not waste time comparing values of dice < 3 (face 1 & 2) because we already
		 * know we can not call those as each call has to be of higher or same face value.
		 * ELSE: Set i=0 and increment current players hand from the start.
		 */
		if ( trumpedOpponentsCall == 1 ){
			i = idxForMatch;
		}
		else {
			i = 0;
		}

		/* Loop through currPlayers dice and stop at first
		 * dice better than player call and call that dice */
		while ( i >= 0 ){
			if ( currCallDie < get<1>(diceCount[i]) ){
				currCallCount = get<0>(diceCount[i]);
				currCallDie = get<1>(diceCount[i]);
				callBluff = 0;
				i = -2;
			}
			if ( i == diceCountSize - 1 ){
				i = -2;
			}
			i++;
		}
	}

	/* If no better dice was found then call BLUFF */
	if ( callBluff == 1 ){
		currCallCount = -1;
		currCallDie = -1;
	}

	get<0>(currPlayerCall) = currCallCount;
	get<1>(currPlayerCall) = currCallDie;

#ifdef DEBUG
	if (get<0>(currPlayerCall) != -1 ){
		cout << "PROBABLISTIC Calls: " << get<0>(currPlayerCall) << " " << get<1>(currPlayerCall) << "s"  <<endl;
	}
#endif
}

void player::smartCall(){
	probableCall();

}

/* This function call the bluff model function any time opponent calls a bluff. It passes
 * in the call the SMART player made whether or not opponent called a bluff for that call. */
void player::evaluateBluffModel(int bluffCalled){
	/* If current  player has already made a call. */
	if ( get<0>(currPlayerCall) != 0 ){
		/* If current player was first one in round to call and opponent called BLUFF right away. */
		if ( get<0>(otherPlayerCall) == 0 ){
			empiricalModel.updateBluffModel(1, currPlayerDice, opponentsDice, currPlayerCall, bluffCalled);
		}
		else{
			empiricalModel.updateBluffModel(0, currPlayerDice, opponentsDice, currPlayerCall, bluffCalled);
		}
	}
}

/* This function is called if we want to debug and look at all map values */
void player::showMapValues(){
	//int i = 0;
	//empiricalModel.printModelValues(i++);
	empiricalModel.printModelValues(2);
}



