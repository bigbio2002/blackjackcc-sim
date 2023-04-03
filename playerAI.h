#ifndef PLAYERAI_H
#define PLAYERAI_H

/*
0 = stand
1 = hit
2 = double or hit
3 = double or stand
4 = surrender
5 = split
6 = split or hit
*/

//player 9-16 against dealer card
const int DealerS17Multideck_Hard[8][10] =
{
	{1, 2, 2, 2, 2, 1, 1, 1, 1, 1},
	{2, 2, 2, 2, 2, 2, 2, 2, 1, 1},
	{2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
	{1, 1, 0, 0, 0, 1, 1, 1, 1, 1},
	{0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
	{0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
	{0, 0, 0, 0, 0, 1, 1, 1, 4, 1},
	{0, 0, 0, 0, 0, 1, 1, 4, 4, 4}
};

//player soft 13-18 against dealer
const int DealerS17Multideck_Soft[6][10] =
{
	{1, 1, 1, 2, 2, 1, 1, 1, 1, 1},
	{1, 1, 1, 2, 2, 1, 1, 1, 1, 1},
	{1, 1, 2, 2, 2, 1, 1, 1, 1, 1},
	{1, 1, 2, 2, 2, 1, 1, 1, 1, 1},
	{1, 2, 2, 2, 2, 1, 1, 1, 1, 1},
	{0, 3, 3, 3, 3, 0, 0, 1, 1, 1}
};

const int DealerS17Multideck_Split[9][10] = 
{
	{6, 6, 5, 5, 5, 5, 1, 1, 1, 1},
	{6, 6, 5, 5, 5, 5, 1, 1, 1, 1},
	{1, 1, 1, 6, 6, 1, 1, 1, 1, 1},
	{2, 2, 2, 2, 2, 2, 2, 2, 1, 1},	//treat pair of 5s as a hard 10
	{6, 5, 5, 5, 5, 1, 1, 1, 1, 1},
	{5, 5, 5, 5, 5, 5, 1, 1, 1, 1},
	{5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
	{5, 5, 5, 5, 5, 0, 5, 5, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}	//always stand on pair of 10s
};

#include <iostream>
#include <string>
//#include "hilowcount.h"
#include "blackjack.h"

using namespace std;

//extern enum states;	//dont need this?

class PlayerAI
{
public:
	string &playHand(const Hand *hand, int dealerUpcard, int numHands, int count, states state)
	{
		switch(state)
		{
		case BUYIN:
			output = "10000";
			return output;

		case BET:
			if(count <= 0)
				output = "10";
			else if(count == 1)
				output = "25";
			else if(count == 2)
				output = "50";
			else if(count == 3)
				output = "75";
			else if(count >= 4)
				output = "100";

			/* conservative betting */
			/*if(count <= 1)
				output = "10";
			else if(count == 2)
				output = "20";
			else if(count == 3)
				output = "30";
			else if(count >= 4)
				output = "40";*/
			/*else if(count >= 5)
				output = "50";
			else if(count >= 6)
				output = "60";*/

			//output = "10";	//flat betting

			return output;

		case PLAY:
			if(hand->isSplittable(numHands))
			{
				if(hand->getTotal() == 12 && hand->isSoft())	//split your aces. this needs a special case due to the way hand totals are calculated
				{
					output = "p";
				}
				else
				{
					tempindex = DealerS17Multideck_Split[(hand->getTotal()>>1) - 2][dealerUpcard-2];
					action(hand, tempindex);
				}
			}
			else if(hand->isSoft())
			{
				//if(hand->getTotal() == 12)
				//	exit(12);	//should never reach here

				if(hand->getTotal() >= 19)
				{
					output = "s";
				}
				else
				{
					tempindex = DealerS17Multideck_Soft[hand->getTotal()-13][dealerUpcard-2];
					action(hand, tempindex);
				}
			}
			else
			{
				if(hand->getTotal() <= 8)
				{
					output = "h";
				}
				else if(hand->getTotal() >= 17)
				{
					output = "s";
				}
				else
				{
					tempindex = DealerS17Multideck_Hard[hand->getTotal()-9][dealerUpcard-2];
					action(hand, tempindex);
				}
			}
			return output;

		case INSURANCE:
			output = "n";
			return output;

		default:
			//should never reach here
			exit(1007);
		};
	}

private:
	string output;
	int tempindex;

	void action(const Hand *hand, int in)
	{
		if(in == 0)
		{
			output = "s";
		}
		else if(in == 1)
		{
			output = "h";
		}
		else if(in == 2)
		{
			if(hand->numCards() == 2)
				output = "d";
			else
				output = "h";
		}
		else if(in == 3)
		{
			if(hand->numCards() == 2)
				output = "d";
			else
				output = "s";
		}
		else if(in == 4)
		{
			if(hand->numCards() == 2)
				output = "r";
			else
				output = "h";
		}
		else if(in == 5)
		{
			output = "p";
		}
		else if(in == 6)
		{
			output = "p";
		}
	}
};

#endif
