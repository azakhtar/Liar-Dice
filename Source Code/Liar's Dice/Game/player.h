#ifndef PLAYER_H
#define PLAYER_H

class player{

public: player(int playerType);
	    enum PLAYERTYPE { BLUFFER, RANDOM, PROBABILISTIC, SMART, NONE };
	    player::PLAYERTYPE getPlayer();
	    void setRoll(int* roll);

private: void setPlayer(int playerType);
	     player::PLAYERTYPE currPlayer;
	     int* playerRoll;

};

#endif // PLAYER_H
