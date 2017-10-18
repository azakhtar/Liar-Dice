#include <iostream>
#include <cstdlib>
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
	printf("DEBUG MODE\n");
#endif
#ifdef RELEASE
	printf("RELEASE MODE\n");
#endif

	player player1(PLAYER1);
	player player2(PLAYER2);

	game newGame(player1.getPlayer(), player2.getPlayer());
	int* result = newGame.getRoll();

	player1.setRoll(result);

	return 0;
}
