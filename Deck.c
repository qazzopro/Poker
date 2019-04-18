#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Deck.h"

// Auxillary Functions
void swap(int *a, int *b);

char values[] = {
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    'T',
    'J',
    'Q',
    'K',
    'A'
};

char suits[] = {
    's',
    'h',
    'c',
    'd'
};

Deck shuffleDeck() 
{
    // Creating a deck with deck[i] = i
    Deck deck = malloc(DECK_SIZE * sizeof(int));
    for (int i = 0; i < DECK_SIZE; i++)
    {
        deck[i] = i;
    }
    
    for (int i = DECK_SIZE - 1; i > 0; i--) 
    {
        // Pick a random index from 0..i
        int j = rand() % (i + 1);
        
        // Swap deck[i] with element at a random index
        swap(&deck[i], &deck[j]);
    }
    
    return deck;   
}

void printCard(int Card)
{
    printf("%c%c ", values[Card % 13], suits[Card / 13]); 
}

void printCommunity(int deck[], Streets street)
{
    for (int i = 0; i != 3 + street; i++)
    {
        printCard(deck[12+i]);
    }     
}

// Simple swap
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
