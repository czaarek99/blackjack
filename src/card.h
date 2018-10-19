#ifndef CARD_H
#define CARD_H

const int MAX_CARD_STRING_LENGTH;

enum card_suit {
    HEARTS, SPADES, CLUBS, DIAMONDS
};

struct card {
    enum card_suit suit;
    short value;
};

void card_to_string(struct card card, char* card_string);

#endif
