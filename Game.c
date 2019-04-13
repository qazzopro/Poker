#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "Game.h"

// Auxillary Functions
int isFlush();
int isStraight();
int isQuads();
int isTrips();
int *isTwoPair();

Game newGame() 
{
    Game Poker = malloc(sizeof(struct _game));

    Poker->deck = shuffleDeck();

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        Poker->players[i] = malloc(sizeof(struct _player));
        for (int j = 0; j <= 4; j++) Poker->players[i]->best[j] = -1;
    }
    
    
    return Poker;
}

void dealCards(Player players[], Deck deck)
{
    int cardnum = 0;
    
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        for (int j = 0; j < HAND_SIZE; j++)
        {
            players[i]->hand[j] = deck[cardnum];
            cardnum++;
        }
    }
}

int numOfCards[13];
int suitOfCards[4];

void calculateHand(Player player, int deck[])
{
    memset(numOfCards, 0, sizeof(numOfCards));
    memset(suitOfCards, 0, sizeof(suitOfCards));
    int *cards = malloc(8 * sizeof(int));
    for (int card = 0; card <= 6; card++)
    {
        if (card <= 1) 
            cards[card] = player->hand[card];
        else 
            cards[card] = deck[10 + card];
            
        numOfCards[cards[card] % 13]++;
        suitOfCards[cards[card] / 13]++;
    }
    cards[7] = '\0';
    printf("\n");
     
    int flush = isFlush();
    int straight = isStraight();
    int quads = isQuads();
    int trips = isTrips();
    int *twopair = isTwoPair();
    //for (int i = 0; i <= 12; i++) printf("%d\n", numOfCards[i]); // For debugging
    
    if (flush != -1 && straight != -1 && flush == straight) 
    {
        printf("%c Straight Flush!\n", values[flush]);
        player->rank = 8;
        if (flush != 3) for (int i = 0; i <= 4; i++) player->best[i] = flush - i;
        else { for (int i = 0; i <= 3; i++) player->best[i] = flush - i; player->best[4] = 12;}
    }
    else if (quads != -1) 
    {
        printf("Quad %cs!\n", values[quads]);
        player->rank = 7;
        for (int i = 0; i <= 3; i++) player->best[i] = quads;
        for (int i = 0; i <= 12; i++) { if (numOfCards[i] >= 1 && i != quads) player->best[4] = i; }
    }
    else if (trips != -1 && twopair[0] != -1) 
    {
        printf("Full house %c!\n", values[trips]);
        player->rank = 6;
        player->best[0] = trips; player->best[1] = trips; player->best[2] = trips;
        player->best[3] = twopair[0]; player->best[4] = twopair[0];
    }
    else if (flush != -1)
    { 
        printf("%c Flush!\n", suits[flush]);
        player->rank = 5;
        int counter = 0;
        for (int i = 12; i >= 0 && counter <= 4; i--) 
        {
            if (numOfCards[i] >= 1)
            {
                for (int i = 0; i <= 6; i++)
                {
                    if (cards[i] % 13 == i && cards[i] / 13 == flush)
                    {
                        player->best[counter] = numOfCards[i];
                        counter++;
                        break;
                    }
                }
            }  
        }  
    }
    else if (straight != -1) 
    {
        printf("%c high straight!\n", values[straight]);
        player->rank = 4;
        for (int i = 0; i <= 4; i++) player->best[i] = straight - i;
    }
    else if (trips != -1) 
    {
        printf("Trip %cs!\n", values[trips]);
        player->rank = 3;
        player->best[0] = trips; player->best[1] = trips; player->best[2] = trips;
        int counter = 3;
        for (int i = 12; i >= 0; i--)
        {
            if (numOfCards[i] != 0 && counter <= 4)
            {
                player->best[counter] = i;
                counter++;
            }
        }
        
    }
    else if (twopair[1] != -1)
    { 
        printf("Two Pair of %cs and %cs!\n", values[twopair[0]], values[twopair[1]]);
        player->rank = 2;
        player->best[0] = twopair[0]; player->best[1] = twopair[0];
        player->best[2] = twopair[1]; player->best[3] = twopair[1];
        int high = 0;
        for (int i = 0; i <= 6; i++) 
        {
            if (cards[i] % 13 != trips && cards[i] % 13 > high % 13) high = cards[i];
        }
        player->best[4] = high % 13;
    }
    else if (twopair[0] != -1) 
    {
        printf("Pair of %cs!\n", values[twopair[0]]);
        player->rank = 1;
        player->best[0] = twopair[0]; player->best[1] = twopair[0];
        int counter = 2;
        for (int i = 12; i >= 0; i--) 
        {
            if (i != twopair[0] && numOfCards[i] >= 1 && counter <= 4) 
            {
                player->best[counter] = i;
                counter++;
            }
        }
    } 
    else 
    {
        player->rank = 0;
        int counter = 0;
        for (int i = 12; i >= 0; i--) 
        {
            if (numOfCards[i] != 0 && counter <= 4) 
            {
                player->best[counter] = i;
                counter++;
            }
        }
        printf("%c High!\n", values[player->best[0]]);
    }
}

void calculateWinner(Player players[], int community[5]) 
{
    int best = 0;
    int tie[MAX_PLAYERS] = {0};
    int tiebreaker = 0;
    for (int i = 1; i < MAX_PLAYERS; i++) 
    {
        if (players[i]->rank > players[best]->rank) 
        {
            best = i;
            memset(tie, 0, sizeof(tie));
            tiebreaker = 0;
        }
        else if (players[i]->rank == players[best]->rank)
        {
            for (int j = 0; j <= 4 && tiebreaker == 0; j++) 
            {
                if (players[i]->best[j] > players[best]->best[j])
                {
                    tiebreaker = 1;
                    best = i;
                    memset(tie, 0, sizeof(tie));
                }
                else if (players[i]->best[j] < players[best]->best[j]) tiebreaker = 1;
            }
            if (tiebreaker == 0) { tie[best] = 1; tie[i] = 1;}
        }
    }
    
    printf("Player ");
    tiebreaker = 0;
    for (int i = 0; i < MAX_PLAYERS; i++) 
    { 
        if (tie[i] != 0) 
        {
            if (i == best) printf("%d ", i + 1);
            else printf("and %d ", i + 1);
            tiebreaker = 1;
        }
    }
    if (tiebreaker == 0) printf("%d won!\n", best + 1);            
    else printf("won!\n");           
}

int isFlush() 
{
    for (int i = 0; i <= 3; i++)
    {
        if (suitOfCards[i] >= 5) return i;
    }
    return -1;
}

int isStraight()
{
    for (int i = 12; i >= 4; i--) 
    {
        if (numOfCards[i] >= 1 && numOfCards[i-1] >= 1 && numOfCards[i-2] >= 1
            && numOfCards[i-3] >= 1 && numOfCards[i-4] >= 1) return i;
    }
   
    if (numOfCards[0] >= 1 && numOfCards[1] >= 1 && numOfCards[2] >= 1 && numOfCards[3] >= 1 && numOfCards[12] >= 1)
        return 3;
    
    return -1;
} 

int isQuads()
{
    for (int i = 12; i >= 0; i--) 
    {
        if (numOfCards[i] == 4) 
            return i;
    }
    
    return -1;
}

int isTrips()
{
    for (int i = 12; i >= 0; i--)
    {
        if (numOfCards[i] == 3)
            return i;
    }
    
    return -1;
}

int *isTwoPair()
{
    int *pairs = malloc(sizeof(int) * 3);
    pairs[0] = -1;
    pairs[1] = -1;
    pairs[2] = '\0';
    for (int i = 12; i >= 0; i--) 
    {
        if (numOfCards[i] == 2)
        {
            if (pairs[0] == -1) pairs[0] = i; 
            else if (pairs[1] == -1) { pairs[1] = i; return pairs; }
        }
    }
    
    return pairs;
} 

void destroyGame(Game game) 
{
    for (int player = 0; player < MAX_PLAYERS; player++)
    {
        free(game->players[player]);
    }
    free(game->deck);
    free(game);
}


