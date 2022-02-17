#ifndef HAND_H
#define HAND_H

#include <iostream>
#include <vector>
#include "card.h"

using namespace std;

class Hand
{
public:
	Hand() : handTotal(0), insured(false), isSplit(false), soft(0), surrendered(false), splitAce(false), bet(0)
	{
	}
	void newHand()
	{
		handTotal = 0;
		hand.clear();
		insured = isSplit = surrendered = splitAce = false;
		soft = 0;
		bet = 0;
	}
	void addCard(char card)
	{
		//add card to hand
		hand.push_back(card);
		handTotal += pointValue(card);

		if(hand.size() == 2 && handTotal == 21 && (!isSplit))
		{
			handTotal = 99;	//blackjack
		}
		//technically this should be a plain "if", but if you have a blackjack, it doesnt matter
		// anyways whether the ace counts as 11. this is mainly a speed optimization
		else if(thisCard(card) == ACE /*&& handTotal < 21*/)
		{
			soft++;
		}
		//if(handTotal == 99)
		//	soft--;
		//if the card is an Ace and the hand is over 21, make it a hard hand
		if(/*(thisCard(card) == ACE || soft)*/soft && handTotal > 21 && handTotal != 99)
		{
			handTotal -= 10;
			soft--;
		}
	}
	int numCards() const
	{
		return hand.size();
	}
	int getTotal() const
	{
		return handTotal;
	}
	bool isSoft() const
	{
		return (soft != 0);
	}

	void insurance()
	{
		insured = true;
	}
	bool isInsured() const
	{
		return insured;
	}

	void surrender()
	{
		surrendered = true;
	}
	bool didSurrender() const
	{
		return surrendered;
	}

	void setBetSize(double bet)
	{
		Hand::bet = bet;
	}
	double getBetSize() const
	{
		return bet;
	}

	bool isSplittable(int numHands) const
	{
		//you cant split different ten cards, such as a K-J. both need to be identical, not that you should split tens anyways
		//also, cannot hit or resplit aces
		if(hand.size() == 2 && thisCard(hand[0]) == thisCard(hand[1]) && numHands < 3 && !splitAce)
			return true;
		else
			return false;
	}
	Hand *split()
	{
		isSplit = true;
		hand.pop_back();	//reduce the two identical cards down to 1
		handTotal = pointValue(hand[0]);	//adjust the hand value
		if(thisCard(hand[0]) == ACE)
		{
			splitAce = true;
			//soft = 1;	//a pair of aces already should have the soft count at 1
		}

		return new Hand(*this);	//copy the hand and return a pointer. dont need a copy constructor; shallow copy
	}
	bool isSplitAce() const
	{
		return splitAce;
	}

	void printHand() const
	{
		for(unsigned int i=0;i<hand.size();i++)
		{
			printName(hand[i]);
		}
	}

private:
	vector<char> hand;
	int handTotal;
	bool insured, isSplit, surrendered, splitAce;
	int soft;
	double bet;
};

#endif