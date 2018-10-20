#include <stdio.h>
#include <mem.h>
#include <stdlib.h>
#include "input.h"

void get_input_discard_overflow(char *input, short input_size) {
    fgets(input, input_size, stdin);

    if (strchr(input, '\n') == NULL) {
        int ch;
        while ((ch = fgetc(stdin)) != '\n' && ch != EOF);
    }
}

void require_input(void* input, short text_input_size,
        verify_input_func verify_func,
        void* verification_data,
        on_bad_input_func bad_input_func,
        on_good_input_func good_input_func) {

    while(true) {
        char* text_input = calloc(text_input_size, 1);
        get_input_discard_overflow(text_input, text_input_size);
        bool verified = (*verify_func)(text_input, input, verification_data);
        if(verified) {
            (*good_input_func)(input);
            break;
        } else {
            (*bad_input_func)(text_input);
        }
    }
}

bool verify_single_letter_action(char *allowed,
        char *chosen,
        void* verified_input) {

    for(int i = 0; i < strlen(allowed); i++) {
        if(allowed[i] == chosen[0]) {
            ((char*) verified_input)[0] = chosen[0];
            return true;
        }
    }

    return false;
}

char require_single_letter_input(verify_input_func verify_func,
        on_bad_input_func bad_input_func,
        on_good_input_func good_input_func) {
    char* input = malloc(1);
    require_input(input, 3, verify_func, NULL, bad_input_func, good_input_func);
    char input_char = input[0];
    free(input);
    return input_char;
}
