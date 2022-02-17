#include "blackjack.h"
#include "shoe.h"

#include <iostream>
#include <ctime>
//#include <cstdlib>

using namespace std;

int main()
{
	Blackjack table(6);

	table.play();

	return 0;
}