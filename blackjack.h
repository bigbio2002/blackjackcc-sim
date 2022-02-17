#ifndef BLACKJACK_H
#define BLACKJACK_H

enum states
{
	BUYIN,
	BET,
	DRAW,
	DEALERCHECK,
	PLAY,
	INSURANCE,
	NEXTPLAYER,
	DEALERPLAY
};

#include <iostream>
#include <string>
#include <strstream>	//deprecated in mingw???
#include <cstdlib>
#include "shoe.h"
#include "hand.h"
#include "player.h"
#include "playerAI.h"

using namespace std;

class Blackjack
{
public:
	Blackjack(int numDecks) : numPlayers(1), gameState(BUYIN), quit(false), autoplay(false), numRoundsToPlay(1000000), roundsPlayedCounter(0)
	{
		shoe = new Shoe(numDecks);
		cutCard = 52*numDecks*0.75;
		shoe->shuffle();
	}
	~Blackjack()
	{
		delete shoe;
	}
	void play()
	{
		while(!quit)
		{
			switch(gameState)
			{
			case BUYIN:
				if(autoplay)
				{
					input = bot.playHand(NULL, 0, 0, 0, BUYIN);
				}
				else
				{
					status_output = "How much money do you wish to play with? $";
					drawTable();
					cin >> input;
				}
				num_input = atoi(input.c_str());

				players[0].receiveWinnings(num_input);
				gameState = BET;
				break;

			case BET:
				if(autoplay)
				{
					input = bot.playHand(NULL, 0, 0, shoe->trueCount(), BET);
				}
				else
				{
					status_output = "Enter your bet, or press B to add chips. Press Q to quit: $";
					drawTable();
					cin >> input;
				}
				num_input = atoi(input.c_str());

				if(input == "b" || input == "B")
				{
					gameState = BUYIN;
					break;
				}
				else if(input == "q" || input == "Q")
				{
					quit = true;
					break;
				}
				else if(input == "autoplay")
				{
					autoplay = true;
					break;
				}
				else if(!players[0].setBet(num_input))	//if setBet() succeeds, skip to next else
				{
					status_output = "You do not have enough money. ";
					drawTable();
					system("pause");
					break;
				}
				else
				{
					gameState = DRAW;
					break;
				}
				//break;

			case DRAW:
				if(shoe->numCardsDealt() > cutCard)
				{
					shoe->shuffle();
					if(!autoplay)
					{
						status_output = "The deck was shuffled. ";
						drawTable();
						system("pause");
					}
				}

				dealer.getCard(shoe->drawCard());	//give the dealer his faceup card
				holeCard = shoe->drawCard();

				players[0].getCard(shoe->drawCard());
				players[0].getCard(shoe->drawCard());	//draw 2 cards

				if(dealer.getTotal() == pointValue(ACE))
					gameState = INSURANCE;
				else if(dealer.getTotal() == pointValue(TEN))
					gameState = DEALERCHECK;
				else
					gameState = PLAY;

				break;

			case INSURANCE:
				if(autoplay)
				{
					input = bot.playHand(NULL, 0, 0, shoe->trueCount(), INSURANCE);
				}
				else
				{
					status_output = "Insurance? ";
					drawTable();
					cin >> input;
				}

				if(input == "n" || input == "N")
				{
					gameState = DEALERCHECK;
				}
				else if(input == "y" || input == "Y")
				{
					if(!players[0].canInsure())
					{
						if(!autoplay)
						{
							status_output = "Cannot insure. ";
							drawTable();
							system("pause");
						}
					}
					else
					{
						players[0].insurance();
					}
				}

				break;

			case DEALERCHECK:
				if(dealer.getTotal() + pointValue(holeCard) == 21)	//dealer has a blackjack
				{
					gameState = DEALERPLAY;	//skip to the end of the hand
				}
				else
				{
					gameState = PLAY;
				}
				break;

			case PLAY:
				do
				{
					while(players[0].getTotal() < 21)
					{
						if(players[0].isSplitAce())	//cannot hit split aces
							break;

						if(autoplay)
						{
							input = bot.playHand(players[0].ptrCurrHand(), dealer.getTotal(), players[0].numOfHands(), shoe->trueCount(), PLAY);
						}
						else
						{
							status_output = "Hit, Stand, sPlit, Double, or suRrender? ";
							drawTable();
							cin >> input;
						}

						/* HIT */
						if(input == "h" || input == "H")
						{
							players[0].getCard(shoe->drawCard());
						}

						/* STAND */
						else if(input == "s" || input == "S")
						{
							break;
						}

						/* SPLIT */
						else if(input == "p" || input == "P")
						{
							if(!players[0].canSplit())
							{
								if(!autoplay)
								{
									status_output = "Cannot split. ";
									drawTable();
									system("pause");
								}
							}
							else
							{
								players[0].split(shoe->drawCard(), shoe->drawCard());
							}
						}

						/* DOUBLE DOWN */
						else if(input == "d" || input == "D")
						{
							if(!players[0].canDoubleDown())
							{
								if(!autoplay)
								{
									status_output = "Cannot double down. ";
									drawTable();
									system("pause");
								}
							}
							else
							{
								players[0].doubleDown();
								players[0].getCard(shoe->drawCard());
								break;
							}
						}

						/* SURRENDER */
						else if(input == "r" || input == "R")
						{
							if(!players[0].canSurrender())
							{
								if(!autoplay)
								{
									status_output = "Too late to surrender. ";
									drawTable();
									system("pause");
								}
							}
							else
							{
								players[0].surrender();
								players[0].receiveWinnings(players[0].getHandValue() / 2);	//receive surrender winnings immediately
								break;
							}
						}
					}
				}
				while(players[0].nextHand());	//do all the split hands

				gameState = DEALERPLAY;
				break;

			case DEALERPLAY:
				dealer.getCard(holeCard);
				while(dealer.getTotal() < 17)
				{
					dealer.getCard(shoe->drawCard());
				}

				/* NEED TO REDO GAME OUTCOME LOGIC */
				/*if(dealer.getTotal == 99)
				{
					if(players[0].getTotal() == 99)
					{
					}
					else if(players[0].getTotal()
				}
				else if(dealer.getTotal() > 21)
				{
				}
				else if(dealer.getTotal() < 21)
				{
				}*/

				players[0].resetNextHand();

				do
				{
					if(players[0].didSurrender())
					{
						if(!autoplay)
						{
							status_output = "Surrendered. ";
							drawTable();
						}
					}
					else if(players[0].getTotal() == 99 && dealer.getTotal() != 99)
					{
						players[0].receiveWinnings(players[0].getHandValue() * 2.5);
						if(!autoplay)
						{
							status_output = "Blackjack! ";
							drawTable();
						}
					}
					else if(players[0].getTotal() > 21 && players[0].getTotal() != 99)
					{
						if(!autoplay)
						{
							//bust
							status_output = "Bust. ";
							drawTable();
						}
					}
					else if(dealer.getTotal() > 21 && dealer.getTotal() != 99)
					{
						players[0].receiveWinnings(players[0].getHandValue() * 2);
						if(!autoplay)
						{
							status_output = "You win. ";
							drawTable();
						}
					}
					else if(dealer.getTotal() > players[0].getTotal())
					{
						if(!autoplay)
						{
							status_output = "You lose. ";
							drawTable();
						}
					}
					else if(dealer.getTotal() < players[0].getTotal())
					{
						players[0].receiveWinnings(players[0].getHandValue() * 2);
						if(!autoplay)
						{
							status_output = "You win. ";
							drawTable();
						}
					}
					else if(dealer.getTotal() == players[0].getTotal())
					{
						players[0].receiveWinnings(players[0].getHandValue());
						if(!autoplay)
						{
							status_output = "Push. ";
							drawTable();
						}
					}
					else
					{
						//should never reach here
						exit(99099);
					}

					if(!autoplay)
					{
						system("pause");
					}
				}
				while(players[0].nextHand());

				gameState = BET;
				dealer.newRound();
				players[0].newRound();

				if(autoplay)
				{
					drawTable();
					roundsPlayedCounter++;

					if(roundsPlayedCounter == numRoundsToPlay)
					{
						autoplay = false;
						roundsPlayedCounter = 0;
						cout << numRoundsToPlay << " rounds were played. " << flush;
						system("pause");
					}
				}
				break;
			}
		}
	}
	void drawTable() const
	{
		if(autoplay)
		{
			if(roundsPlayedCounter % 50000 == 0)
				cout << roundsPlayedCounter << "\n";
		}
		else
		{
			system("cls");
			cout << "                                   Blackjack                                    \n\n";	//80 spaces
			cout << "Dealer: ";	dealer.printHand(); cout << "  Total: " << dealer.getTotal()/*((dealer.getTotal()==99) ? ("BJ") : (dealer.getTotal()))*/ << "\n";
			cout << "                                       Deck: "; drawDeck(); cout << '\n';	//39 spaces, can accomodate 8 decks
			for(int i=0;i<numPlayers;i++)
			{
				cout << "Player " << i+1 << ", Hand " << players[i].handNum()+1 << ": "; players[i].printHand(); cout << "  Total: " << players[i].getTotal()/*((players[i].getTotal()==99) ? ("BJ") : (players[i].getTotal()))*/ << "\n";
				cout << "Bankroll: $" << players[i].getBankroll();
			}
			cout << "\n\n" << status_output << flush;
		}
	}
	void drawDeck() const
	{
		int bars, spaces;
		static int fieldsize = shoe->totalNumCards()/13;

		bars = (shoe->totalNumCards() - shoe->numCardsDealt())/13;
		spaces = fieldsize - bars;

		cout << '[';
		int i;
		for(i=0;i<bars;i++)
		{
			cout << '|';
		}
		for(i=0;i<spaces;i++)
		{
			cout << ' ';
		}
		cout << ']';
	}

private:
	Shoe *shoe;
	int numPlayers;
	unsigned int cutCard;

	string input;
	int num_input;

	string status_output;

	Player dealer;
	char holeCard;

	Player players[2];	//todo: adjust this to the max num of players we want
	PlayerAI bot;

	states gameState;
	bool autoplay;
	int numRoundsToPlay, roundsPlayedCounter;
	bool quit;
};

#endif