#ifndef CARD_H
#define CARD_H

class Card {
    public:
        Card();
        Card(int CardDeal);
        int getSuit() const;
        int getFace() const;

    private:
        int cCard;

        static const int CARD_FACES[13];
        static const int CARD_SUITS[4];
};

#endif
