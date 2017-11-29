#ifndef EMPIRICALDATA_H
#define EMPIRICALDATA_H

#include <vector>
#include <tuple>
#include <map>

class empiricalData{

public: empiricalData();

		//FUNCTIONS
		void initializeModel();
		void updateBluffModel(int goingFirst, int myDice, int oppDice, std::tuple <int, int> myCall, int bluffCalled);
		void updateCallModel(int goingFirst, int myDice, int oppDice, std::tuple <int, int> oppCall, int trueOrFalseCall);
		void printModelValues(int modelNumber);

		//VARIABLES

private: //FUNCTIONS
		 void createOpponentModels();
		 void initializeModelValsToNull(int myDice);

		/* Following model contains information about opponents calling method and the truthfulness of opponents calls
		 * isItOppFirstCall (1-Yes, 0-No)
		 *	 --> numberOfMyDice (int 1-5)
		 *		 --> numberOfOpponentDice (int 1-5)
		 *		 	--> opponentCall (tuple of ints <numberOfDice, diceValue>)
		 *		 		--> truthfulness (tuple of TODO <truthProbability, lieProbability>)
		 */
		std::map <int, std::map <int, std::map <int, std::map <std::tuple <int, int>, std::tuple <int, int>>>>> opponentMakesCall;

		/* Following model contains information about whether or not the opponent calls a bluff on my calls
		 * isItMyFirstCall (1-Yes, 0-No)
		 *	 --> numberOfMyDice (int 1-5)
		 *		 --> numberOfOpponentDice (int 1-5)
		 *		 	--> myCall (tuple of ints <totalNumberOfDice, diceValue>)
		 *		 		--> probabilityOfCallingBluff (tuple of ints <numberOfBluffsNotCalled, numberOfBluffsCalled>)
		 */
		std::map <int, std::map <int, std::map <int, std::map <std::tuple <int, int>, std::tuple <int, int>>>>> opponentCallsBluff;

		//VARIABLES
		std::tuple <int, int> defaultTupleVal = (std::make_tuple(0, 0));
		std::tuple <int, int> defaultCall = (std::make_tuple(0, 0));
		std::tuple <int, int> currCall = (std::make_tuple(0, 0));
		int totalDice = 10;

};

#endif // EMPIRICALDATA_H
