/*

genzignor.c

Generate a binary file containing normally N(0,1) distributed random numbers

*/

#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "randommw.h"

#define FNAMEMAX 63

int main(int argc, char **argv)
{
	char fname[FNAMEMAX+1];
	union
	{
		int64_t int64;
		uint64_t uint64;
	} zigseed, Nsamples;
	uint64_t i;
	double *dbuf;
	FILE *fp;
	
	switch(argc)
	{
		case 4:
			zigseed.int64 = atoll(argv[1]);
			Nsamples.int64 = atoll(argv[2]);
			if (Nsamples.int64 < 0)
				Nsamples.int64 = 0;
			strncpy(fname, argv[3], FNAMEMAX);
			break;
		default:
			printf("ERROR. Unexpected number of arguments.\n");
			printf("usage: %s <seed> <Nsamples> <filename>\n", argv[0]);
			return(1);
	}

	printf("\nGENZIGNOR v1.0\n");
	printf("---------------------------------------------------------\n");	
	printf("seed (int64 -> uint64)      : %"PRId64" -> %"PRIu64"\n", 
	       zigseed.int64, zigseed.uint64);
	printf("Nsamples (uint64)           : %"PRIu64"\n", Nsamples.uint64);
	printf("output file                 : %s\n", fname);
	printf("---------------------------------------------------------\n");
	
	RanInit("", zigseed.uint64, 0);
	
	dbuf = malloc(sizeof(*dbuf) * Nsamples.uint64);
	
	StartTimer();
	for (i = 0; i < Nsamples.uint64; i++)
		dbuf[i] = DRanNormalZig();
	StopTimer();
	
	printf("Random generation           : %s\n", GetLapsedTime());
	printf("---------------------------------------------------------\n");	
	
	StartTimer();
	fp = fopen(fname, "wb");
	fwrite(dbuf, sizeof(*dbuf), Nsamples.uint64, fp);	
	fclose(fp);
	StopTimer();
	
	printf("File output                 : %s\n", GetLapsedTime());
	printf("---------------------------------------------------------\n\n");	
	
	free(dbuf);
	
	return 0;
}
