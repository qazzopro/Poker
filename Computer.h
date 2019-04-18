// An AI for playing poker
#ifndef COMPUTER_H
#define COMPUTER_H

#include "Game.h"

// Calculate Computer's turn
int compTurn(Player cpu, Streets street, int *call, int *raised, int position, int pot);

bool compOpen(Player cpu, int position);



#endif
