#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mem.h>
#include "deck.h"

const int MAX_DECKS = 6;
const int DEALER_MAX_DRAW_SCORE = 17;

enum game_state {
    PLAYERS_TURN, HOUSE_TURN, FINISH
};

void get_input_discard_overflow(char *input, short input_size) {
    fgets(input, input_size, stdin);

    if (strchr(input, '\n') == NULL) {
        int ch;
        while ((ch = fgetc(stdin)) != '\n' && ch != EOF);
    }
}

bool should_house_hit(struct deck_score score) {
    bool draw_on_alt_score = score.score > BLACKJACK && score.score < DEALER_MAX_DRAW_SCORE;
    return score.score < DEALER_MAX_DRAW_SCORE || draw_on_alt_score;
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

    int *game_deck_index = malloc(sizeof(int));
    *game_deck_index = 0;

    deck house_deck = make_deck(PLAYER_MAX_CARDS);
    int *house_game_index = malloc(sizeof(int));
    *house_game_index = 0;

    deck player_deck = make_deck(PLAYER_MAX_CARDS);
    int *player_deck_index = malloc(sizeof(int));
    *player_deck_index = 0;

    for (int i = 0; i < 2; i++) {
        copy_card_between_decks(game_deck, game_deck_index, house_deck, house_game_index);
        copy_card_between_decks(game_deck, game_deck_index, player_deck, player_deck_index);
    }

    enum game_state state = PLAYERS_TURN;
    while (true) {
        printf("-----\n");
        bool house_turn = state == HOUSE_TURN;

        struct deck_score house_deck_score = get_deck_score(house_deck, *house_game_index);
        while (house_turn && should_house_hit(house_deck_score)) {
            printf("Dealer is drawing...\n");
            copy_card_between_decks(game_deck, game_deck_index, house_deck, house_game_index);
            house_deck_score = get_deck_score(house_deck, *house_game_index);
        }

        char *house_deck_string;
        bool house_has_blackjack = has_blackjack(house_deck_score);
        if (house_turn || house_has_blackjack) {
            house_deck_string = alloc_deck_string();
            deck_to_string(house_deck, *house_game_index, house_deck_string);
        } else {
            //+1 to account for spaces
            house_deck_string = calloc(MAX_CARD_STRING_LENGTH + 3, 1);
            struct card first_house_card = house_deck[0];
            char first_card_string[MAX_CARD_STRING_LENGTH];
            card_to_string(first_house_card, first_card_string);
            strcpy(house_deck_string, first_card_string);
            strcat(house_deck_string, " ?");
        }

        if (house_turn) {
            state = FINISH;
        }

        printf("House hand: %s\n", house_deck_string);
        free(house_deck_string);

        char *player_deck_string = alloc_deck_string();
        deck_to_string(player_deck, *player_deck_index, player_deck_string);

        struct deck_score player_deck_score = get_deck_score(player_deck, *player_deck_index);

        char *score_string = calloc(20, 1);
        if (player_deck_score.score == player_deck_score.alt_score || player_deck_score.alt_score > BLACKJACK) {
            itoa(player_deck_score.score, score_string, 10);
        } else {
            itoa(player_deck_score.score, score_string, 10);
            char alt_score_string[3];
            itoa(player_deck_score.alt_score, alt_score_string, 10);
            strcat(score_string, " or ");
            strcat(score_string, alt_score_string);
        }

        printf("Your hand is: %swith a score of: %s\n", player_deck_string, score_string);
        free(player_deck_string);
        free(score_string);

        if (state == PLAYERS_TURN) {
            bool player_has_blackjack = has_blackjack(player_deck_score);
            if (player_deck_score.score > BLACKJACK && player_deck_score.alt_score > BLACKJACK) {
                printf("Your score exceeded %i therefore you lose!", BLACKJACK);
                break;
            } else if(house_has_blackjack && !player_has_blackjack) {
                printf("House has blackjack and you don't! You lose!\n");
                break;
            }

            printf("Enter 'h' for another card or 's' to stand:");

            char input[3];
            get_input_discard_overflow(input, 3);
            char option = input[0];

            if (option == 'h') {
                copy_card_between_decks(game_deck, game_deck_index, player_deck, player_deck_index);
            } else if (option == 's') {
                state = HOUSE_TURN;
            }
        } else if(state == FINISH) {
            break;
        }
    }

    return 0;
}