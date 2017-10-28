#include <iostream>
#include <algorithm>
#include <cstddef>
#include <cmath>
#include <iomanip>
#include "player.h"
#include "game.h"
#include "roll.h"

using namespace std;

/* This class creates a human player */
player::player(int playerType){
	setPlayer(playerType);
	if ( currPlayer == 2 ){
		validityThreshold = 25.0;
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

#ifdef DEBUG
	cout << "PLAYER " << currPlayer << " ROLL" << endl;
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
	else if ( currPlayer == player::PLAYERTYPE::CONSERVATIVE ){
		conservativeCall();
	}
	else if ( currPlayer == player::PLAYERTYPE::PROBABILISTIC ){
		probableCall();
	}
	//else if ( currPlayer == player::PLAYERTYPE::SMART ){}
	return currPlayerCall;
}

/* This function determines call the appropriate
 * function to respond for the player. */
void player::getResponse(){

	if ( currPlayer == player::PLAYERTYPE::BLUFFER ){
		blufferResponse();
	}
	else if ( currPlayer == player::PLAYERTYPE::CONSERVATIVE ){
		conservativeResponse();
	}
	else if ( currPlayer == player::PLAYERTYPE::PROBABILISTIC ){
		probableResponse();
	}
	//else if ( currPlayer == player::PLAYERTYPE::SMART ){}
}

/* This function sets the current call to other players call. */
void player::setCall(std::tuple <int, int> call){
	otherPlayerCall = call;
}

/* This function determines the call for BLUFFER.
 * It does it by:
 * 1. Stores values from last call if its not 1st round of a game
 *    and set them equal to initial values for current call
 * 2. Iterates over current call to find best possible call to make
 * 3. If current Call is of count<=2 & dieValue<=2, players bluffs
 * 4. If best call to make is still less than last call (step 1) it
 *    uses the last call and increments the die value by 1.
 * 5. If player still can not find better call in steps above it
 *    calls a bluff.
 */
void player::blufferCall(){
	int lastCallExists = 0;
	int lastCallCount = 0;
	int lastCallDie = 0;
	int currCallCount = 0;
	int currCallDie = 0;
	int betterCallFound = 0;

	if ( get<0>(otherPlayerCall) != 0 ){
		lastCallExists = 1;
		lastCallCount = get<0>(otherPlayerCall);
		lastCallDie = get<1>(otherPlayerCall);
		currCallCount = lastCallCount;
		currCallDie = lastCallDie;
	}

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

	if ( currCallCount <= 2 && currCallDie <= 2 ){
		currCallCount = 3;
		currCallDie = 3;
	}

	if ( lastCallExists == 1 && betterCallFound == 0 ){
		if ( currCallDie <= lastCallDie && lastCallDie != 6 ){
			currCallCount = lastCallCount;
			currCallDie = lastCallDie + 1;
		}
		else {
			currCallCount = -1;
			currCallDie = -1;
		}
	}

	get<0>(currPlayerCall) = currCallCount;
	get<1>(currPlayerCall) = currCallDie;

#ifdef DEBUG
		cout << "Player " << currPlayer << " Call: " << get<0>(currPlayerCall) << " " << get<1>(currPlayerCall) << "s"  <<endl;
#endif
}

/* This function determines the call for CONSERVATIVE player.
 * It does it by:
 * 1. Stores values from last call if its not 1st round of a game
 *    and set them equal to initial values for current call
 * 2. Iterates over current call to find best possible call to make
 * 3. If best call to make it less than last call (step 1) it uses
 *    the last call and increments the count and die value by 1.
 */
void player::conservativeCall(){
	int lastCallExists = 0;
	int lastCallCount = 0;
	int lastCallDie = 0;
	int currCallCount = 0;
	int currCallDie = 0;

	if ( get<0>(otherPlayerCall) != 0 ){
		lastCallExists = 1;
		lastCallCount = get<0>(otherPlayerCall);
		lastCallDie = get<1>(otherPlayerCall);
		currCallCount = lastCallCount;
		currCallDie = lastCallDie;
	}

	for( size_t i = 0; i < diceCount.size(); i++ ){
		if ( currCallCount < get<0>(diceCount[i]) ){
			currCallCount = get<0>(diceCount[i]);
			currCallDie = get<1>(diceCount[i]);
		}
		else if ( currCallCount == get<0>(diceCount[i]) && currCallDie < get<1>(diceCount[i]) ){
			currCallDie = get<1>(diceCount[i]);
		}
	}

	if ( lastCallExists == 1 ){
		if ( currCallCount <= lastCallCount ){
			currCallCount = lastCallCount + 1;
			currCallDie = lastCallDie + 1;
		}
		else if ( get<1>(currPlayerCall) <= lastCallDie ){
			currCallCount = lastCallCount + 1;
			currCallDie = lastCallDie + 1;
		}
	}

	get<0>(currPlayerCall) = currCallCount;
	get<1>(currPlayerCall) = currCallDie;

#ifdef DEBUG
		cout << "Player " << currPlayer << " Call: " << get<0>(currPlayerCall) << " " << get<1>(currPlayerCall) << "s"  <<endl;
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
	int betterDieAvailable = 0;
	int i = 0;

	if ( get<0>(otherPlayerCall) != 0 ){
		lastCallExists = 1;
		lastCallCount = get<0>(otherPlayerCall);
		lastCallDie = get<1>(otherPlayerCall);
		currCallCount = lastCallCount;
		currCallDie = lastCallDie;
	}

	int diceCountSize = 0;
	for( size_t i = 0; i < diceCount.size(); i++ ){
		diceCountSize++;
	}


	//If opponent made a call
	if ( lastCallExists == 1 ){
		//Loop through current players dice to find matching dice to opponent call
		while ( i >= 0 ){
			if ( lastCallDie == get<1>(diceCount[i]) ){
				trumpedOpponentsCall = 1;
				//Once die is found check to see if opponent is lying or not by comparing
				//(opponentDie < currPlayerDie + thresholdIdx). If opponentDie is greater
				//it means opponent if lying. Rather than calling the bluff, currPlayer will
				//bet a higher face value if available. If not, they will call bluff
				if ( lastCallCount <= (get<0>(diceCount[i]) + validUnknownDiceLimit) ){
					if ( (lastCallCount + 1) <= (get<0>(diceCount[i]) + validUnknownDiceLimit) ){
						currCallCount = lastCallCount + 1;
						currCallDie = lastCallDie;
						callBluff = 0;
						i = -2;
					}
				}
				//Player trying to find a card of higher face value than call
				//If found, the player bets that face value card
				if ( callBluff == 1 ){
					int j = i;
					while ( j >= 0 ) {
						if ( currCallDie < get<1>(diceCount[j]) ){
							currCallCount = get<0>(diceCount[j]);
							currCallDie = get<1>(diceCount[j]);
							i = -2;
							j = -2;
						}
						if ( j == diceCountSize-1 ){
							i = -2;
							j = -2;
						}
						j++;
					}
					//If a higher face value card is not found, the player
					//call a bluff on its opponoent
					if ( currCallDie == lastCallDie ){
						currCallCount = -1;
						currCallDie = -1;
						i = -2;
					}
				}
			}
			if ( i == diceCountSize-1 ){
				i = -2;
			}
			i++;
		}
	}

	//This is executed if currPlayer is going first or if a better
	//call was not found in nested loops above
	if ( trumpedOpponentsCall == 0 ){
		i = 0;
		//Loop through currPlayers dice and stop at first
		//dice better than player call and call that dice
		while ( i >= 0 ){
			if ( currCallDie < get<1>(diceCount[i]) ){
				currCallCount = get<0>(diceCount[i]);
				currCallDie = get<1>(diceCount[i]);
				betterDieAvailable = 1;
				i = -2;
			}
			if ( i == diceCountSize-1 ){
				i = -2;
			}
			i++;
		}
		//If no better dice was found then call BLUFF
		if ( betterDieAvailable == 0 ){
			currCallCount = -1;
			currCallDie = -1;
		}
	}

	get<0>(currPlayerCall) = currCallCount;
	get<1>(currPlayerCall) = currCallDie;

#ifdef DEBUG
		cout << "Player " << currPlayer << " Call: " << get<0>(currPlayerCall) << " " << get<1>(currPlayerCall) << "s"  <<endl;
#endif
}

void player::blufferResponse(){}
void player::conservativeResponse(){}
void player::probableResponse(){}



