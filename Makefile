CC = gcc -g

all: Poker

Poker: Poker.c Computer.o Game.o Deck.o 
Computer.o: Computer.h Computer.c
Game.o: Game.h Game.c
Deck.o: Deck.h Deck.c



.phony: clean
clean:
	rm -rf *.o
