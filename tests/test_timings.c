#include <stdio.h>
#include <stdlib.h>

#include "randommw.h"


void TimerHeader() 
{
	printf("------------------------------------------------------------------------\n");
	printf("%-30s%8s %22s %10s\n", "Name", "time", "mean", "reps");
	printf("------------------------------------------------------------------------\n");
}

void TimerFooter()
{
	printf("------------------------------------------------------------------------\n");
}

void Timer(char *sName, double (*DRan)(), void (*DRanSeed)(uint64_t), unsigned int cM) 
{
	unsigned int i;
	double ran, mean;
	
	printf("%-29s", sName);

	(*DRanSeed)(17732);

	StartTimer();
	for (i = 0, mean = 0.0; i < cM; ++i)
	{
		ran = (*DRan)();
		mean += ran;
	}
	mean /= cM;
	StopTimer();

	printf(" %8s %#22.15g %10d\n", GetLapsedTime(), mean, cM);
}


int main(void) 
{
	unsigned int cm = 1000000000;

	TimerHeader();

	RanInit("MWC8222", 0, 0);
	Timer("Warming up",		DRanU, 		RanSetSeed, cm/10); 
	
	RanInit("MWC8222", 0, 0);
	Timer("MWC8222",		DRanU, 		RanSetSeed, cm);
		
	RanInit("Lehmer64", 0, 0);
	Timer("Lehmer64",		DRanU, 		RanSetSeed, cm);

	RanInit("PCG64DXSM", 0, 0);
	Timer("PCG64DXSM",		DRanU, 		RanSetSeed, cm);
	
	RanInit("Xoshiro256+", 0, 0);
	Timer("Xoshiro256+",	DRanU,		RanSetSeed, cm);
		
	RanInit("MELG19937", 0, 0);
	Timer("MELG19937",     	DRanU,		RanSetSeed, cm);



	RanInit("MWC8222", 0, 0);
	Timer("ZIGNOR MWC8222",			DRanNormalZig, 			RanSetSeed, cm);
		
	RanInit("Lehmer64", 0, 0);
	Timer("ZIGNOR Lehmer64",		DRanNormalZig, 			RanSetSeed, cm);

	RanInit("PCG64DXSM", 0, 0);
	Timer("ZIGNOR PCG64DXSM",		DRanNormalZig, 			RanSetSeed, cm);
	
	RanInit("Xoshiro256+", 0, 0);
	Timer("ZIGNOR Xoshiro256+",		DRanNormalZig, 			RanSetSeed, cm);

	RanInit("MELG19937", 0, 0);
	Timer("ZIGNOR MELG19937",		DRanNormalZig, 			RanSetSeed, cm);
	
	RanInit("MWC8222", 0, 0);
	Timer("ZIGNOR MWC8222 (again)",	DRanNormalZig, 			RanSetSeed, cm);

	
	/* Code snippet to see if the program indeed crashes gracefully if
	 * wrong string passed to RanSetRan                          */
	/*
	RanSetRan("CRASH!! SEGV...");
	Timer("CRASH!!",	DRanNormalZig, 			RanInit, cm);
	*/
	
	TimerFooter();
	
	return 0;
}
