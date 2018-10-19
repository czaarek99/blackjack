#ifndef DECK_H
#define DECK_H

#include "card.h"

typedef struct card* deck;

struct deck_score {
    int score;
    int alt_score;
};

const int CARDS_IN_DECK;
const int PLAYER_MAX_CARDS;

void generate_deck(deck deck, int deck_count);
void shuffle_deck(deck deck, int deck_count);
struct deck_score get_deck_score(deck deck, int deck_size);
deck make_deck(int size);
char* alloc_deck_string();
struct card get_card_from_deck(deck deck, int* deck_index);
void deck_to_string(deck deck, int deck_size, char* deck_string);
void copy_card_between_decks(deck source, int *source_deck_index,
                             deck target, int *target_deck_index);

#endif
