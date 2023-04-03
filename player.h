#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "hand.h"
#include "playerAI.h"

class Player
{
public:
	Player() : currHand(0), cash(0), isPlaying(true)
	{
		hands.push_back(new Hand);
	}

	~Player()
	{
		for(unsigned int i=0;i<hands.size();i++)
		{
			if(hands[i])
				delete hands[i];	//avoid memory leaks; this causes a crash
		}
	}

	void newRound()
	{
		Hand *temp;

		for(unsigned int i=1;i<hands.size();i++)
		{
			delete hands[i];	//avoid memory leaks
		}
		temp = hands[0];
		temp->newHand();

		hands.clear();
		hands.push_back(temp);
		currHand = 0;	//use zero-indexed count for hands; this means that the player has one hand
	} /* end newRound() */

	void receiveWinnings(double win)
	{
		cash+=win;
	}
	bool setBet(double bet)
	{
		if(bet <= cash)
		{
			cash-=bet;
			hands[currHand]->setBetSize(bet);
			return true;
		}
		else
			return false;
	}
	double getHandValue() const
	{
		return hands[currHand]->getBetSize();
	}
	double getBankroll() const
	{
		return cash;
	}

	void getCard(char card)
	{
		hands[currHand]->addCard(card);
	}
	void printHand() const
	{
		hands[currHand]->printHand();
	}
	int getTotal() const
	{
		return hands[currHand]->getTotal();
	}

	int numOfHands() const
	{
		return hands.size();
	}
	bool nextHand()
	{
		if(currHand < hands.size()-1)
		{
			currHand++;
			return true;
		}

		return false;
	}
	void resetNextHand()
	{
		currHand = 0;
	}
	int handNum() const
	{
		return currHand;
	}
	Hand *ptrCurrHand() const
	{
		return hands[currHand];
	}

	bool canInsure() const
	{
		if((hands[currHand]->getBetSize() / 2) <= cash)
		{
			return true;
		}
		else
			return false;
	}
	void insurance()
	{
		cash -= hands[currHand]->getBetSize()/2;
		hands[currHand]->insurance();
	}
	bool checkInsurance() const
	{
		return hands[currHand]->isInsured();
	}

	bool canDoubleDown() const
	{
		if(hands[currHand]->getBetSize() <= cash && hands[currHand]->numCards() == 2)
		{
			return true;
		}
		else
			return false;
	}
	void doubleDown()
	{
		cash -= hands[currHand]->getBetSize();
		hands[currHand]->setBetSize( hands[currHand]->getBetSize() * 2 );
	}

	bool canSurrender() const
	{
		if(hands[currHand]->numCards() == 2)
		{
			return true;
		}
		else
			return false;
	}
	void surrender()
	{
		hands[currHand]->surrender();
	}
	bool didSurrender() const
	{
		return hands[currHand]->didSurrender();
	}

	bool canSplit() const
	{
		if(hands[currHand]->isSplittable(hands.size()) && hands[currHand]->getBetSize() <= cash)
			return true;
		else
			return false;
	}
	void split(char card1, char card2)
	{
		cash-=hands[currHand]->getBetSize();

		Hand *tempHand = hands[currHand]->split();
		hands[currHand]->addCard(card1);
		tempHand->addCard(card2);
		hands.push_back(tempHand);

		//delete hands[currHand];	//replace the old hand with a new one that has a single card
		//hands[currHand] = new Hand;
		//hands[currHand]->addCard(temp);
		//hands[currHand]->wasSplit();

		//Hand *tempHand;
		//tempHand = new Hand;	//add a new hand with the other card from the split hand
		//tempHand->addCard(temp);
		//tempHand->wasSplit();
		//hands.push_back(tempHand);
	}
	bool isSplitAce() const
	{
		return hands[currHand]->isSplitAce();
	}

private:
	std::vector<Hand*> hands;
	int currHand;
	double cash;
	bool isPlaying;
};

#endif
