#ifndef SHOE_H
#define SHOE_H

#include <iostream>
#include <ctime>
#include <list>
#include "mersenne.h"
#include "card.h"
#include "hilowcount.h"

using namespace std;

class Shoe
{
public:
	Shoe(unsigned int numDecks) : numDecks(numDecks), numCards(numDecks*52), cardsDealt(0)
	{
		cards = new char[numCards];

		for(unsigned int i=0;i<numCards;i++)
		{
			cards[i] = (i%52);
		}

		//set up the RNG
		randgen.srand((unsigned)time(NULL));
		for(unsigned int i=0;i<1000;i++)
		{
			//avoid zero excess initial state
			randgen.rand();
		}
	}
	~Shoe()
	{
		delete cards;
	}

	void shuffle()
	{
		//KFY shuffle
		for(unsigned int i = numCards-1; i > 0; i--)
		{
			char temp;
			int n;

			n = randgen.rand()%numCards;

			if(n != i)
			{
				temp = cards[i];
				cards[i] = cards[n];
				cards[n] = temp;
			}
		}

		cardsDealt = 0;
		count.resetCount();
	}

	/* this function "removes" the cards in play from the shuffled deck so
	 its as if only the cards in the discard pile were shuffled. it scans the
	 freshly shuffled shoe for instances of cards that are on the table, and
	 moves them before the index cardsDealt. there is no actual discard pile that
	 is maintained, and therefore you cannot trace back to see what order the
	 cards were dealt in previous hands */
	void adjustShuffle(list<char> &cardsInPlay)
	{
		char temp;

		cardsDealt = cardsInPlay.size();

		unsigned int i, j;
		for(i=0;i<cardsDealt;i++)
		{
			for(j=0;j<numCards;j++)
			{
				if(cards[j] == cardsInPlay.back())
				{
					temp = cards[j];
					cards[j] = cards[i];
					cards[i] = temp;

					cardsInPlay.pop_back();
					break;
				}
			}
		}
	}

	char drawCard()
	{
		//TODO - shuffle instead of terminating
		if(cardsDealt == numCards)
			exit(187);	//deck is empty

		count.count(cards[cardsDealt]);
		cardsDealt++;
		return cards[cardsDealt-1];
	}

	int trueCount() const
	{
		//upcast these to double; still may be errors
		return static_cast<double>(count.getCount()) / (static_cast<double>((numCards - cardsDealt)) / 52.0);
	}

	int runningCount() const
	{
		return count.getCount();
	}

	unsigned int totalNumCards() const
	{
		return numCards;
	}

	unsigned int numCardsDealt() const
	{
		return cardsDealt;
	}

	void printDebug() const
	{
		for(unsigned int i=0;i<numCards;i++)
		{
			cout << (int)cards[i] << " ";
		}
	}
	void printNamesDebug() const
	{
		for(unsigned int i=0;i<numCards;i++)
		{
			printName(cards[i]);
			cout << " ";
		}
	}
	//how often does the first card appear at the start of the deck?
	void debugStatCount(int iterations = 10000000)
	{
		int count = 0;
		for(int i=0;i<iterations;i++)
		{
			shuffle();
			if(cards[0] == 1)
			{
				count++;
			}
		}
		double total = (double)count/iterations*52;
		cout << total << endl;
	}

private:
	Mersenne randgen;
	HiLowCount count;
	unsigned int numDecks;
	unsigned int numCards;
	char *cards;
	unsigned int cardsDealt;
};

#endif
