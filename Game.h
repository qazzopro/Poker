#ifndef GAME_H
#define GAME_H

#define BUY_IN 1000
#define BIG_BLIND (BUY_IN / 100)
#define SMALL_BLIND (BUY_IN / 200)

// Positions
#define BUTTON  0
#define SB      1
#define BB      2
#define UTG     3
#define HJ      4
#define CO      5

#define TYPES 9
#define REGS 5

#include "Deck.h"

typedef enum { UNOPENED, CHECK, CALL, BET, RAISE, FOLD, ALL_IN_CALL, ALL_IN_RAISE } Actions;

typedef enum { 
    PLAYER, 
    TIGHT_PASSIVE_FISH, 
    LOOSE_PASSIVE_FISH, 
    PASSIVE_WHALE, 
    NIT, 
    TIGHT_REG, 
    STANDARD_REG,
    LOOSE_REG,
    AGGRO_FISH,
    AGGRO_WHALE,
} PlayerType;
     

struct _player 
{
    int hand[HAND_SIZE];
    int rank;
    int best[5];
    int stack;
    Actions action;
    PlayerType type; 
};

typedef struct _player *Player;

struct _game {
    Deck deck;
    Player players[PLAYERS];
    int pot;
    int *raised;
};

typedef struct _game *Game; 
  
// Create a new game
Game newGame();

// Deal cards
void dealCards(Player players[], Deck deck);

// Print the community cards on specific street
void printCommunity(int deck[], Streets street);

int calculatePosition(int player, int button);

// Calculate the value of current hand
void calculateHand(Player player, int cards[]);

// Calculate the player that won
void calculateWinner(Player players[], int community[5]);

// Destroy a game
void destroyGame(Game game);

#endif
