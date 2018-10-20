#ifndef INPUT_H
#define INPUT_H
#include <stdbool.h>

typedef bool (*verify_input_func)(char*, void*);
typedef void (*on_bad_input_func)(char*);
typedef void (*on_good_input_func)(void*);


void get_input_discard_overflow(char *input, short input_size);
void require_input(void* input, short input_size,
        verify_input_func verify_func,
        on_bad_input_func bad_input_func,
        on_good_input_func good_input_func);

#endif
