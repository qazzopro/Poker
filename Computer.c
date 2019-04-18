// Implementation for AI 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Computer.h"

int compTurn(Player cpu, Streets street, int *call, int *raised, int position, int pot)
{
    /*if (street == PREFLOP) 
    
        // If planning on opening
        if (*raised == 0 && pot == BIG_BLIND + SMALL_BLIND) 
        {
            
            
        }*/
    
    if (*raised == 0) 
    {        
        cpu->action = ALL_IN_RAISE;
        int increase = cpu->stack + cpu->placed[0];
        if ((increase - *call) % *call != 0) 
            increase += *raised - ((increase - *call) % *call);
        printf("He alls in $%d!\n", cpu->stack);
        *raised = increase - *call;
        cpu->placed[0] += cpu->stack;
        *call = increase;
        return cpu->stack;
    }
    
    else 
    {
        cpu->action = ALL_IN_RAISE;
        int increase = cpu->stack + cpu->placed[0];
        if ((increase - *call) % *raised != 0)
            increase += *raised - ((increase - *call) % *raised);
        printf("He alls in $%d!\n", cpu->stack);                 
        *raised = increase - *call;
        *call = increase;
        cpu->placed[0] += cpu->stack;
        return cpu->stack; 
    }
    
}

bool compOpen(Player cpu, int position)
{
 //   if (position == UTG) 
    {}
      return false;  
}

