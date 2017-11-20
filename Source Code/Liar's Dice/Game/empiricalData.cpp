#include <iostream>
#include <algorithm>
#include <cstddef>
#include <cmath>
#include <iomanip>
#include "empiricalData.h"
#include "player.h"

using namespace std;

/* This class creates an object for empiricalData by initializing
 * the models for opponent calls and opponent bluffs. */
empiricalData::empiricalData(){
	//createOpponentModels();
}

/* This function takes the values passed in to initialize the two models
 * (opponent calls & opponent bluffs) to tuple values of zeros for 1-5 opponents dice vals.
 */
void empiricalData::initializeModelValsToNull(int myDice){
	for ( int oppDice = 1; oppDice <= 5; oppDice++ ){
		opponentMakesCall[1][myDice][oppDice][defaultCall] = defaultTupleVal;
		opponentMakesCall[0][myDice][oppDice][defaultCall] = defaultTupleVal;

		opponentCallsBluff[1][myDice][oppDice][defaultCall] = defaultTupleVal;
		opponentCallsBluff[0][myDice][oppDice][defaultCall] = defaultTupleVal;
	}
}

/* This function creates values for initializing models. Example
 * isItMyFirstCall(1/2)->numberOfMyDice(1..5)->opponentCallVal(1..10)->opponentCallDieFace(1..6) = calls initializeModelValsToNull>
*/
void empiricalData::createOpponentModels(){
	for ( int dice = 1; dice <= 5; dice++ ){
		for ( int callVal = 1; callVal <= totalDice; callVal++ ){
			for ( int callDice = 1; callDice <= 6; callDice++ ){
				get<0>(defaultCall) = callVal;
				get<1>(defaultCall) = callDice;
				initializeModelValsToNull(dice);
			}
		}
	}
}

/* This function initializes the model */
void empiricalData::initializeModel(){
	createOpponentModels();
}

/* This function updates the bluff model */
void empiricalData::updateBluffModel(){
	//TODO
}

/* This function is only called in DEBUG mode to print the model values
 * If modelNumber == 1: PRINT OUT ALL VALUES for opponentMakesCall MODEL
 * Else: PRINT OUT ALL VALUES for opponentCallsBluff MODEL. */
void empiricalData::printModelValues(int modelNumber){
	for ( int initialKey = 0; initialKey <= 1; initialKey++ ){
		for ( int myDice = 1; myDice <= 5; myDice++ ){
			for ( int oppDice = 1; oppDice <= 5; oppDice++ ){
				cout << "########################" << endl;
				cout << "KEY = " << initialKey << "--> MY DICE = " << myDice << ", OPP DICE =" << oppDice << endl;
				if ( modelNumber == 1 ){
					for(auto it = opponentMakesCall[initialKey][myDice][oppDice].cbegin(); it != opponentMakesCall[initialKey][myDice][oppDice].cend(); ++it){
						std::cout << get<0>(it->first) << " " << get<1>(it->first) << "s : Lie- " << get<0>(it->second) << " Truth-" << get<1>(it->second) << "\n";
					}
				}
				else{
					for(auto it = opponentCallsBluff[initialKey][myDice][oppDice].cbegin(); it != opponentCallsBluff[initialKey][myDice][oppDice].cend(); ++it){
						std::cout << get<0>(it->first) << " " << get<1>(it->first) << "s : Bluff- " << get<0>(it->second) << " Call-" << get<1>(it->second) << "\n";
					}
				}
			}
		}
	}
}
