/*
First attempts towards using a 'jump-able' PRNG with zignor

xoshiro256+ which is good enough for the 32-bit unsigned integers
and 53-bit mantissa doubles

see: https://prng.di.unimi.it/
*/

#include <stdio.h>

#include "randommw.h"

/*---------------------------------------------------------------- 
 * test & development code
 ----------------------------------------------------------------*/
int main(int argc, char **argv)
{
	uint64_t i;
	uint64_t Ni = 100000000;
	double dsum;
	
	printf("hello, world\n");

	RanSetSeed_xoshiro256p(0x974ef530e0479120); 

	printf("\n");
	
	for (i = 0; i < 20; i++)
		printf("%u\n", IRan_xoshiro256p());

	printf("\n");

	for (i = 0; i < 20; i++)
		printf("%f\n", DRan_xoshiro256p());

	printf("\n");
	
	dsum = 0.0;
	for (i = 0; i < Ni; i++)
		dsum += (DRan_xoshiro256p() - 0.5);
	
	printf("dsum = %f\n", dsum/Ni);

	return 0;
}
