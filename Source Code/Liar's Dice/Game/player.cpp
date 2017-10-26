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

	cout << "####################################" << endl;
	cout << "n    0     1     2     3     4     5" << endl;
	for ( int n = unknownDice; n > 0; n-- ){
		diceFactorial = factorial(n);
		cout << n;
		std::cout << std::setprecision(2) << std::fixed;

		for ( int k = 0; k <= n; k++ ){
			instanceFactorial = factorial(k);
			diceMinusInstanceFactorial = factorial(n-k);
			probability = (diceFactorial/((instanceFactorial)*(diceMinusInstanceFactorial)))*(pow(1.0/6.0, double(k)))*(pow(5.0/6.0, double(n-k)))*100;
			diceProbabilities[n].push_back(probability);

			cout << "  " << diceProbabilities[n].at(k);
		}
		cout << endl;
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

/* This function sets the dice of the both players. */
void player::setDice(int player1Dice, int player2Dice){
	currPlayerDice = player1Dice;
	opponentsDice = player2Dice;

	if ( gameInProcess == 0 ){
		player::setProbability(opponentsDice);
		gameInProcess = 1;
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
	return otherPlayerCall;
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
 * 2. Iterates over current call to find best possible call to make
 * 3. If current Call is of count<=2 & dieValue<=2, players bluffs
 * 4. If best call to make is still less than last call (step 1) it
 *    uses the last call and increments the count and die value by 1.
 */
void player::blufferCall(){
	int lastCallExists = 0;
	int lastCallCount = 0;
	int lastCallDie = 0;

	if ( get<0>(otherPlayerCall) != 0 ){
		lastCallExists = 1;
		lastCallCount = get<0>(otherPlayerCall);
		lastCallDie = get<1>(otherPlayerCall);
	}

	for( size_t i = 0; i < diceCount.size(); i++ ){
		if ( get<0>(otherPlayerCall) < get<0>(diceCount[i]) ){
			get<0>(otherPlayerCall) = get<0>(diceCount[i]);
			get<1>(otherPlayerCall) = get<1>(diceCount[i]);
		}
		else if ( get<0>(otherPlayerCall) == get<0>(diceCount[i]) && get<1>(otherPlayerCall) < get<1>(diceCount[i]) ){
			get<1>(otherPlayerCall) = get<1>(diceCount[i]);
		}
	}

	if ( get<0>(otherPlayerCall) <= 2 && get<1>(otherPlayerCall) <= 2 ){
		get<0>(otherPlayerCall) = 3;
		get<1>(otherPlayerCall) = 3;
	}

	if ( lastCallExists == 1 ){
		if ( get<0>(otherPlayerCall) <= lastCallCount ){
			get<0>(otherPlayerCall) = lastCallCount + 1;
			get<1>(otherPlayerCall) = lastCallDie + 1;
		}
		else if ( get<1>(otherPlayerCall) <= lastCallDie ){
			get<0>(otherPlayerCall) = lastCallCount + 1;
			get<1>(otherPlayerCall) = lastCallDie + 1;
		}
	}

#ifdef DEBUG
		cout << "Player " << currPlayer << " Call: " << get<0>(otherPlayerCall) << " " << get<1>(otherPlayerCall) << "s"  <<endl;
#endif
}

/* This function determines the call for CONSERVATIVE player.
 * It does it by:
 * 1. Stores values from last call if its not 1st round of a game
 * 2. Iterates over current call to find best possible call to make
 * 3. If best call to make it less than last call (step 1) it uses
 *    the last call and increments the count and die value by 1.
 */
void player::conservativeCall(){
	int lastCallExists = 0;
	int lastCallCount = 0;
	int lastCallDie = 0;

	if ( get<0>(otherPlayerCall) != 0 ){
		lastCallExists = 1;
		lastCallCount = get<0>(otherPlayerCall);
		lastCallDie = get<1>(otherPlayerCall);
	}

	for( size_t i = 0; i < diceCount.size(); i++ ){
		if ( get<0>(otherPlayerCall) < get<0>(diceCount[i]) ){
			get<0>(otherPlayerCall) = get<0>(diceCount[i]);
			get<1>(otherPlayerCall) = get<1>(diceCount[i]);
		}
		else if ( get<0>(otherPlayerCall) == get<0>(diceCount[i]) && get<1>(otherPlayerCall) < get<1>(diceCount[i]) ){
			get<1>(otherPlayerCall) = get<1>(diceCount[i]);
		}
	}

	if ( lastCallExists == 1 ){
		if ( get<0>(otherPlayerCall) <= lastCallCount ){
			get<0>(otherPlayerCall) = lastCallCount + 1;
			get<1>(otherPlayerCall) = lastCallDie + 1;
		}
		else if ( get<1>(otherPlayerCall) <= lastCallDie ){
			get<0>(otherPlayerCall) = lastCallCount + 1;
			get<1>(otherPlayerCall) = lastCallDie + 1;
		}
	}

#ifdef DEBUG
		cout << "Player " << currPlayer << " Call: " << get<0>(otherPlayerCall) << " " << get<1>(otherPlayerCall) << "s"  <<endl;
#endif
}

/* This function determines the call for probable player. */
void player::probableCall(){}
void player::blufferResponse(){}
void player::conservativeResponse(){}
void player::probableResponse(){}



