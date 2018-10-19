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
const int MAX_CARD_STRING_LENGTH = 4;
const int MAX_SCORE = 21;

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
        case SPADES:
            card_string[suit_index] = 'S';
        case CLUBS:
            card_string[suit_index] = 'C';
        case DIAMONDS:
            card_string[suit_index] = 'D';
    }

    card_string[suit_index+1] = '\0';
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

struct deck_score {
    int score;
    int alt_score;
};

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
            printf("Please enter a valid number between 1 and %d:", MAX_DECKS);
        } else {
            hasDecks = true;
            printf("Starting blackjack with %lu decks\n", deck_count);
        }
    }

    deck game_deck = make_deck(CARDS_IN_DECK * deck_count);
    generate_deck(game_deck, deck_count);
    shuffle_deck(game_deck, deck_count);

    int* game_deck_index = malloc(sizeof(int));
    *game_deck_index = 0;

    deck house_deck = make_deck(PLAYER_MAX_CARDS);
    int* house_game_index = malloc(sizeof(int));
    *house_game_index = 0;

    deck player_deck = make_deck(PLAYER_MAX_CARDS);
    int* player_deck_index = malloc(sizeof(int));
    *player_deck_index = 0;

    for(int i = 0; i < 2; i++) {
        copy_card(game_deck, game_deck_index, house_deck, house_game_index);
        copy_card(game_deck, game_deck_index, player_deck, player_deck_index);
    }

    int game_round = 0;
    while(true) {
        printf("-----\n");

        char* house_deck_string;
        if(game_round == 0) {
            house_deck_string = calloc(MAX_CARD_STRING_LENGTH + 3, 1);
            struct card first_house_card = house_deck[0];
            char first_card_string[MAX_CARD_STRING_LENGTH];
            card_to_string(first_house_card, first_card_string);
            strcpy(house_deck_string, first_card_string);
            strcat(house_deck_string, " ?");
        } else {
            //+1 to account for spaces
            house_deck_string = alloc_deck_string();
            deck_to_string(house_deck, *house_game_index, house_deck_string);
        }

        printf("House hand: %s\n", house_deck_string);
        free(house_deck_string);

        char* player_deck_string = alloc_deck_string();
        deck_to_string(player_deck, *player_deck_index, player_deck_string);

        struct deck_score score = get_deck_score(player_deck, *player_deck_index);

        char* score_string = calloc(20, 1);
        if(score.score == score.alt_score || score.alt_score > MAX_SCORE) {
            itoa(score.score, score_string, 10);
        } else {
            itoa(score.score, score_string, 10);
            char alt_score_string[3];
            itoa(score.alt_score, alt_score_string, 10);
            strcat(score_string, " or ");
            strcat(score_string, alt_score_string);
        }

        printf("Your hand is: %swith a score of: %s\n", player_deck_string, score_string);
        free(player_deck_string);
        free(score_string);

        if(score.score > MAX_SCORE && score.alt_score > MAX_SCORE) {
            printf("Your score exceeded %i therefore you lose!", MAX_SCORE);
            break;
        }

        printf("Enter 'h' for another card or 's' to stand:");

        char input[3];
        get_input_discard_overflow(input, 3);
        char option = input[0];

        fflush(stdout);
        if(option == 'h') {
            copy_card(game_deck, game_deck_index, player_deck, player_deck_index);
        }

        game_round++;
    }

    return 0;
}