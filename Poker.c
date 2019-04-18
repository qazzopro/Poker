#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#include "Deck.h"
#include "Game.h"
#include "Player.h"
#include "Computer.h"

static int active(Player players[]);
static int allIn(Player players[]);


int main (void) 
{
    Game Poker = newGame();
    printf("\n");
    int button = PLAYERS - 3;
    
    while (1) 
    {
        // Reset stack, pot, chips placed and players' action
        for (int i = 0; i < PLAYERS; i++) {
            Poker->players[i]->stack = BUY_IN;
            Poker->players[i]->action = UNOPENED;
            Poker->players[i]->placed[0] = 0;
        }
        Poker->pot = 0;
        
        // Blinds
        if (button + 1 == PLAYERS) 
        {
            Poker->players[0]->stack -= SMALL_BLIND;
            Poker->players[0]->placed[0] = SMALL_BLIND;
            Poker->players[1]->stack -= BIG_BLIND;
            Poker->players[1]->placed[0] = BIG_BLIND;
        }
        else if (button + 2 == PLAYERS) 
        {
            Poker->players[button+1]->stack -= SMALL_BLIND;
            Poker->players[button+1]->placed[0] = SMALL_BLIND;
            Poker->players[0]->stack -= BIG_BLIND;
            Poker->players[0]->placed[0] = BIG_BLIND;
        }
        else
        {
            Poker->players[button+1]->stack -= SMALL_BLIND;
            Poker->players[button+1]->placed[0] = SMALL_BLIND;
            Poker->players[button+2]->stack -= BIG_BLIND;
            Poker->players[button+2]->placed[0] = BIG_BLIND;
        }
        Poker->pot = BIG_BLIND + SMALL_BLIND;
        int *call = malloc(sizeof(int));
        *call = BIG_BLIND;
        
        // Deal Cards
        printf("Dealing Cards\nYour hand: ");
        dealCards(Poker->players, Poker->deck);
        
        // Show player cards
        printCard(Poker->players[0]->hand[0]);
        printCard(Poker->players[0]->hand[1]);
        printf("\n");
        
        // Preflop
        int turn = button + 3; // Who's turn
        if (button + 3 >= PLAYERS) turn -= PLAYERS;
        while (active(Poker->players) != 0)
        {
            if (Poker->players[turn]->action != FOLD 
                    && Poker->players[turn]->action != ALL_IN_CALL
                    && Poker->players[turn]->action != ALL_IN_RAISE)
            {
            printf("Player %d's turn, with a stack of $%d and a pot of $%d.\n", turn + 1, Poker->players[turn]->stack, Poker->pot);
                if (turn == 0)  
                { // My turn
                    printf("my turn.\n");
                    Poker->pot += myTurn(Poker->players[0], call, Poker->raised);
                }                        
                
                else 
                {
                    // Computer's turn 
                               
                Poker->pot += compTurn(Poker->players[turn], PREFLOP, call, Poker->raised, calculatePosition(turn, button), Poker->pot); 
                }
            }
            
            if (Poker->players[turn]->action == BET 
                || Poker->players[turn]->action == RAISE
                || Poker->players[turn]->action == ALL_IN_RAISE)
            {
                if (Poker->players[turn]->action == ALL_IN_RAISE ||
                    Poker->players[turn]->action == ALL_IN_CALL)
                    Poker->players[turn]->stack = 0;
                
                for (int i = 0; i < PLAYERS; i++)
                    if (i != turn && Poker->players[i]->action != ALL_IN_RAISE 
                        && Poker->players[i]->action != ALL_IN_CALL 
                        && Poker->players[i]->action != UNOPENED
                        && Poker->players[i]->action != FOLD) {
                        Poker->players[i]->action = UNOPENED;}
            }
            turn++; 
            turn %= PLAYERS;
        }
        
        
        printf("The Flop\n");
        printCommunity(Poker->deck, FLOP);
        printf("\n\n");
        Poker->raised[0] = 0;
        
        printf("The Turn\n");
        printCommunity(Poker->deck, TURN);
        printf("\n\n");
        
        printf("The River\n");
        printCommunity(Poker->deck, RIVER);
        printf("\n\n");
        
        // Calculate Player Hand
        for (int i = 0; i < PLAYERS; i++)
        {
            printf("\nPlayer %d's Hand\n", i + 1);
            printCard(Poker->players[i]->hand[0]);
            printCard(Poker->players[i]->hand[1]);
            calculateHand(Poker->players[i], Poker->deck); 
            for (int j = 0; j <= 4; j++) 
            {      
                printf("%c ", values[Poker->players[i]->best[j]]);
            }
            printf("\n");
        }
        printf("\n");
        
        // Calculate winner
        int community[5];
        for (int i = 0; i <= 4; i++) community[i] = Poker->deck[i+12];
        calculateWinner(Poker->players, community);
        printf("\n\n");
        
        // Move button
        button++;
        if (button == PLAYERS) button = 0;
    }
    destroyGame(Poker);
    
    return 0; 
}

static int active(Player players[])
{
    int active = 0;
    for (int i = 0; i < PLAYERS; i++)
        if (players[i]->action == UNOPENED 
            || players[i]->action == CALL 
            || players[i]->action == BET
            || players[i]->action == RAISE
            || players[i]->action == CHECK) 
            active++;   
            
    return active;
}

static int allIn(Player players[])
{
    int allin = 0;
    for (int i = 0; i < PLAYERS; i++)
        if (players[i]->action == ALL_IN_CALL || players[i]->action == ALL_IN_RAISE) allin++;
        
    return allin; 
    
} 
