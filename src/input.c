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
        on_bad_input_func bad_input_func,
        on_good_input_func good_input_func) {

    while(true) {
        char* text_input = calloc(text_input_size, 1);
        get_input_discard_overflow(text_input, text_input_size);
        bool verified = (*verify_func)(text_input, input);
        if(verified) {
            (*good_input_func)(input);
            break;
        } else {
            (*bad_input_func)(text_input);
        }
    }
}