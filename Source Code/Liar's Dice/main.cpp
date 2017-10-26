#include <iostream>
#include <cstdlib>
#include <vector>
#include <tuple>
#include "Game/player.h"
#include "Game/roll.h"
#include "Game/game.h"

using namespace std;

/* 0: BLUFFER
 * 1: RANDOM
 * 2: PROBABILISTIC
 * 3: SMART
 */
#define PLAYER1 1
#define PLAYER2 0

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
	std::tuple <int, int> player2Call;

	//Start a new game
	game newGame(player1.getPlayer(), player2.getPlayer());

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

	//Get call from player 1
	player1Call = player1.getCall();

	//Set & get call from player 2
	player2.setCall(player1Call);
	player2Call = player2.getCall();


	return 0;
}
