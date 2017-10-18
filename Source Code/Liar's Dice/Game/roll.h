#ifndef ROLL_H
#define ROLL_H

#include <vector>

class roll{

public: roll(int totalDice);
		std::vector <int> getDiceRoll();
				
private: void rollDice(); 
		 void setDice(int playersDice);
		 int totalDice;
		 //int rollResult[5];
		 std::vector <int> rollResult;
};

#endif // ROLL_H
