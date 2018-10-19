#include "deck.h"
#include <time.h>
#include <stdlib.h>
#include <mem.h>

const int CARDS_IN_DECK = 52;
const int PLAYER_MAX_CARDS = 21;

void generate_deck(deck deck, int deck_count) {
    for (int deck_num = 0; deck_num < deck_count; deck_num++) {
        for (short card_value = 1; card_value < 14; card_value++) {
            struct card card_hearts;
            card_hearts.suit = HEARTS;
            card_hearts.value = card_value;

            struct card card_spades;
            card_spades.suit = SPADES;
            card_spades.value = card_value;

            struct card card_clubs;
            card_clubs.suit = CLUBS;
            card_clubs.value = card_value;

            struct card card_diamonds;
            card_diamonds.suit = DIAMONDS;
            card_diamonds.value = card_value;

            const int deck_index = deck_num * CARDS_IN_DECK + (card_value - 1) * 4;
            deck[deck_index] = card_hearts;
            deck[deck_index + 1] = card_spades;
            deck[deck_index + 2] = card_clubs;
            deck[deck_index + 3] = card_diamonds;
        }
    }
}

void shuffle_deck(deck deck, int deck_count) {
    srand(time(NULL));
    for(int i = 0; i < deck_count * CARDS_IN_DECK; i++) {
        int swap_index = rand() % CARDS_IN_DECK;
        struct card temp = deck[i];
        deck[i] = deck[swap_index];
        deck[swap_index] = temp;
    }
}

const int CARD_ACE = 1;
struct deck_score get_deck_score(deck deck, int deck_size) {
    struct deck_score score;
    score.score = 0;
    score.alt_score = 0;

    for(int i = 0; i < deck_size; i++) {
        struct card card = deck[i];
        if(card.value == CARD_ACE) {
            score.score += 1;
            score.alt_score += 14;
        } else {
            score.score += card.value;
            score.alt_score += card.value;
        }
    }

    return score;
}

deck make_deck(int size) {
    return malloc(size * sizeof(struct card));
}

char* alloc_deck_string() {
    return calloc((MAX_CARD_STRING_LENGTH + 1) * PLAYER_MAX_CARDS, 1);
}

struct card get_card_from_deck(deck deck, int* deck_index) {
    return deck[*deck_index];
}

void deck_to_string(deck deck, int deck_size, char* deck_string) {
    strcpy(deck_string, "");
    for(int i = 0; i < deck_size; i++) {
        char card_string[MAX_CARD_STRING_LENGTH];
        struct card card = deck[i];
        card_to_string(card, card_string);
        strcat(deck_string, card_string);
        strcat(deck_string, " ");
    }
}
