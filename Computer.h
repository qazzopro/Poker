// An AI for playing poker
#ifndef COMPUTER_H
#define COMPUTER_H

#include "Game.h"

// Calculate Computer's turn
void compTurn(Player cpu, Streets street, int call, int button);

bool compOpen(Player cpu, int position);



#endif
