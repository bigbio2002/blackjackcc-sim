#ifndef MERSENNE_H
#define MERSENNE_H

#include <cstdlib>

class Mersenne
{
public:
	Mersenne() : index(625)
	{
	}

	void srand(unsigned int seed)
	{
		index = 624;

		MT[0] = seed;
		for(unsigned int i=1;i<624;i++)
		{
			//MT[i] = lowest w bits of (f * (MT[i-1] xor (MT[i-1] >> (w-2))) + i)
			MT[i] = (1812433253u * (MT[i-1] ^ (MT[i-1] >> 30)) + i);
		}
	}

	unsigned int rand(void)
	{
		if(index >= 624)
		{
			if(index > 624)
				exit(1);	//generator was not seeded

			generateNumbers();
		}

		unsigned int y;
		y = MT[index];
		y = y^(y >> 11);
		y = y^((y << 7) & 2636928640u);	// (b), 0x9d2c5680
		y = y^((y << 15) & 4022730752u);	// (c), 0xefc60000
		y = y^(y >> 18);

		++index;
		return y;
	}

private:
/*
** MT19937 coefficients **

(w, n, m, r) = (32, 624, 397, 31)
a = 0x9908b0df
(u, d) = (11, 0xffffffff)
(s, b) = (7, 0x9d2c5680)
(t, c) = (15, 0xefc60000)
l = 18

f = 1,812,433,253
*/

/*
** MT19937-64 coefficients **

(w, n, m, r) = (64, 312, 156, 31)
a = 0xb5026f5aa96619e9
(u, d) = (29, 0x5555555555555555)
(s, b) = (17, 0x71d67fffeda60000)
(t, c) = (37, 0xfff7eee000000000)
l = 43

f = 6,364,136,223,846,793,005
*/

	unsigned int index;
	unsigned int MT[624];

	static const unsigned int lower_mask = 0x7FFFFFFF; // 2147483647u
	static const unsigned int upper_mask = 0x80000000; // 2147483648u

	void generateNumbers(void)
	{
		unsigned int x, xA;

		for(unsigned int i=0;i<624;i++)
		{
			//-2147483648 = 32nd bit of a 32 bit integer
			//2147483647 = bits 1-31 of an int

			x = (MT[i] & upper_mask) | (MT[(i+1) % 624] & lower_mask);
			xA = (x >> 1);

			if(x & 1)	//if number is odd
			{
				xA = xA ^ 2567483615u;	// (a), 0x9908b0df
			}

			MT[i] = MT[(i+397) % 624] ^ xA;
		}

		index = 0;
	}
};

#endif
