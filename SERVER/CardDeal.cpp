#include "CardDeal.h"

const int Card::CARD_FACES[13] =  {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const int Card::CARD_SUITS[4] = {0, 1, 2, 3};

Card::Card() {
}

Card::Card(int CardDeal) {
    cCard = CardDeal;
}

int Card::getFace() const {
    return CARD_FACES[cCard%13];
}

int Card::getSuit() const {
    return CARD_SUITS[cCard/13];
}
