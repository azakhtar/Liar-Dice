#ifndef EMPIRICALDATA_H
#define EMPIRICALDATA_H

#include <vector>
#include <tuple>
#include <map>

class empiricalData{

public: empiricalData();
		void initializeModel();
		void updateBluffModel();
		void printModelValues(int modelNumber);
		//VARIABLES


	    //FUNCTIONS


private: //FUNCTIONS
		 void createOpponentModels();
		 void initializeModelValsToNull(int myDice);

		/* Following model contains information about opponents calling method and the truthfulness of opponents calls
		 * isItOppFirstCall (1-Yes, 0-No)
		 *	 --> numberOfMyDice (int 1-5)
		 *		 --> numberOfOpponentDice (int 1-5)
		 *		 	--> opponentCall (tuple of ints <numberOfDice, diceValue>)
		 *		 		--> truthfulness (tuple of TODO <lieProbability, truthProbability>)
		 */
		std::map <int, std::map <int, std::map <int, std::map <std::tuple <int, int>, std::tuple <int, int>>>>> opponentMakesCall;

		/* Following model contains information about whether or not the opponent calls a bluff on my calls
		 * isItMyFirstCall (1-Yes, 0-No)
		 *	 --> numberOfMyDice (int 1-5)
		 *		 --> numberOfOpponentDice (int 1-5)
		 *		 	--> myCall (tuple of ints <totalNumberOfDice, diceValue>)
		 *		 		--> probabilityOfCallingBluff (tuple of ints <numberOfBluffsCalled, numberOfBluffsNotCalled>)
		 */
		std::map <int, std::map <int, std::map <int, std::map <std::tuple <int, int>, std::tuple <int, int>>>>> opponentCallsBluff;

		std::tuple <int, int> defaultTupleVal = (std::make_tuple(0, 0));
		std::tuple <int, int> defaultCall = (std::make_tuple(0, 0));
		std::tuple <int, int> currCall = (std::make_tuple(0, 0));
		int totalDice = 10;

};

#endif // EMPIRICALDATA_H
