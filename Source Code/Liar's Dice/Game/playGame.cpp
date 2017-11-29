#include <iostream>
#include <algorithm>
#include <cstddef>
#include <cmath>
#include <iomanip>
#include "player.h"
#include "game.h"
#include "playGame.h"

using namespace std;

/* This class creates an object to play liar dice games */
playGame::playGame(int player1Type, int player2Type, int dicePerPlayer, int playerTurn, int totalGames){
	startGame(player1Type, player2Type, dicePerPlayer, playerTurn, totalGames);
}

void playGame::startGame(int player1Type, int player2Type, int dicePerPlayer, int playerTurn, int totalGames){

	player player1(player1Type);
	player player2(player2Type);
	turn = playerTurn;

	while ( games < totalGames ){
#ifdef DEBUG
		cout << "\nSTARTING NEW GAME\n" << endl;
#endif
		game newGame(player1.getPlayer(), player2.getPlayer(), dicePerPlayer);
		currGameOver = 0;
		games++;

		/* Switch who goes first based off winner from previous round */
		if ( winner == 1 ){
			turn = 1;
		}
		else if ( winner == 2 ) {
			turn = 2;
		}

		/* This loop covers one full game. E.g 1 out of 1000 games */
		while ( currGameOver == 0 ){
			currGameOver = newGame.startGame();

			/* If game is not over, get the player dice from games and tell
			 * players how many dice they and their opponents have left */
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

			/* This while loop plays 1 full round until one of the players loses a die */
			while ( currRoundOver == 0 ){

				/* Depending on whose turn it is, that player makes a call
				 * by judging the old call on the table made by the other player */
				if ( turn == 1 ){
					player1.setCall(oldCall);
					currCall = player1.getCall();
				}
				else {
					player2.setCall(oldCall);
					currCall = player2.getCall();
				}

				/* This node is executed if any of the players decides to call BLUFF on other player */
				if ( get<0>(currCall) == -1 ){
					callCompareResult = newGame.compareCall(oldCall);

					//######################################################
					//*****THIS CODE IS FOR AI AGENT MODELLING *************
					/* Following code means if player type is SMART Player and
					 * opponent calls bluff we want to increment the bluff counter. */
					if ( player1Type == 2 and turn != 1 ){
						player1.evaluateBluffModel(1);
					}
					else if ( player2Type == 2 and turn == 1){
						player2.evaluateBluffModel(1);
					}
					//######################################################

					/* If 1 is returned that means player that called BLUFF wins, else other player wins */
					if ( callCompareResult == 1 ){
						if ( turn == 1 ){
							newGame.setRoundStatus(game::ROUND_RESULT::WON, game::ROUND_RESULT::LOST);
#ifdef DEBUG
							cout << "P1 Challenged --> P2 Lost" << endl;
#endif
						}
						else {
							newGame.setRoundStatus(game::ROUND_RESULT::LOST, game::ROUND_RESULT::WON);
#ifdef DEBUG
							cout << "P2 Challenged --> P1 Lost" << endl;
#endif
						}
					}
					else {
						if ( turn == 1 ){
							newGame.setRoundStatus(game::ROUND_RESULT::LOST, game::ROUND_RESULT::WON);
#ifdef DEBUG
							cout << "P1 Challenged --> P1 Lost" << endl;
#endif
						}
						else {
							newGame.setRoundStatus(game::ROUND_RESULT::WON, game::ROUND_RESULT::LOST);
#ifdef DEBUG
							cout << "P2 Challenged --> P2 Lost" << endl;
#endif
						}
					}

					//##############################################################
					//**********THIS CODE IS FOR AI AGENT MODELLING ***************
					/* Following code is executed every time a round finishes when
					 * AI agent is one of the two players. Opponents roll is passed
					 * in to smart agent training data to learn about its opponent */
					if ( player1Type == 2 and turn != 1 ){
						player1.evaluateCallModel( player2Roll );
					}
					else if ( player2Type == 2 and turn == 1){
						player2.evaluateCallModel( player1Roll );
					}
					//######################################################

					/* Set round over to 1 */
					currRoundOver = 1;
					turn = newGame.getWinner();
				}
				/* If no one calls BLUFF then set currCall as oldCall and flip flop the turns
				 * for the other player to call first. E.g if turn=1 that means PLAYER1 just called
				 * so we change turn=2 so PLAYER2 can call next iteration */
				else {

					//######################################################
					//*****THIS CODE IS FOR AI AGENT MODELLING *************
					/* Following code means if player type is SMART Player and
					 * opponent does not call bluff after SMART Player call then
					 * show that value in the player model. */
					if ( player1Type == 2 and turn != 1 ){
						player1.evaluateBluffModel(0);
					}
					else if ( player2Type == 2 and turn == 1){
						player2.evaluateBluffModel(0);
					}
					//######################################################

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

	//TODO: TEMP
	if ( player1Type == 2 ){
		player1.showMapValues(1);
	}

	/* Print out the results for matchup */
	cout << "############################" << endl;
	cout << "Player 1 Won: " << gamesP1Won << " games" << endl;
	cout << "Player 2 Won: " << gamesP2Won << " games" << endl;

}
