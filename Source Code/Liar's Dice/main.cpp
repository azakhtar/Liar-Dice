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

/* 0: BLUFFER
 * 1: RANDOM
 * 2: PROBABILISTIC
 * 3: SMART
 */
#define PLAYER1 0
#define PLAYER2 1

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
	int turn = 1; //TODO: This assignment will depend on whoever #define PLAYER1 is
	int totalGames = 0;
	int gamesP1Won = 0;
	int gamesP2Won = 0;
	int winner = 0;

	while ( totalGames < 1000 ){
#ifdef DEBUG
		cout << "\nSTARTING NEW GAME\n" << endl;
#endif
		game newGame(player1.getPlayer(), player2.getPlayer());
		currGameOver = 0;
		totalGames++;

		//TODO: Need to try many trials with different turns
		if ( winner == 1 ){
			turn = 1;
		}
		else if ( winner == 2 ) {
			turn = 2;
		}

		while ( currGameOver == 0 ){
			currGameOver = newGame.startGame();

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

			while ( currRoundOver == 0 ){

				if ( turn == 1 ){
					player1.setCall(oldCall);
					currCall = player1.getCall();
				}
				else {
					player2.setCall(oldCall);
					currCall = player2.getCall();
				}

				//If player1 called bluff compare the call
				if ( get<0>(currCall) == -1 ){
					callCompareResult = newGame.compareCall(oldCall);

					//If 1 is returned that means p1 won BLUFF else p2 lost
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

