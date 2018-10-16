//TODO: Start with implementation then move on to ascii art in terminal
#include <stdio.h>
#include <stdlib.h>

enum card_suit {
    HEARTS, SPADES, CLUBS, DIAMONDS
};

struct card {
    enum card_suit suit;
    short value;
};

int main() {
    const short input_size = 3;
    int deck_count;
    char input[input_size];

    printf("Please enter how many decks you'd like to play with (max 6): ");
    fgets(input, input_size, stdin);
    int result = strtol(input, NULL, 10);
    if(result == LONG_MAX || result == LONG_MIN || result == 0) {
        perror("Invalid number.");
    } else {
        printf("%d", result);
    }
    
    return 0;
}