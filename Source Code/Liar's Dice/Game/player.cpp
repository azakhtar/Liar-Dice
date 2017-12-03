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

/*****************************************************************************/
//********************PROBABLISTIC AGENT FUNCTION*****************************/
/*****************************************************************************/

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

/*****************************************************************************/
//***********************SMART AGENT FUNCTION*********************************/
/*****************************************************************************/

/* The purpose for this function is to save each of opponents call for
 * model training purposes of the smart player. */
void player::saveOpponentCalls(){
	if ( get<0>(otherPlayerCall) != 0 ){
		savedOppCalls.push_back( otherPlayerCall );
	}
}

/* This function evaluate if opponent is lying and determines if its worth calling a Bluff. */
int player::evaluateOpponentCall(){
	int callLie = 0;

	/* If current player is going second set oppGoesFirst flag to 1. */
	if ( get<0>(currPlayerCall) == 0 ){
		oppGoesFirst = 1;
	}

	truthLieVals = empiricalModel.extractCallModelVal(oppGoesFirst, currPlayerDice, opponentsDice, otherPlayerCall);

#ifdef DEBUG
	cout << "BLUFFER Calls: " << get<0>(otherPlayerCall) << " " << get<1>(otherPlayerCall) << "s"  <<endl;
	cout << "CALL MODEL VAL RETURNED: Truth- " << get<0>(truthLieVals) << " Lie- " << get<1>(truthLieVals) << "s"  <<endl;
#endif

	/* Reset the flag to 0 for rest of the round */
	oppGoesFirst = 0;

	//TODO: Work in progress
	/* If the ration of Truth < 20*Lies then call bluff. */
	if ( get<1>(truthLieVals) > get<0>(truthLieVals) * 20 ){
		int localCount = 0;

		for( size_t i = 0; i < diceCount.size(); i++ ){
			//localCount = 0;
			if ( get<1>(otherPlayerCall) == get<1>(diceCount[i]) ){
				localCount++;
			}
		}

		if ( localCount <= 1 and get<0>(otherPlayerCall) > 1 ){
			currPlayerCall = (std::make_tuple(-1, -1));
			callLie = 1;
		}
	}

	return callLie;
}

/* This function evaluate if opponent will call my bluff and if so it modifies the call. */
void player::evaluateAIAgentCall(){
	/* If current player is going first set smartAgentGoesFirst flag to 1. */
	if ( get<0>(otherPlayerCall) == 0 ){
		smartAgentGoesFirst = 1;
	}

	callBluffVals = empiricalModel.extractBluffModelVal(smartAgentGoesFirst, currPlayerDice, opponentsDice, currPlayerCall);

#ifdef DEBUG
	cout << "PROBABILISIC Calls: " << get<0>(currPlayerCall) << " " << get<1>(currPlayerCall) << "s"  <<endl;
	cout << "BLUFF MODEL VAL RETURNED: Call- " << get<0>(callBluffVals) << " Bluff- " << get<1>(callBluffVals) << "s"  <<endl;
#endif

	/* Evaluate if this is the best call to make or not. If not, update the call. */
	if ( get<1>(callBluffVals) > get<0>(callBluffVals) ){
		int localCount = 0;

		/* Determin how many dies of the call face value curr player actually has.
		 * If the player has less than or equal to the call value then go ahead with the call.
		 * Otherwise go in the else and keep incrementing through the model until a call is reached
		 * where opponent will be more likely to lose depending on if they call or not. */
		for( size_t i = 0; i < diceCount.size(); i++ ){
			localCount = 0;
			if ( get<1>(currPlayerCall) == get<1>(diceCount[i]) ){
				localCount++;
			}
		}

		if ( localCount > get<0>(currPlayerCall) ){
			int betterCallFound = 0;
			int newCallCount = 0;
			int newCallDieFace = 0;
			std::tuple <int, int> newCall = (std::make_tuple(0, 0));

			while ( betterCallFound == 0 ){
				/* Increment count value of call by 1 unless it is 6, then reset to 1
				 * and increment the face value of the call by 1. E.g 3 4s -> 3 5s OR
				 * 6 3s -> 1 4s. */
				if ( get<0>(currPlayerCall) == 6 ){
					newCallCount = 1;
					newCallDieFace = get<1>(currPlayerCall) + 1;
				}
				else {
					newCallCount = get<0>(currPlayerCall) + 1;
					newCallDieFace = get<1>(currPlayerCall);
				}

				/* Create a new call tuple with values set above. */
				newCall = (std::make_tuple(newCallCount, newCallDieFace));

				/* Obtain the Call/Bluff ratio from the Bluff Model for the new call. */
				callBluffVals = empiricalModel.extractBluffModelVal(smartAgentGoesFirst, currPlayerDice, opponentsDice, newCall);

				//TODO:
				if ( get<1>(callBluffVals) < get<0>(callBluffVals) ){
					currPlayerCall = newCall;
					betterCallFound = 1;
				}
			}
		}
	}

	/* Reset the flag to 0 for rest of the round */
	smartAgentGoesFirst = 0;

}

void player::smartCall(){
	int opponentIsLying = 0;

	/* If SMART agent goes first ( which is get<0>(otherPlayerCall)=0 )that means
	 * smartPlayerOpponent is not going first so change that value. */
	if ( get<0>(otherPlayerCall) == 0 and smartPlayerOpponent == 1 ){
		smartPlayerOpponent = 0;
	}

	/* If training is complete determine if opponent is lying or not with their call. */
	if ( trainingComplete == 1 ){
		opponentIsLying = evaluateOpponentCall();
	}

	/* Use the probabilistic model to make a call and save opponents call. */
	if ( opponentIsLying == 0 ){
		probableCall();
		saveOpponentCalls();

		/* If training is complete determine if opponent will call bluff or current call. */
		if ( trainingComplete == 1 ){
			evaluateAIAgentCall();
		}
	}

}

/* Once round is over, this function uses all opponents saved calls in
 * saveOpponentCalls() and compares them to opponents roll to see which calls
 * were bluffs and which were real. Once determined empirical Call model is updated
 * by passing 0 as Truth and 1 as Lie value.*/
void player::evaluateCallModel(std::vector <int> oppRoll){
	int count = 0;
	int dieVal = 0;
	int actualCount = 0;

#ifdef DEBUG
		cout << "###############################" << endl;
#endif

	/* Iterate over all saved opponent calls */
	for( size_t i = 0; i < savedOppCalls.size(); i++ ){
		count = get<0>(savedOppCalls[i]);
		dieVal = get<1>(savedOppCalls[i]);
		actualCount = 0;

		/* Iterate over opponents roll to validate result */
		for( size_t j = 0; j < oppRoll.size(); j++ ){
			if ( oppRoll[j] == dieVal ){
				actualCount++;
			}
		}

#ifdef DEBUG
		cout << "Opp Call: " << count << " " << dieVal << "s. Actual_Count: " << actualCount;
#endif

		/* If opponent is telling truth pass in the call and 0 as TRUTH ELSE pass 1 as Lie. */
		if ( actualCount >= count ){
			empiricalModel.updateCallModel(smartPlayerOpponent, currPlayerDice, opponentsDice, savedOppCalls[i], 0);
#ifdef DEBUG
			cout << " --> KEY " << smartPlayerOpponent << " TRUTH" << endl;
#endif
		}
		else{
			empiricalModel.updateCallModel(smartPlayerOpponent, currPlayerDice, opponentsDice, savedOppCalls[i], 1);
#ifdef DEBUG
			cout << " --> KEY " << smartPlayerOpponent << " LIE" << endl;
#endif
		}

		//Set opponent Key to 0 after first call is checked
		smartPlayerOpponent = 0;
	}

	/* Clear the vector for new game */
	savedOppCalls.clear();

	//Set opponent to 1 again to validate if check in smart agent for new round
	smartPlayerOpponent = 1;
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

/* This function is called if we want to debug and look at all map values.
 * 1-Print Call Model, 2-Print Bluff Model, 3-Print Both Models. */
void player::showMapValues(int mapType){
	if ( mapType == 1 ){
		empiricalModel.printModelValues(1);
	}
	else if ( mapType == 2 ){
		empiricalModel.printModelValues(2);
	}
	else if ( mapType == 3 ){
		empiricalModel.printModelValues(1);
		empiricalModel.printModelValues(2);
	}
}

/* This function updates the trainingComplete flag to value passed in. */
void player::updateTrainingStatus(int flag){
	trainingComplete = flag;
}


