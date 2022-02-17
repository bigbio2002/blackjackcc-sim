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
		MT[0] = seed;
		static int i;
		for(i=1;i<624;i++)
		{
			MT[i] = 1812433253u * ( MT[i-1] ^ (MT[i-1]>>30) ) + i;
		}
	}
	unsigned int rand()
	{
		if(index == 0)
			generateNumbers();

		static unsigned int y;
		y = MT[index];
		y = y^(y>>11);
		y = y^((y<<7) & 2636928640u);
		y = y^((y<<15) & 4022730752u);
		y = y^(y>>18);

		index = (index+1)%624;
		return y;
	}
	void generateNumbers()
	{
		static int i;
		for(i=0;i<624;i++)
		{
			//-2147483648 = 32nd bit of a 32 bit integer
			//2147483647 = bits 1-31 of an int
			static unsigned int y;
			y = (MT[i] & 0x80000000) + ((MT[i+1]%624) & 0x7FFFFFFF);
			MT[i] = MT[(i+397)%624] ^ (y>>1);

			//if(y%2 == 1)	//if number is odd
			if(y & 0x00000001)
			{
				MT[i] = MT[i]^2567483615u;
			}
		}
	}
private:
	int index;
	unsigned int MT[624];
};

#endif