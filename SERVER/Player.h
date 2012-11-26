#ifndef PLAYER_H_
#define PLAYER_H_

#include "SDL/SDL.h"
#include "SDL/SDL_net.h"
#include <string>

#include "CardDeal.h"
using std::string;

class Player {
public:
	Player();
	Player(string, double, double);
	string getName();
	double getCash();
	void setCash(double);
	void setBet(double);
	double getBet();

	void setName(const string&);
	virtual ~Player();

	TCPsocket socket;
	IPaddress address;

	int playerZone;
	int bankerZone;
	int tieZone;

	bool dealed;
	int playerCards;

	Card cards[3];

private:
	string playerName;
	double cash;
	double bet;
};

#endif
