#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <tuple>

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
	     void blufferCall();
	     void probableCall();
	     void conservativeCall();
	     void blufferResponse();
	     void probableResponse();
	     void conservativeResponse();

	     //VARIABLES
	     int currPlayerDice;
	     int opponentsDice;
	     player::PLAYERTYPE currPlayer;
	     std::vector <int> playerRoll;
	     std::vector <std::tuple <int, int>> diceCount;
	     std::tuple <int, int> otherPlayerCall = (std::make_tuple(0, 0));

};

#endif // PLAYER_H
