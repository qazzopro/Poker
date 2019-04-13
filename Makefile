CC = dcc

all: Poker

Poker: Poker.c Game.o Deck.o 
Game.o: Game.h Game.c
Deck.o: Deck.h Deck.c

.phony: clean
clean:
	rm -rf *.o
