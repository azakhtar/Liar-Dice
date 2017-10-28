#include <iostream>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cstddef>
#include "Game/player.h"
#include "Game/roll.h"
#include "Game/game.h"

using namespace std;

/* 0: BLUFFER
 * 1: RANDOM
 * 2: PROBABILISTIC
 * 3: SMART
 */
#define PLAYER1 0
#define PLAYER2 2

int main(){

#ifdef DEBUG
	cout << "DEBUG MODE" << endl;
#else
	cout << "RELEASE MODE" << endl;
#endif

	player player1(PLAYER1);
	player player2(PLAYER2);
	int player1Dice;
	int player2Dice;
	std::vector <int> player1Roll;
	std::vector <int> player2Roll;
	std::tuple <int, int> player1Call;
	std::tuple <int, int> player2Call = (std::make_tuple(0, 0));
	int gameOver = 0;
	int roundOver = 0;
	int firstTimeInLoop = 1;
	int callCompareResult = 0;

	//Start a new game
	game newGame(player1.getPlayer(), player2.getPlayer());

	while ( gameOver == 0 ){
		//Set the call compare result to 0
		callCompareResult = 0;

		//Call for players to roll the dice again if a game is over
		//If any of the players run out of all 5 of their dice -1
		//value is returned which means game is over
		if ( roundOver == 1 ){
			gameOver = newGame.startGame();
			roundOver = 0;
			firstTimeInLoop = 1;
			//TODO:: TEMP because need to fix who goes first
			player2Call = (std::make_tuple(0, 0));
		}

		if ( firstTimeInLoop == 1 ){
			//Get dice for both players
			player1Dice = newGame.getDice(player1.getPlayer());
			player2Dice = newGame.getDice(player2.getPlayer());

			//Set dice for both players
			player1.setDice(player1Dice, player2Dice);
			player2.setDice(player2Dice, player1Dice);

			//Get rolls for both players
			player1Roll = newGame.getRoll(player1.getPlayer());
			player2Roll = newGame.getRoll(player2.getPlayer());

			//Set rolls for both players
			player1.setRoll(player1Roll);
			player2.setRoll(player2Roll);
		}

		//TODO:: Player 1 will not always go first because
		//TODO:: player who won last round will
		//Everytime after first round set p2 call for p1
		//if ( firstTimeInLoop == 0 ){
			player1.setCall(player2Call);
		//}

		//Get call from player 1
		player1Call = player1.getCall();

		//If player1 called bluff compare the call
		if ( get<0>(player1Call) == -1 ){
			callCompareResult = newGame.compareCall(player2Call);

			//If 1 is returned that means p1 won BLUFF else p2 lost
			if ( callCompareResult == 1 ){
				newGame.setRoundStatus(game::ROUND_RESULT::WON, game::ROUND_RESULT::LOST);
				cout << "P1 Challenged: P2 Lost" << endl;
			}
			else {
				newGame.setRoundStatus(game::ROUND_RESULT::LOST, game::ROUND_RESULT::WON);
				cout << "P1 Challenged: P1 Lost" << endl;
			}

			//Set round over to 1
			roundOver = 1;
		}

		if ( callCompareResult == 0 ){
			//Set & get call from player 2
			player2.setCall(player1Call);
			player2Call = player2.getCall();
		}

		//If player2 called bluff compare the call
		if ( get<0>(player2Call) == -1 ){
			callCompareResult = newGame.compareCall(player1Call);

			//If 1 is returned that means p2 won BLUFF else p2 lost
			if ( callCompareResult == 1 ){
				newGame.setRoundStatus(game::ROUND_RESULT::LOST, game::ROUND_RESULT::WON);
				cout << "P2 Challenged: P1 Lost" << endl;
			}
			else {
				newGame.setRoundStatus(game::ROUND_RESULT::WON, game::ROUND_RESULT::LOST);
				cout << "P2 Challenged: P2 Lost" << endl;
			}

			//Set round over to 1
			roundOver = 1;
		}

		firstTimeInLoop = 0;
	}

	return 0;
}

