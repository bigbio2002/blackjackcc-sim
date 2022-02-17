#ifndef HILOWCOUNT_H
#define HILOWCOUNT_H

#include "card.h"

class HiLowCount
{
public:
	HiLowCount() : runningCount(0)
	{
	}

	void count(char card)
	{
		switch(thisCard(card))
		{
		case ACE:
		case KING:
		case QUEEN:
		case JACK:
		case TEN:
			runningCount--;
			break;

		case TWO:
		case THREE:
		case FOUR:
		case FIVE:
		case SIX:
			runningCount++;
			break;

		default:
			break;
		}
	}

	int getCount() const
	{
		return runningCount;
	}

	void resetCount()
	{
		runningCount = 0;
	}

private:
	int runningCount;
};

#endif