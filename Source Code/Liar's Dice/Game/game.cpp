#include <iostream>
#include "game.h"
#include "player.h"
#include "roll.h"

using namespace std;

/* This class creates a human player. */
game::game(player::PLAYERTYPE player1, player::PLAYERTYPE player2){
	game::setPlayers(player1, player2);
	game::startGame();
}

/* This function initializes player data. */
void game::setPlayers(player::PLAYERTYPE p1, player::PLAYERTYPE p2){
	player1.playerType = p1;
	player1.assignment = ROLLER;
	player1.playerCall = NA;
	player1.roundStatus = NONE;
	player1.dice = 5;

	player2.playerType = p2;
	player2.assignment = CALLER;
	player2.playerCall = NA;
	player2.roundStatus = NONE;
	player2.dice = 5;
}

/* This function initializes Dice to 5 for a new game
 * and subtracts 1 Die for every loss. */
void game::updateDice(){
	if ( player1.roundStatus == LOST ){
		player1.dice = player1.dice - 1;
	}
	else if ( player2.roundStatus == LOST ) {
		player2.dice = player2.dice - 1;
	}
}

/* This function sets the roller/caller for each game/round. */
void game::updateGameAssignments(){
	if ( player1.roundStatus == WON ){
		player1.assignment = ROLLER;
		player2.assignment = CALLER;
	}
	else if ( player2.roundStatus == WON ){
		player2.assignment = ROLLER;
		player1.assignment = CALLER;
	}

	if ( player1.playerCall == CALL && player2.playerCall == ACCEPT ){
		player2.assignment = ROLLER;
		player1.assignment = CALLER;
	}
	else if ( player2.playerCall == CALL && player1.playerCall == ACCEPT ){
		player1.assignment = ROLLER;
		player2.assignment = CALLER;
	}
}

/* This function gets the result of rolling dice &
 * copies those results into the diceRoll array. */
void game::rollDice(){
	int* result;
	int maxIdx = 0;

	if ( player1.assignment == ROLLER ){
		roll rollDice(player1.dice);
		result = rollDice.getDiceRoll();
		maxIdx = player1.dice;
	}
	else if ( player2.assignment == ROLLER ){
		roll rollDice(player2.dice);
		result = rollDice.getDiceRoll();
		maxIdx = player2.dice;
	}

	for(int idx = 0; idx < maxIdx; idx++){
		diceRoll[idx] = *(result + idx);
	}
}

/* This function returns -1 if game is over. */
int game::isGameOver(){
	if ( player1.dice == 0){
		winner = player2.playerType;
		loser = player1.playerType;
		return -1;
	}
	else if ( player2.dice == 0 ){
		winner = player1.playerType;
		loser = player2.playerType;
		return -1;
	}

	return 0;
}

int game::startGame(){
	game::updateDice();
	game::updateGameAssignments();
	gameStatus = game::isGameOver();

	if (gameStatus == -1){
		return gameStatus;
	}

	game::rollDice();

	return 0;
}

/* This function returns the current dice roll. */
int* game::getRoll(){
	return diceRoll;
}

/* This function returns the current roller */
player::PLAYERTYPE game::getRoller(){
	if ( player1.assignment == ROLLER ){
		return player1.playerType;
	}
	else {
		return player2.playerType;
	}

}

/* This function set the player calls for each round */
void game::setPlayerCalls(game::CALLS p1Call, game::CALLS p2Call){
	player1.playerCall = p1Call;
	player2.playerCall = p2Call;
}

/* This function sets the results for each round */
void game::setRoundStatus(game::ROUND_RESULT p1Result, game::ROUND_RESULT p2Result){
	player1.roundStatus = p1Result;
	player2.roundStatus = p2Result;
}

