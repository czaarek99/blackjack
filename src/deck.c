#include "deck.h"
#include "util.h"
#include <time.h>
#include <stdlib.h>
#include <mem.h>

const int CARDS_IN_DECK = 52;
const int PLAYER_MAX_CARDS = 21;
const int BLACKJACK = 21;

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
            score.alt_score += 11;
        } else if(card.value > 9) {
            score.score += 10;
            score.alt_score += 10;
        } else {
            score.score += card.value;
            score.alt_score += card.value;
        }
    }

    return score;
}

int get_deck_best_score(deck deck, int deck_size) {
    struct deck_score score = get_deck_score(deck, deck_size);
    if(score.score > BLACKJACK || score.alt_score > BLACKJACK) {
        return score.score;
    } else {
        return MAX(score.score, score.alt_score);
    }
}

deck make_deck(int size) {
    return malloc(size * sizeof(struct card));
}

char* alloc_deck_string() {
    return calloc((MAX_CARD_STRING_LENGTH + 1) * PLAYER_MAX_CARDS, 1);
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

void copy_card_between_decks(deck source, int *source_deck_index,
                             deck target, int *target_deck_index) {
    target[*target_deck_index] = source[*source_deck_index];
    (*source_deck_index)++;
    (*target_deck_index)++;
}

bool has_blackjack(struct deck_score score) {
    return score.score == BLACKJACK || score.alt_score == BLACKJACK;
}

enum better_deck compare_decks(deck deck1, int deck1_size, deck deck2, int deck2_size) {
    int score1 = get_deck_best_score(deck1, deck1_size);
    int score2 = get_deck_best_score(deck2, deck2_size);

    if(score1 > BLACKJACK) {
        return SECOND;
    } else if(score2 > BLACKJACK) {
        return FIRST;
    } else if(score1 == score2) {
        return NONE;
    } else if(score1 > score2){
        return FIRST;
    } else {
        return SECOND;
    }
}
