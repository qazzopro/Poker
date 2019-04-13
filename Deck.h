#ifndef CARD_H
#define CARD_H

#define DECK_SIZE 52
#define HAND_SIZE 2
#define MAX_PLAYERS 6

typedef enum { FLOP, TURN, RIVER } Streets;

typedef int * Deck;

extern char values[]; 

extern char suits[];

// A shuffled deck based on Fisher-Yates Algorithm
Deck shuffleDeck();

void printCard(int Card);

#endif
