#ifndef GAME_H
#define GAME_H

#include <vector>
#include "player.h"

class game{

public: game(player::PLAYERTYPE player1, player::PLAYERTYPE player2);

		//ENUMERATIONS
		enum ROUND_RESULT { WON, LOST, NONE };
		enum TURN { FIRST, SECOND };
		enum CALLS { CALL, ACCEPT, CHALLENGE, NA };

		//DATA TYPES
		struct playerDetails{
			player::PLAYERTYPE playerType;
			game::TURN turn;
			game::CALLS playerCall;
			game::ROUND_RESULT roundStatus;
			int dice;
		};

		//FUNCTIONS
		int startGame();
		std::vector <int> getRoll(player::PLAYERTYPE player);
		player::PLAYERTYPE getPlayerTurn();
		int getDice(player::PLAYERTYPE player);
		void setPlayerCalls(game::CALLS p1Call, game::CALLS p2Call);
		void setRoundStatus(game::ROUND_RESULT p1Result, game::ROUND_RESULT p2Result);
		int compareCall(std::tuple <int, int> callOnTable);
		int getWinner();

private: //FUNCTIONS
		 void setPlayers(player::PLAYERTYPE p1, player::PLAYERTYPE p2);
		 void updateDice();
		 void updateGameAssignments();
		 std::vector <int> getDiceRoll(int playersDice);
		 void rollDice();
		 int isGameOver();

		 //VARIABLES
		 game::playerDetails player1;
		 game::playerDetails player2;
		 std::vector <int> firstPlayerDiceRoll;
		 std::vector <int> secondPlayerDiceRoll;
		 int gameStatus;
		 player::PLAYERTYPE winner;
		 player::PLAYERTYPE loser;

};

#endif // GAME_H
