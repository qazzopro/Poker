// Implementation for AI 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Computer.h"

int compTurn(Player cpu, Streets street, int *call, int *raised, int position)
{
    // ALL IN
    
    if (*raised == 0) // BET ALL IN
    {        
        cpu->action = ALL_IN;
        printf("He alls in $%d!\n", cpu->stack);
        *raised = cpu->stack - *call;
        return cpu->stack; 
    }
    
    else 
    {
        cpu->action = ALL_IN;
        printf("He alls in $%d!\n", cpu->stack);
        return cpu->stack;
    }
}

bool compOpen(Player cpu, int position)
{
 //   if (position == UTG) 
    {}
      return false;  
}

