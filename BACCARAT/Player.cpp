#include "Player.h"

Player::Player()
{
	playerName="";
	cash=0.0;
	bet=0.0;
	playerZone = 0;
	bankerZone = 0;
	tieZone = 0;
}

Player::Player(string _playerName, double _cash, double _bet)
{
	playerName = _playerName;
	cash = _cash;
	bet = _bet;
}

void Player::setBet(double tmpBet)
{
	bet = tmpBet;
}

double Player::getBet()
{
	return bet;
}

double Player::getCash()
{
	return cash;
}
void Player::setCash(double curr){
	cash += curr;
}
string Player::getName()
{
	return playerName;
}

void Player::setName(const string& name){
	playerName = name;
}

Player::~Player()
{

}
