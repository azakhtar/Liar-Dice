#ifndef GAME_H
#define GAME_H

#include "player.h"

class game{

public: game(player::PLAYERTYPE player1, player::PLAYERTYPE player2);

		//ENUMERATIONS
		enum ROUND_RESULT { WON, LOST, NONE };
		enum ASSIGNMENT { ROLLER, CALLER };
		enum CALLS { CALL, ACCEPT, CHALLENGE, NA };

		//DATA TYPES
		struct playerDetails{
			player::PLAYERTYPE playerType;
			game::ASSIGNMENT assignment;
			game::CALLS playerCall;
			game::ROUND_RESULT roundStatus;
			int dice;
		};

		//FUNCTIONS
		int startGame();
		int* getRoll();
		player::PLAYERTYPE getRoller();
		void setPlayerCalls(game::CALLS p1Call, game::CALLS p2Call);
		void setRoundStatus(game::ROUND_RESULT p1Result, game::ROUND_RESULT p2Result);

private: //FUNCTIONS
		 void setPlayers(player::PLAYERTYPE p1, player::PLAYERTYPE p2);
		 void updateDice();
		 void updateGameAssignments();
		 void rollDice();
		 int isGameOver();

		 //VARIABLES
		 game::playerDetails player1;
		 game::playerDetails player2;
		 int diceRoll[5];
		 int gameStatus;
		 player::PLAYERTYPE winner;
		 player::PLAYERTYPE loser;

};

#endif // GAME_H
