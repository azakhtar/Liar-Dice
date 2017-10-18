#ifndef ROLL_H
#define ROLL_H

class roll{

public: roll(int totalDice);
		int* getDiceRoll();
				
private: void rollDice(); 
		 void setDice(int playersDice);
		 int totalDice;
		 int rollResult[5];
};

#endif // ROLL_H
