/*

genzignor.c

Generate a binary file containing normally N(0,1) distributed random numbers

*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "randommw.h"
#include "zigtimer.h"

#define FNAMEMAX 63

int main(int argc, char **argv)
{
	char fname[FNAMEMAX+1];
	int zigseed;
	long long int i, Nsamples;
	double *dbuf;
	FILE *fp;
	
	switch(argc)
	{
		case 4:
			zigseed = atoi(argv[1]);
			Nsamples = atoll(argv[2]);
			strncpy(fname, argv[3], FNAMEMAX);
			break;
		default:
			printf("ERROR. Unexpected number of arguments.\n");
			printf("usage: %s <seed> <Nsamples> <filename>\n", argv[0]);
			return(1);
	}

	printf("\nGENZIGNOR v1.0\n");
	printf("-------------------------------------\n");	
	printf("seed (int32)      : %d\n", zigseed);
	printf("Nsamples (int64)  : %lld\n", Nsamples);
	printf("output file       : %s\n", fname);
	printf("-------------------------------------\n");
	
	RanNormalSetSeedZig(&zigseed, 1);
	
	dbuf = malloc(sizeof(*dbuf) * Nsamples);
	
	StartTimer();
	for (i = 0; i < Nsamples; i++)
		dbuf[i] = DRanNormalZig();
	StopTimer();
	
	printf("Random generation : %s seconds\n", GetLapsedTime());
	printf("-------------------------------------\n");	
	
	StartTimer();
	fp = fopen(fname, "wb");
	fwrite(dbuf, sizeof(*dbuf), Nsamples, fp);	
	fclose(fp);
	StopTimer();
	
	printf("File output       : %s seconds\n", GetLapsedTime());
	printf("-------------------------------------\n\n");	
	
	free(dbuf);
	
	return 0;
}