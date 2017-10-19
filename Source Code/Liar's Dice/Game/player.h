#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <tuple>

class player{

public: player(int playerType);

		//ENUMERATIONS
	    enum PLAYERTYPE { BLUFFER, RANDOM, PROBABILISTIC, SMART, NONE };

	    //FUNCTIONS
	    player::PLAYERTYPE getPlayer();
	    void setRoll(std::vector <int> roll);
	    std::tuple <int, int> getCall();
	    void getResponse();

private: //FUNCTIONS
	     void setPlayer(int playerType);
	     void blufferCall();
	     void probableCall();
	     void randomCall();
	     void blufferResponse();
	     void probableResponse();
	     void randomResponse();

	     //VARIABLES
	     player::PLAYERTYPE currPlayer;
	     std::vector <int> playerRoll;
	     std::vector <std::tuple <int, int>> diceCount;
	     std::tuple <int, int> currCall = (std::make_tuple(0, 0));
	     std::tuple <int, int> lastCall = (std::make_tuple(0, 0));

};

#endif // PLAYER_H
