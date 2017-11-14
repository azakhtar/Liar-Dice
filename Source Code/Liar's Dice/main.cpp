#include <iostream>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cstddef>
#include <time.h>
#include "Game/player.h"
#include "Game/game.h"

using namespace std;

/* DEFINE PLAYERS */
// 0: BLUFFER
// 1: PROBABILISTIC
// 2: SMART
#define PLAYER1 0
#define PLAYER2 1

/* DEFINE WHICH PLAYER GOES FIRST FOR FIRST GAME */
// TURN = 1 --> FOR PLAYER 1 above
// TURN = 2 --> FOR PLAYER 2 above
#define TURN 1

/* ENTER TOTAL NUMBER OF DICE PER PLAYER */
#define DICEPERPLAYER 5

/* ENTER INT NUMBER FOR TOTAL GAMES */
#define TOTALGAMES 1000

/* ENTER TOTAL ROUNDS OF TOTAL GAMES ABOVE */
//#define ROUNDS 2

int main(){

	srand( time(0) );

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
	std::tuple <int, int> currCall;
	std::tuple <int, int> oldCall;
	int currGameOver;
	int currRoundOver;
	int callCompareResult;
	int turn = TURN;
	int games = 0;
	int gamesP1Won = 0;
	int gamesP2Won = 0;
	int winner = 0;

	while ( games < TOTALGAMES ){
#ifdef DEBUG
		cout << "\nSTARTING NEW GAME\n" << endl;
#endif
		game newGame(player1.getPlayer(), player2.getPlayer(), DICEPERPLAYER);
		currGameOver = 0;
		games++;

		//Switch who goes first based off winner from previous round
		if ( winner == 1 ){
			turn = 1;
		}
		else if ( winner == 2 ) {
			turn = 2;
		}

		//This loop covers one full game
		while ( currGameOver == 0 ){
			currGameOver = newGame.startGame();

			//If game is not over, get the player dice from games and tell players
			//how many dice they and their opponents have left
			if ( currGameOver != -1 ){
				currRoundOver = 0;
				oldCall = (std::make_tuple(0, 0));
				callCompareResult = 0;

				//Get & set dice for both players
				player1Dice = newGame.getDice(player1.getPlayer());
				player2Dice = newGame.getDice(player2.getPlayer());
				player1.setDice(player1Dice, player2Dice);
				player2.setDice(player2Dice, player1Dice);

				//Get & set rolls for both players
				player1Roll = newGame.getRoll(player1.getPlayer());
				player2Roll = newGame.getRoll(player2.getPlayer());
				player1.setRoll(player1Roll);
				player2.setRoll(player2Roll);
			}
			//If game is over, get the winner of the game and increment their total wins
			else {
				winner = newGame.getWinner();
				if ( winner == 1 ){
					gamesP1Won++;
#ifdef DEBUG
					cout << "Player 1 Won the game." << endl;
#endif
				}
				else {
					gamesP2Won++;
#ifdef DEBUG
					cout << "Player 2 Won the game." << endl;
#endif
				}
				break;
			}

			//This while loop plays 1 full round until one of the players loses a die
			while ( currRoundOver == 0 ){

				//Depending on whose turn it is, that player makes a call
				//by judging the old call on the table made by the other player
				if ( turn == 1 ){
					player1.setCall(oldCall);
					currCall = player1.getCall();
				}
				else {
					player2.setCall(oldCall);
					currCall = player2.getCall();
				}

				//This node is executed if any of the players decides to call BLUFF on other player
				if ( get<0>(currCall) == -1 ){
					callCompareResult = newGame.compareCall(oldCall);

					//If 1 is returned that means player that called BLUFF wins, else other player wins
					if ( callCompareResult == 1 ){
						if ( turn == 1 ){
							newGame.setRoundStatus(game::ROUND_RESULT::WON, game::ROUND_RESULT::LOST);
#ifdef DEBUG
							cout << "P1 Challenged: P2 Lost" << endl;
#endif
						}
						else {
							newGame.setRoundStatus(game::ROUND_RESULT::LOST, game::ROUND_RESULT::WON);
#ifdef DEBUG
							cout << "P2 Challenged: P1 Lost" << endl;
#endif
						}
					}
					else {
						if ( turn == 1 ){
							newGame.setRoundStatus(game::ROUND_RESULT::LOST, game::ROUND_RESULT::WON);
#ifdef DEBUG
							cout << "P1 Challenged: P1 Lost" << endl;
#endif
						}
						else {
							newGame.setRoundStatus(game::ROUND_RESULT::WON, game::ROUND_RESULT::LOST);
#ifdef DEBUG
							cout << "P2 Challenged: P2 Lost" << endl;
#endif
						}
					}

					//Set round over to 1
					currRoundOver = 1;
					turn = newGame.getWinner();
				}
				//If no one calls BLUFF then set currCall as oldCall and flip flop the turns
				//for the other player to call first. E.g if turn=1 that means PLAYER1 just called
				//so we change turn=2 so PLAYER2 can call next iteration
				else {
					oldCall = currCall;
					if ( turn == 1 ){
						turn = 2;
					}
					else {
						turn = 1;
					}
				}

			}
		}
	}

	cout << "############################" << endl;
	cout << "Player 1 Won: " << gamesP1Won << " games" << endl;
	cout << "Player 2 Won: " << gamesP2Won << " games" << endl;

	return 0;
}

