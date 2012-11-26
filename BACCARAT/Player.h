#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL/SDL.h>
#include <string>
using std::string;

class Player {
public:
	Player();
	Player(string,double,double);
	string getName();
	double getCash();
	void setCash(double);
	void setBet(double);
	double getBet();

	void setName(const string&);
	virtual ~Player();
private:
	string playerName;
	double cash;
	double bet;
	int playerZone;
	int bankerZone;
	int tieZone;
};

#endif
