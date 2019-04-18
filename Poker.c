#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#include "Deck.h"
#include "Game.h"
#include "Computer.h"

static int unopened(Player players[]);
static int folded(Player players[]);
static int activePlayers(Player players[]);
static int myTurn(Player me, int *call, int *raised);
static int allIn(Player players[]);

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
        int *call = malloc(sizeof(int));
        *call = BIG_BLIND;
        
        // Deal Cards
        printf("Dealing Cards\nYour hand: ");
        dealCards(Poker->players, Poker->deck);
        
        // Show player cards
        printCard(Poker->players[0]->hand[0]);
        printCard(Poker->players[0]->hand[1]);
        printf("\n\n");
        
        // Preflop
        int turn = button + 3; // Who's turn
        if (button + 3 >= PLAYERS) turn -= PLAYERS;
        while (unopened(Poker->players) != 0 || activePlayers(Poker->players) < (folded(Poker->players) + 1))
        {
            if (turn == 0)  
            { // My turn
                if (Poker->players[0]->action != FOLD 
                    && Poker->players[0]->action != ALL_IN_CALL
                    && Poker->players[0]->action != ALL_IN_RAISE)
                {
                    printf("Pot size is $%d\n", Poker->pot);
                    Poker->pot += myTurn(Poker->players[0], call, Poker->raised);
                }
            }                        
            
            else if (Poker->players[0]->action != FOLD 
                    && Poker->players[0]->action != ALL_IN_CALL
                    && Poker->players[0]->action != ALL_IN_RAISE)
            { 
                // Computer's turn
                printf("Player %d's turn, with a stack of $%d and a pot of $%d.\n", turn + 1, Poker->players[turn]->stack, Poker->pot);
                Poker->pot += compTurn(Poker->players[turn], PREFLOP, call, Poker->raised, calculatePosition(turn, button), Poker->pot); 
            }
            
            
            if (Poker->players[turn]->action == BET 
                || Poker->players[turn]->action == RAISE
                || Poker->players[turn]->action == ALL_IN_RAISE)
            {
                for (int i = 0; i < PLAYERS; i++)
                    if (i != turn && Poker->players[i]->action != ALL_IN_RAISE 
                        && Poker->players[i]->action != ALL_IN_CALL)
                        
                        Poker->players[i]->action = UNOPENED;
            }
            turn++; 
            if (turn == PLAYERS) turn = 0;
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
        exit(0);
    }
    destroyGame(Poker);
    
    return 0; 
}

static int unopened(Player players[])
{
    int active = 0;
    for (int i = 0; i < PLAYERS; i++)
        if (players[i]->action == UNOPENED) active++;   
        
    return active;
}
static int folded(Player players[])
{
    int folded = 0;
    for (int i = 0; i < PLAYERS; i++)
        if (players[i]->action == UNOPENED) folded++; 
        
    return folded;
}

static int allIn(Player players[])
{
    int allin = 0;
    for (int i = 0; i < PLAYERS; i++)
        if (players[i]->action == ALL_IN_CALL || players[i]->action == ALL_IN_RAISE) allin++;
        
    return allin; 
    
} 

static int activePlayers(Player players[])
{
    int active = 0;
    for (int i = 0; i < PLAYERS; i++)
        if (players[i]->action != FOLD 
            || players[i]->action == ALL_IN_CALL || players[i]->action == ALL_IN_RAISE) 
            
            active++;
          
    return active;
}
static int myTurn(Player me, int *call, int *raised)
{
    assert(*raised >= 0);
    printf("Type number for action\n");
    int action = -1;
    printf("To call: %d.\n", *call);
    if (me->stack <= *call) 
    {
        printf("0: CALL, 1: FOLD\n");
        scanf("%d", &action);
        while (action < 0 || action > 1)
        {
            printf("Invalid input, please enter correct input. 1\n");
            scanf("%d", &action);
        }
        if (action == 1) action++;
    }
    
    else if (*raised == 0) printf("0: CALL, 1: BET, 2: FOLD\n");
    else printf("0: CALL, 1: RAISE, 2: FOLD\n");
    if (action == -1) scanf("%d", &action); 
    while (action > 2 || action < 0)
    {
        printf("Invalid input, please enter correct input. 2\n");
        scanf("%d", &action);
    }
        
    if (action == 2) 
    {
        me->action = FOLD;
        printf("You fold.\n");      
        return 0;  
    }
    
    if (action == 0)
    {
        if (me->stack <= *call) 
        {
            me->action = ALL_IN_CALL;
            printf("You all in $%d!\n", me->stack); 
            return me->stack; 
        }
        else 
        {
            me->action = CALL;
            printf("You call $%d!\n", *call);
            me->stack -= *call;
            return *call;
        }
    }
    
    if (action == 1) // Bet or Raise
    {
        int increase = 0;
        if (*raised == 0) // Betting
        {
            if (me->stack <= 2 * *call) // Cannot even min bet
            {
                me->action = ALL_IN_RAISE;
                printf("You all in $%d!\n", me->stack);
                *raised = *call;
                *call = 2 * *call;
                return me->stack; 
            }
            
            printf("Bet how much?\n");
            scanf("%d", &increase);
            while (increase < 2 * *call || increase % BIG_BLIND != 0)
            {
                if (increase < 2 * *call)
                {
                    printf("Please bet at least the minimum of %d.\n", 2 * *call);
                    scanf("%d", &increase);
                }
                
                if (increase % BIG_BLIND != 0)
                {
                    printf("Please bet a multiple of the BB.\n");
                    scanf("%d", &increase);
                }
            }
            
            if (increase >= me->stack)
            {
                me->action = ALL_IN_RAISE;
                printf("You all in $%d!\n", me->stack);                
                *raised = me->stack - *call;
                *call = increase;
                return me->stack; 
            }
            
            else 
            {
                me->action = BET;
                printf("You bet $%d!\n", increase);
                *raised = increase - *call;
                *call = increase;             
                return increase;
            }
        }
        
        else // Raising
        {
            if (me->stack <= *call + 2 * *raised) // Can't even min raise
            {
                me->action = ALL_IN_RAISE;    
                printf("You all in $%d!\n", me->stack);
                *call = increase = *call + 2 * *raised;
                return me->stack;
            }
            
            printf("Raise how much?\n");
            scanf("%d", &increase);
            while (increase < *call + 2 * *raised || increase - *call % BIG_BLIND != 0)
            {
                if (increase < *call + 2 * (*raised))
                {
                    printf("Please raise to a minimum of $%d.\n", *call + 2 * (*raised));
                    scanf("%d", &increase);
                }
                
                if (increase - *call % BIG_BLIND != 0)
                {
                    printf("Please raise to a multiple of the BB.\n");
                    scanf("%d", &increase);
                } 
            }
            
            if (increase >= me->stack)
            {
                me->action = ALL_IN_RAISE;
                printf("You all in $%d!\n", me->stack);  
                *raised = me->stack - *call;
                *call = increase;
                return me->stack; 
            }
            
            else 
            {
                me->action = RAISE;
                printf("You raised to $%d!\n", increase);
                *raised = increase - *call;
                *call = increase;
                return increase;
            }
        }
    }
    printf("FUCKED UP!\n");
    exit(0);
}
