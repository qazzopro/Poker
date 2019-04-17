#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdbool.h>

#include "Deck.h"
#include "Game.h"
#include "Computer.h"

static int activePlayers(Player players[]);
static void myTurn(Player me, int call);

int main (void) 
{
    Game Poker = newGame();
    printf("\n");
    int button = PLAYERS - 3;
    
    while (1) 
    {
        // Reset stack, pot and players' action
        for (int i = 0; i < PLAYERS; i++) {
            Poker->players[i]->stack = BUY_IN;
            Poker->players[i]->action = UNOPENED;
        }
        Poker->pot = 0;
        
        // Blinds
        if (button + 1 == PLAYERS) 
        {
            Poker->players[0]->stack -= SMALL_BLIND;
            Poker->players[1]->stack -= BIG_BLIND;
        }
        else if (button + 2 == PLAYERS) 
        {
            Poker->players[button+1]->stack -= SMALL_BLIND;
            Poker->players[0]->stack -= BIG_BLIND;
        }
        else
        {
            Poker->players[button+1]->stack -= SMALL_BLIND;
            Poker->players[button+2]->stack -= BIG_BLIND;
        }
        Poker->pot = BIG_BLIND + SMALL_BLIND;
        int call = BIG_BLIND;
        
        // Deal Cards
        printf("Dealing Cards\nPot size is $%d\nYour hand: ", Poker->pot);
        dealCards(Poker->players, Poker->deck);
        
        // Show player cards
        printCard(Poker->players[0]->hand[0]);
        printCard(Poker->players[0]->hand[1]);
        printf("\n\n");
        
        // Preflop
        int turn = button + 3; // Who's turn
        if (button + 3 >= PLAYERS) turn -= PLAYERS;
        while (activePlayers(Poker->players) >= 2)
        {
            if (turn == 0 && Poker->players[0]->action != FOLD)  // My turn
                myTurn(Poker->players[0], call);                        
            
            else 
                // Computer's turn
                compTurn(Poker->players[turn], call, calculatePosition(turn, button)); 
            
            turn++; 
            if (turn == PLAYERS) turn = 0;
        }
        
        
        printf("The Flop\n");
        printCommunity(Poker->deck, FLOP);
        printf("\n\n");
        
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

static int activePlayers(Player players[])
{
    int active = 0;
    for (int i = 0; i < PLAYERS; i++)
    {
        if (players[i]->action != FOLD) active++;   
    }
    return active;
}

static void myTurn(Player me, int call)
{
    printf("Type number for action\n");
    if (call == BIG_BLIND) printf("0: CALL, 1: BET, 2: FOLD\n");
    else printf("0: CALL, 1: RAISE, 2: FOLD\n");
    int action;
    scanf("%d", &action); 
    if (action == 2) me->action = FOLD; 
}
