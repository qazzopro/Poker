#ifndef GAME_H
#define GAME_H

#define BUY_IN 1000
#define BIG_BLIND (BUY_IN / 100)
#define SMALL_BLIND (BUY_IN / 200)

#include "Deck.h"

typedef enum { UNOPENED, CHECK, CALL, BET, RAISE, FOLD } Actions;

struct _player 
{
    int hand[HAND_SIZE];
    int rank;
    int best[5];
    int stack;
    Actions action; 
};

typedef struct _player *Player;

struct _game {
    Deck deck;
    Player players[PLAYERS];
    int pot;
};

typedef struct _game *Game; 
  
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
