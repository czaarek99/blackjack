cmake_minimum_required(VERSION 3.12)
project(blackjack C)

set(CMAKE_C_STANDARD 11)

add_executable(blackjack
        src/main.c src/util.h
        src/card.c src/card.h
        src/deck.c src/deck.h
        src/input.c src/input.h
        )