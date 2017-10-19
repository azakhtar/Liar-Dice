#ifndef ROLL_H
#define ROLL_H

#include <vector>

class roll{

public: roll(int totalDice);
		void rollDice(int playersDice);
		std::vector <int> getDiceRoll();
				
private: std::vector <int> rollResult;
};

#endif // ROLL_H
