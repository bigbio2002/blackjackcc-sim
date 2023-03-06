#ifndef MERSENNE_H
#define MERSENNE_H

class Mersenne
{
public:
	Mersenne() : index(0)
	{
	}
	void srand(unsigned int seed)
	{
index = 624;

		MT[0] = seed;
		for(unsigned int i=1;i<624;i++)
		{
			MT[i] = (1812433253u * (MT[i-1] >> 30)) + i; //TODO: verify order of operations
		}
	}
	unsigned int rand()
	{
		if(index >= 624)
		{
			if(index > 624)
				exit(1);

			generateNumbers();
		}

		unsigned int y;
		y = MT[index];
		y = y^(y >> 11);
		y = y^((y << 7) & 2636928640u);
		y = y^((y << 15) & 4022730752u);
		y = y^(y >> 18);

		index = ++index;
		return y;
	}
	void generateNumbers()
	{
		for(unsigned int i=0;i<624;i++)
		{
			//-2147483648 = 32nd bit of a 32 bit integer
			//2147483647 = bits 1-31 of an int
			unsigned int x, xA;

			x = (MT[i] & upper_mask) | (MT[(i+1) % 624] & lower_mask);
			xA = (x >> 1);

			if(x & 1)	//if number is odd
			{
				xA = (xA ^ 2567483615u);
			}

			MT[i] = (MT[(i+397) % 624] ^ xA);
		}

	index = 0;
	}

private:
/*
MT19937 coefficients:

(w, n, m, r) = (32, 624, 397, 31)
a = 0x9908b0df
(u, d) = (11, 0xffffffff)
(s, b) = (7, 0x9d2c5680)
(t, c) = (15, 0xefc60000)
l = 18

f = 1,812,433,253
*/

/*
MT19937-64 coefficients:

TODO: fill in
*/

	unsigned int index;
	unsigned int MT[624];

	static const unsigned int lower_mask = 0x7FFFFFFF; // 2147483647u
	static const unsigned int upper_mask = 0x80000000; // 2147483648u

	void generateNumbers();
};

#endif
