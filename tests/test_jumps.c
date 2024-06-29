/*

test_jumps.c

Test the speed of the long jump capability of the Xoshiro256+ PRNG.

This uses multiple calls to xoshiro256p_long_jump() internally.

   xoshiro256p_long_jump() is the long-jump function for the generator. 
   It is equivalent to
   2^192 calls to next(); it can be used to generate 2^64 starting points,
   from each of which jump() will generate 2^64 non-overlapping
   subsequences for parallel distributed computations.


Typical speed test results on Intel Core i7 (Windows 11, gcc, w64devkit, 2024)

- 2000000 long jumps of Xoshiro256+ in  0.94 s
- 50000000 long jumps of Xoshiro256+ in 23.36 s
- 200000000 long jumps of Xoshiro256+ in  1'33.57
- 1000000000 long jumps of Xoshiro256+ in  7'47.33

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "randommw.h"
#include "zigtimer.h"

int main(int argc, char **argv)
{
	uint64_t zigseed;
	uint64_t Njumps;
	unsigned int i;
	double rval;

	switch(argc)
	{
		case 3:
			zigseed = (uint64_t)atoi(argv[1]);
			Njumps = (uint64_t)atoll(argv[2]);
			break;
		default:
			printf("ERROR. Unexpected number of arguments.\n");
			printf("usage: %s <seed> <Njumps>\n", argv[0]);
			return(1);
	}

	RanSetRan("Xoshiro256+"); printf("Xoshiro256+ activated.\n");
	// RanSetRan("MELG19937"); printf("MELG19937 activated.\n");
	RanInit(zigseed);
	
	printf("\n");
	for(i = 0; i < 20; i++)	{
		rval = DRanNormalZig();
		printf("%10.6f\n", rval);
	}
	
	printf("\n");
	printf("*** %"PRIu64" long jumps of PRNG ***\n", Njumps);
	printf("\n");
	
	StartTimer();
	RanJumpRan(Njumps);
	StopTimer();
	
	for(i = 0; i < 20; i++)	{
		rval = DRanNormalZig();
		printf("%10.6f\n", rval);
	}
	printf("\n");
	
	printf("%"PRIu64" long jumps of PRNG in %s\n\n", 
	       Njumps, GetLapsedTime());

	return 0;
}
