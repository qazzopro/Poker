#ifndef GAME_H
#define GAME_H

#include "Deck.h"

struct _player {
    int hand[HAND_SIZE];
    int rank;
    int best[5];
};

typedef struct _player *Player;

struct _game {
    Deck deck;
    Player players[MAX_PLAYERS];
};

typedef struct _game *Game; 

typedef enum {
    STRAIGHT_FLUSH,
    QUADS,
    FULL_HOUSE,
    FLUSH,
    STRAIGHT,
    TRIPS,
    TWO_PAIR,
    PAIR,
    HIGH_CARD
} Ranks; 

// Create a new game
Game newGame();

// Deal cards
void dealCards(Player players[], Deck deck);

// Print the community cards on specific street
void printCommunity(int deck[], Streets street);

// Calculate the value of current hand
void calculateHand(Player player, int cards[]);

// Calculate the player that won
void calculateWinner(Player players[], int community[5]);

// Destroy a game
void destroyGame(Game game);

#endif
