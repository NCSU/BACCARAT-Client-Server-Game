#include "Rules.h"

Rules::Rules(){
}
void Rules::getCards(int player, int banker){
	twosumplayer = player;
	twosumbanker = banker;
}
int Rules::checkWinner(){
	if (twosumplayer > twosumbanker)
		return 1;
	else if (twosumplayer < twosumbanker)
		return 2;
	else return 3;
	return 0;
}
bool Rules::drawthirdplayer() const{
	if (twosumplayer <= 5)
		return true;
	else return false;
}
void Rules::getThirdPlayer(int thirdPl){
	thirdcardpl = thirdPl;
}
bool Rules::drawthirdbanker() const{
	if (twosumbanker <= 2)
		return true;
	if (twosumbanker == 3 && thirdcardpl != 8)
		return true;
	else return false;
	if (twosumbanker == 4 && thirdcardpl >= 2 && thirdcardpl <= 7)
		return true;
	else return false;
	if (twosumbanker == 5 && thirdcardpl >= 4 && thirdcardpl <= 7)
		return true;
	else return false;
	if (twosumbanker == 6 && thirdcardpl >= 6 && thirdcardpl <= 7)
		return true;
	else return false;
	if (twosumbanker == 7)
		return false;
}
