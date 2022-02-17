#include <iostream>
#include <cstdlib>
#include "card.h"

using namespace std;

int thisCard(char c)
{
	return (c%13);
}

void printName(char c)
{
	static char temp;

	temp = (c%13);

	switch(temp)
	{
	case ACE:
		cout << "A";
		break;
	case TWO:
		cout << "2";
		break;
	case THREE:
		cout << "3";
		break;
	case FOUR:
		cout << "4";
		break;
	case FIVE:
		cout << "5";
		break;
	case SIX:
		cout << "6";
		break;
	case SEVEN:
		cout << "7";
		break;
	case EIGHT:
		cout << "8";
		break;
	case NINE:
		cout << "9";
		break;
	case TEN:
		cout << "T";
		break;
	case JACK:
		cout << "J";
		break;
	case QUEEN:
		cout << "Q";
		break;
	case KING:
		cout << "K";
		break;
	default:
		exit(20);
	}
}

int pointValue(char c)
{
	static char temp;

	temp = (c%13);

	switch(temp)
	{
	case ACE:
		return 11;
	case TWO:
		return 2;
	case THREE:
		return 3;
	case FOUR:
		return 4;
	case FIVE:
		return 5;
	case SIX:
		return 6;
	case SEVEN:
		return 7;
	case EIGHT:
		return 8;
	case NINE:
		return 9;
	case TEN:
	case JACK:
	case QUEEN:
	case KING:
		return 10;
	default:
		exit(21);
	}
}