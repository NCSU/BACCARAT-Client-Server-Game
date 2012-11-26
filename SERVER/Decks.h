#ifndef DECK_H
#define DECK_H

class Deck {
    public:
        Deck();
        Card dealOneCard();
        void shuffle();

    private:
        Card allCards[52];
        int  nextCardIndex;
};

#endif
