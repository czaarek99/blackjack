//TODO: Start with implementation then move on to ascii art in terminal
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mem.h>
#include <time.h>

enum card_suit {
    HEARTS, SPADES, CLUBS, DIAMONDS
};

struct card {
    enum card_suit suit;
    short value;
};

typedef struct card* deck;

const int MAX_DECKS = 6;
const int CARDS_IN_DECK = 52;
const int PLAYER_MAX_CARDS = 21;

void get_input_discard_overflow(char *input, short input_size) {
    fgets(input, input_size, stdin);

    if(strchr(input, '\n') == NULL) {
        int ch;
        while ((ch = fgetc(stdin)) != '\n' && ch != EOF);
    }
}

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

void copy_card(deck source, int *source_deck_index,
               deck target, int *target_deck_index) {
    target[*target_deck_index] = source[*source_deck_index];
    (*source_deck_index)++;
    (*target_deck_index)++;
}

struct card get_card_from_deck(deck deck, int* deck_index) {
    return deck[*deck_index];
}

void card_to_string(struct card card, char* card_string) {
    int len = 2;

    if(card.value == 1) {
        card_string[0] = 'A';
    } else if(card.value > 1 && card.value < 9) {
        itoa(card.value, card_string, 10);
    } else if(card.value == 10) {
        len = 3;
        card_string = "10?";
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
        case SPADES:
            card_string[suit_index] = 'S';
        case CLUBS:
            card_string[suit_index] = 'C';
        case DIAMONDS:
            card_string[suit_index] = 'D';
    }

    card_string[suit_index+1] = '\0';
}

deck make_deck(int size) {
    return malloc(size * sizeof(struct card));
}

int main() {
    const short input_size = 10;
    char input[input_size];

    setbuf(stdout, 0);
    printf("Welcome to blackjack!\n");
    printf("Please enter how many decks you'd like to play with (max %d):", MAX_DECKS);

    bool hasDecks = false;
    long deck_count = 0;
    while (!hasDecks) {
        get_input_discard_overflow(input, input_size);

        deck_count = strtol(input, NULL, 10);
        if (deck_count == LONG_MAX || deck_count == LONG_MIN || deck_count == 0 || deck_count > MAX_DECKS) {
            printf("Please enter a valid number between 1 and %d", MAX_DECKS);
        } else {
            hasDecks = true;
            printf("Starting blackjack with %lu decks", deck_count);
        }
    }

    deck deck = malloc(CARDS_IN_DECK * deck_count * sizeof(struct card));
    generate_deck(deck, deck_count);
    shuffle_deck(deck, deck_count);

    return 0;
}