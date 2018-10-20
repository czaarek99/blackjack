#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mem.h>
#include "deck.h"
#include "input.h"

const int MAX_DECKS = 6;
const int DEALER_MAX_DRAW_SCORE = 17;
const int START_MONEY = 200;

enum game_state {
    PLAYERS_TURN, HOUSE_TURN, FINISH
};

bool should_house_hit(struct deck_score score) {
    bool draw_on_alt_score = score.score > BLACKJACK && score.score < DEALER_MAX_DRAW_SCORE;
    return score.score < DEALER_MAX_DRAW_SCORE || draw_on_alt_score;
}

bool verify_deck_count(char *input,
                       void *verified_input, void *verification_data) {
    long deck_count = strtol(input, NULL, 10);
    if (deck_count != LONG_MAX && deck_count != LONG_MIN && deck_count != 0 && deck_count <= MAX_DECKS) {
        *(int *) (verified_input) = deck_count;
        return true;
    } else {
        return false;
    }
}

void on_deck_bad_input(char *bad_input) {
    printf("Please enter a valid number between 1 and %d:", MAX_DECKS);
}

void on_deck_good_input(void *good_input) {
    long deck_count = *(long *) (good_input);
    printf("Starting blackjack with %lu decks\n", deck_count);
}

SINGLE_LETTER_VERIFY_FUNC("hs", verify_game_action)

void on_game_action_bad_input(char *bad_input) {
    printf("Please enter either 'h' to hit or 's' to stand:");
}

void on_game_action_good_input(void *good_input) {
    char good_input_char = good_input_to_char(good_input);
    if (good_input_char == 'h') {
        printf("Hit! Giving you another card.\n");
    } else if (good_input_char == 's') {
        printf("Stand. Giving it up for the dealer\n");
    }
}

bool verify_bet(char *input, void *verified_input,
                void *verification_data) {
    int *max_bet = (int *) verification_data;
    long bet = strtol(input, NULL, 10);
    if (bet != LONG_MAX && bet != LONG_MIN && bet != 0 && bet <= *max_bet) {
        *(int *) (verified_input) = bet;
        return true;
    } else {
        return false;
    }
}

void on_bet_bad_input(char *bad_input) {
    printf("Please enter a valid bet: ");
}

void on_bet_good_input(void *good_input) {
    int *bet = (int *) good_input;
    printf("Betting %i$\n", *bet);
}

void play_hand(long deck_count, int *player_money) {
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

    printf("Please enter a bet between 1 and %i:", *player_money);

    int *player_bet = malloc(sizeof(int));
    require_input(player_bet, 10, &verify_bet, player_money,
                  &on_bet_bad_input, &on_bet_good_input);

    *player_money -= *player_bet;

    enum game_state state = PLAYERS_TURN;
    while (state != FINISH) {
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

        bool player_has_blackjack = has_blackjack(player_deck_score);

        if (state == PLAYERS_TURN) {
            if (player_deck_score.score > BLACKJACK && player_deck_score.alt_score > BLACKJACK) {
                printf("Your score exceeded %i therefore you lose!\n", BLACKJACK);
                state = FINISH;
            } else if (house_has_blackjack && !player_has_blackjack) {
                printf("Dealer has blackjack and you don't! You lose!\n");
                state = FINISH;
            } else {
                printf("Enter 'h' for another card or 's' to stand:");

                char game_action = require_single_letter_input(&verify_game_action,
                                                               &on_game_action_bad_input, &on_game_action_good_input);

                if (game_action == 'h') {
                    copy_card_between_decks(game_deck, game_deck_index, player_deck, player_deck_index);
                } else if (game_action == 's') {
                    state = HOUSE_TURN;
                }
            }
        } else if (house_turn) {
            enum better_deck winner = compare_decks(house_deck,
                                                    *house_game_index, player_deck, *player_deck_index);
            if (winner == FIRST) {
                printf("House has better hand. You lose!\n");
            } else if (winner == SECOND) {
                printf("You have a better hand. You win!\n");
                *player_money = *player_money + (int) (*player_bet * 1.5);
            } else {
                printf("Tie!\n");
                *player_money = *player_money + *player_bet;
            }

            state = FINISH;
        }

        if(state == FINISH) {
            printf("You now have $%i\n", *player_money);
        }
    }

    free(player_bet);
}

SINGLE_LETTER_VERIFY_FUNC("pq", verify_continue_game)

void on_continue_game_bad_input(char *bad_input) {
    printf("Enter 'p' to keep playing enter 'q' to quit.");
}

void on_continue_game_good_input(void *good_input) {
    char good_input_char = good_input_to_char(good_input);
    if (good_input_char == 'q') {
        printf("Thanks for playing! Quitting\n");
    } else if (good_input_char == 'p') {
        printf("Let's keep playing!\n");
    }
}

int main() {
    setbuf(stdout, 0);
    printf("Welcome to blackjack!\nYou start with $%i.\n", START_MONEY);
    printf("Please enter how many decks you'd like to play with (max %i):", MAX_DECKS);

    long *deck_count = malloc(sizeof(int));
    require_input(deck_count, 10, &verify_deck_count, NULL,
                  &on_deck_bad_input, &on_deck_good_input);

    int *player_money = malloc(sizeof(int));
    *player_money = START_MONEY;

    while (true) {
        play_hand(*deck_count, player_money);

        printf("Would you like to keep playing?\n");
        printf("Enter 'q' to quit or 'p' to continue the game:");

        char char_input = require_single_letter_input(&verify_continue_game,
                                                      &on_continue_game_bad_input, &on_continue_game_good_input);
        if (char_input == 'q') {
            printf("This games profit: %i$", *player_money - START_MONEY);
            break;
        }
    }

    free(deck_count);
    free(player_money);

    return 0;
}