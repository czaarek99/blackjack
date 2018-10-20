#include "card.h"
#include <stdlib.h>

const int MAX_CARD_STRING_LENGTH = 4;

void card_to_string(struct card card, char* card_string) {
    int len = 2;

    if(card.value == 1) {
        card_string[0] = 'A';
    } else if(card.value > 1 && card.value < 10) {
        char card_num[2];
        itoa(card.value, card_num, 10);
        card_string[0] = card_num[0];
    } else if(card.value == 10) {
        len = 3;
        card_string[0] = '1';
        card_string[1] = '0';
    } else if(card.value == 11) {
        card_string[0] = 'J';
    } else if(card.value == 12) {
        card_string[0] = 'Q';
    } else if(card.value == 13) {
        card_string[0] = 'K';
    }

    int suit_index = len == 3 ? 2 : 1;
    switch(card.suit) {
        case HEARTS:
            card_string[suit_index] = 'H';
            break;
        case SPADES:
            card_string[suit_index] = 'S';
            break;
        case CLUBS:
            card_string[suit_index] = 'C';
            break;
        case DIAMONDS:
            card_string[suit_index] = 'D';
            break;
    }

    card_string[suit_index+1] = '\0';
}