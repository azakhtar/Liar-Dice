#ifndef PLAYGAME_H
#define PLAYGAME_H

#include <cstdlib>
#include <vector>
#include <tuple>

class playGame{

public: 	playGame(int player1Type, int player2Type, int dicePerPlayer, int playerTurn, int totalGames);

private: 	void startGame(int player1Type, int player2Type, int dicePerPlayer, int playerTurn, int totalGames);

			int player1Dice = -1;
			int player2Dice = -1;
			std::vector <int> player1Roll;
			std::vector <int> player2Roll;
			std::tuple <int, int> currCall;
			std::tuple <int, int> oldCall;
			int currGameOver = -1;
			int currRoundOver = -1;
			int callCompareResult = -1;
			int turn = -1;
			int games = 0;
			int gamesP1Won = 0;
			int gamesP2Won = 0;
			int winner = 0;

};

#endif // PLAYGAME_H
