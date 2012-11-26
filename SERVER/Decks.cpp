#include <algorithm>
using std::swap;
#include <ctime>
using std::rand;
using std::srand;

#include "CardDeal.h"
#include "Decks.h"



Deck::Deck() {
	for (int i = 0; i < 52; i++)
		allCards[i] = Card(i);
	shuffle();
}

Card Deck::dealOneCard(){
	if (nextCardIndex >= 52){
		nextCardIndex = 0;
		shuffle();
	}
    return allCards[nextCardIndex++];
}

void Deck::shuffle() {
	srand(time(0));
	for (int i = 0; i < (52); i++) {
		int randnum = i + (rand() % (52-i));
		swap(allCards[i], allCards[randnum]);
	}
    nextCardIndex = 0;
}
