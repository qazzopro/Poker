CC = dcc -g

all: Poker

Poker: Poker.c Game.o Deck.o 
Deck.o: Deck.h Deck.c
Game.o: Game.h Game.c

.phony: clean
clean:
	rm -rf *.o
