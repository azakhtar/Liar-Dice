#include <iostream>
#include <time.h>
#include <algorithm>
#include "game.h"
#include "player.h"

using namespace std;

/* This class creates a human player. */
game::game(player::PLAYERTYPE player1, player::PLAYERTYPE player2, int dicePerPlayer){
	game::setPlayers(player1, player2, dicePerPlayer);
}

/* This function initializes player data. */
void game::setPlayers(player::PLAYERTYPE p1, player::PLAYERTYPE p2, int dicePerPlayer){
	player1.playerType = p1;
	player1.turn = FIRST;
	player1.playerCall = NA;
	player1.roundStatus = NONE;
	player1.dice = dicePerPlayer;

	player2.playerType = p2;
	player2.turn = SECOND;
	player2.playerCall = NA;
	player2.roundStatus = NONE;
	player2.dice = dicePerPlayer;
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

/* This function sets the FIRST/SECOND for each game/round. */
void game::updateGameAssignments(){
	if ( player1.roundStatus == WON ){
		player1.turn = FIRST;
		player2.turn = SECOND;
	}
	else if ( player2.roundStatus == WON ){
		player2.turn = FIRST;
		player1.turn = SECOND;
	}
}

/* Rolls the number of dice passed in, sorts them and returns result */
std::vector <int> game::getDiceRoll(int playersDice){
	std::vector <int> rollResult;

	for ( int idx = 0; idx < playersDice; idx++ ) {
		rollResult.push_back(((rand() % 6) + 1));
	}

	std::sort(rollResult.begin(), rollResult.end());

	return rollResult;
}

/* This function rolls the dice for both players &
 * copies those results into appropriate  arrays. */
void game::rollDice(){
	firstPlayerDiceRoll = game::getDiceRoll(player1.dice);
	secondPlayerDiceRoll = game::getDiceRoll(player2.dice);
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
std::vector <int> game::getRoll(player::PLAYERTYPE player){
	if ( player1.playerType == player ){
		return firstPlayerDiceRoll;
	}
	else if ( player2.playerType == player ){
		return secondPlayerDiceRoll;
	}
	else {
		cout << "ERROR game::getRoll : Roll for player does not exist!" <<endl;
		exit(-1);
	}
}

/* This function returns the player with first turn. */
player::PLAYERTYPE game::getPlayerTurn(){
	if ( player1.turn == FIRST ){
		return player1.playerType;
	}
	else {
		return player2.playerType;
	}
}

/* Returns the dice of the player requested. */
int game::getDice(player::PLAYERTYPE player){
	if ( player1.playerType == player ){
		return player1.dice;
	}
	else if ( player2.playerType == player ){
		return player2.dice;
	}
	else {
		cout << "ERROR game::getRoll : Roll for player does not exist!" <<endl;
		exit(-1);
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

/* This function compares the bluff call against all dice on the table
 * to determine if the player calling the bluff WON (1) or LOST (-1).
 */
int game::compareCall(std::tuple <int, int> callOnTable){
	int count = 0;

	for( size_t i = 0; i < firstPlayerDiceRoll.size(); i++ ){
		if ( get<1>(callOnTable) == firstPlayerDiceRoll[i] ){
			count++;
		}
	}

	for( size_t i = 0; i < secondPlayerDiceRoll.size(); i++ ){
		if ( get<1>(callOnTable) == secondPlayerDiceRoll[i] ){
			count++;
		}
	}

	if ( get<0>(callOnTable) > count ){
		//cout << "FALSE Count: " << count << endl;
		return 1;
	}
	else {
		//cout << "TRUE Count: " << count << endl;
		return -1;
	}
}
//This function returns the winner for the round
int game::getWinner(){
	if ( player1.roundStatus == game::ROUND_RESULT::WON ){
		return 1;
	}
	else{
		return 2;
	}
}
