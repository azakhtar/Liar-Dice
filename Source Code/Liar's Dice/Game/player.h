#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <tuple>
#include <map>

class player{

public: player(int playerType);

		//ENUMERATIONS
	    enum PLAYERTYPE { BLUFFER, CONSERVATIVE, PROBABILISTIC, SMART, NONE };

	    //FUNCTIONS
	    player::PLAYERTYPE getPlayer();
	    void setDice(int player1Dice, int player2Dice);
	    void setRoll(std::vector <int> roll);
	    std::tuple <int, int> getCall();
	    void setCall(std::tuple <int, int> otherPlayerCall);
	    void getResponse();

private: //FUNCTIONS
	     void setPlayer(int playerType);
	     int  factorial(int n);
	     void setProbability(int unknownDice);
	     void blufferCall();
	     void probableCall();
	     void conservativeCall();
	     void blufferResponse();
	     void probableResponse();
	     void conservativeResponse();


	     //VARIABLES
	 	 double validityThreshold = 0.0;
	     int gameInProcess = 0;
	     int currPlayerDice;
	     int opponentsDice;
	     int validUnknownDiceLimit = 0;
	     player::PLAYERTYPE currPlayer;
	     std::vector <int> playerRoll;
	     std::vector <std::tuple <int, int>> diceCount;
	     std::tuple <int, int> currPlayerCall = (std::make_tuple(0, 0));
	     std::tuple <int, int> otherPlayerCall = (std::make_tuple(0, 0));
	     std::tuple <int, int> defaultBluff = (std::make_tuple(-1, -1));
	     std::map <int, std::vector <double>> diceProbabilities;

};

#endif // PLAYER_H
