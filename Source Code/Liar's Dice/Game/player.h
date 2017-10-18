#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

class player{

public: player(int playerType);
	    enum PLAYERTYPE { BLUFFER, RANDOM, PROBABILISTIC, SMART, NONE };
	    player::PLAYERTYPE getPlayer();
	    void setRoll(std::vector <int> roll);

private: void setPlayer(int playerType);
	     player::PLAYERTYPE currPlayer;
	     std::vector <int> playerRoll;

};

#endif // PLAYER_H
