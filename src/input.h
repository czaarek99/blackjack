#ifndef INPUT_H
#define INPUT_H
#include <stdbool.h>

#define SINGLE_LETTER_VERIFY_FUNC(LETTERS, FUNC_NAME)  \
    bool (FUNC_NAME)(char *input, void *verified_input, void *verification_data) { \
        return verify_single_letter_action((LETTERS), input, verified_input); \
    } \


typedef bool (*verify_input_func)(char*, void*, void*);
typedef void (*on_bad_input_func)(char*);
typedef void (*on_good_input_func)(void*);


void require_input(void* input, short text_input_size,
        verify_input_func verify_func,
        void* verification_data,
        on_bad_input_func bad_input_func,
        on_good_input_func good_input_func);

bool verify_single_letter_action(char *allowed,
        char *chosen,
        void* verified_input);

char require_single_letter_input(verify_input_func verify_func,
        on_bad_input_func bad_input_func,
        on_good_input_func good_input_func);

#endif
