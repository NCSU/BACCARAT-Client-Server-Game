#ifndef RULES_H_
#define RULES_H_

class Rules {
public:
	Rules();
	bool drawthirdplayer() const;
	bool drawthirdbanker() const;
	int checkWinner();
	void getThirdPlayer(int);
	void getCards(int, int);
private:
	int twosumplayer;
	int twosumbanker;
	int thirdcardpl;
};

#endif
