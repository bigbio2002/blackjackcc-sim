#ifndef CARD_H
#define CARD_H

enum cardNames
{
	ACE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	QUEEN,
	KING
};

int thisCard(char c);
void printName(char c);
int pointValue(char c);

#endif