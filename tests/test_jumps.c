/*

test_jumps.c

Test the speed of the long jump capability of the PRNGs.
(Presently, only Xoshiro256+ and MELG19937 support this.)


Xoshiro256+ long jump corresponds to 2^192 calls to next()
MELG19937 long jumps correspond to 2^256 calls to next()


Typical speed test results on Intel Core i7 (Windows 11, gcc, w64devkit, 2024)

- 2000000 long jumps of Xoshiro256+ in  0.94 s
- 50000000 long jumps of Xoshiro256+ in 23.36 s
- 200000000 long jumps of Xoshiro256+ in  1'33.57
- 1000000000 long jumps of Xoshiro256+ in  7'47.33

(MELG19937 long jumps are considerably slower)

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "randommw.h"

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
			RanInit("Xoshiro256+", zigseed, 0); // zero initial jumps
			printf("Xoshiro256+ activated.\n");
			break;
		case 4:
			zigseed = (uint64_t)atoi(argv[1]);
			Njumps = (uint64_t)atoll(argv[2]);
			printf("%s pseudo-random number generator selected.\n", argv[3]);
			RanInit(argv[3], zigseed, 0);
			break;
		default:
			printf("ERROR. Unexpected number of arguments.\n");
			printf("usage: %s <seed> <Njumps> [<PRNG>]\n", argv[0]);
			return(1);
	}

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
