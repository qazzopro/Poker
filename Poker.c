#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Deck.h"
#include "Game.h"

int main (void) 
{
    Game Poker = newGame();
    printf("\n");
    printf("Dealing Cards\n");
    printf("\n\n");
    dealCards(Poker->players, Poker->deck);
    
    printf("The Flop\n");
    printCommunity(Poker->deck, FLOP);
    printf("\n\n");
    
    printf("The Turn\n");
    printCommunity(Poker->deck, TURN);
    printf("\n\n");
    
    printf("The River\n");
    printCommunity(Poker->deck, RIVER);
    printf("\n\n");
    
    for (int i = 0; i < MAX_PLAYERS; i++)
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
    
    int community[5];
    for (int i = 0; i <= 4; i++) community[i] = Poker->deck[i+12];
    calculateWinner(Poker->players, community);
    
    destroyGame(Poker);
    
    return 0; 
}



