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
#define PLAYER1 0
#define PLAYER2 1

int main(){

#ifdef DEBUG
	cout << "DEBUG MODE" << endl;
#else
	cout << "RELEASE MODE" << endl;
#endif

	player player1(PLAYER1);
	player player2(PLAYER2);

	game newGame(player1.getPlayer(), player2.getPlayer());
	std::vector <int> result = newGame.getRoll();

	player1.setRoll(result);
	player1.getCall();

	std::tuple <int, int> call = player1.getCall();
	cout << "Player Call: " << get<0>(call) << " " << get<1>(call) << "s"  <<endl;

	return 0;
}
