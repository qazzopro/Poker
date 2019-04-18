#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Player.h"

int myTurn(Player me, int *call, int *raised)
{
    assert(*raised >= 0);
    printf("Type number for action\n");
    int action = -1;
    printf("To call: %d.\n", *call - me->placed[0]);
    if (me->stack <= *call - me->placed[0]) 
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
        if (me->stack <= *call - me->placed[0]) 
        {
            me->action = ALL_IN_CALL;
            printf("You all in $%d!\n", me->stack);
            me->placed[0] = me->stack;  
            return me->stack; 
        }
        else 
        {
            me->action = CALL;
            printf("You call $%d!\n", *call);
            me->stack -= *call - me->placed[0];
            me->placed[0] = *call;
            return *call;
        }
    }
    
    if (action == 1) // Bet or Raise
    {
        int increase = 0;
        if (*raised == 0) // Betting
        {
            if (me->stack <=  *call - me->placed[0]) // Cannot even min bet
            {
                me->action = ALL_IN_RAISE;
                printf("You all in $%d!\n", me->stack);
                *raised = *call;
                *call *= 2;
                me->placed[0] = me->stack; 
                return me->stack; 
            }
            
            printf("Bet to how much?\n");
            scanf("%d", &increase);
            while (increase < BIG_BLIND * 2 || increase % BIG_BLIND != 0)
            {
                if (increase < BIG_BLIND * 2)
                {
                    printf("Please bet at least the minimum of %d.\n", 2 * BIG_BLIND);
                    scanf("%d", &increase);
                }
                
                if (increase % BIG_BLIND != 0)
                {
                    printf("Please bet a multiple of the BB.\n");
                    scanf("%d", &increase);
                }
            }
            
            if (increase >= me->stack + me->placed[0])
            {
                me->action = ALL_IN_RAISE;
                increase = me->stack + me->placed[0];
                if ((increase - *call) % *call != 0) 
                    increase += *raised - ((increase - *call) % *call);
                printf("You all in $%d!\n", me->stack);                
                *raised = increase - *call;
                me->placed[0] += me->stack;
                *call = increase;
                return me->stack; 
            }
            
            else 
            {
                me->action = BET;
                printf("You bet $%d!\n", increase);
                *raised = increase - *call;
                *call = increase;
                me->placed[0] = increase;             
                return me->placed[0];
            }
        }
        
        else // Raising
        {
            if (me->stack <= *call - me->placed[0] + *raised) // Can't even min raise
            {
                me->action = ALL_IN_RAISE;    
                printf("You all in $%d!\n", me->stack);
                *call += *raised;
                *raised *= 2;
                me->placed[0] += me->stack;
                return me->stack;
            }
            
            printf("Raise to how much?\n");
            scanf("%d", &increase);
            while (increase < *call - me->placed[0] + *raised  || (increase - *call) % *raised != 0)
            {
                if (increase < *call + 2 * (*raised))
                {
                    printf("Please raise to a minimum of $%d.\n", *call + 2 * (*raised));
                    scanf("%d", &increase);
                }
                
                if ((increase - *call) % *raised != 0)
                {
                    printf("Please raise to a multiple of the last bet/raise.\n");
                    scanf("%d", &increase);
                } 
            }
            
            if (increase >= me->stack + me->placed[0])
            {
                me->action = ALL_IN_RAISE;
                printf("You all in $%d!\n", me->stack);                 
                increase = me->stack + me->placed[0];
                if ((increase - *call) % *raised != 0)
                    increase += *raised - ((increase - *call) % *raised);
                *raised = increase - *call;
                *call = increase;
                me->placed[0] += me->stack;
                me->stack = 0;
                return me->stack; 
            }
            
            else 
            {
                me->action = RAISE;
                printf("You raised to $%d!\n", increase);
                *raised = increase - *call;
                *call = increase;
                me->placed[0] = increase;
                return increase;
            }
        }
    }
}
