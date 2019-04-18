CC = gcc -g

all: Poker

Poker: Poker.c Player.o Computer.o Game.o Deck.o 
Player.o: Player.h Player.c
Computer.o: Computer.h Computer.c
Game.o: Game.h Game.c
Deck.o: Deck.h Deck.c



.phony: clean
clean:
	rm -rf *.o
