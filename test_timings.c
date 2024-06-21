#include <stdio.h>
#include <stdlib.h>

#include "randommw.h"
#include "zigtimer.h"


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

	(*DRanSeed)(0);

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

	Timer("Warming up",		DRan_MWC8222, 			RanSetSeed_MWC8222, cm/10); 
	Timer("MWC8222",		DRan_MWC8222, 			RanSetSeed_MWC8222, cm);
	Timer("MWC_52",			DRan_MWC_52, 			RanSetSeed_MWC8222, cm);

	RanSetRan("MWC8222");
	Timer("ZIGNOR MWC8222",			DRanNormalZig, 			RanInit, cm);
	
	RanSetRan("MWC_52");
	Timer("ZIGNOR MWC_52",	DRanNormalZig, 			RanInit, cm);
	
	RanSetRan("MWC8222");
	Timer("ZIGNOR MWC8222 (again)",	DRanNormalZig, 			RanInit, cm);
	
	RanSetRan("Xoshiro256+");
	Timer("ZIGNOR Xoshiro256+",	DRanNormalZig, 			RanInit, cm);
	
	/* Code snippet to see if the program indeed crashes gracefully if
	 * wrong string passed to RanSetRan                          */
	/*
	RanSetRan("CRASH!! SEGV...");
	Timer("CRASH!!",	DRanNormalZig, 			RanInit, cm);
	*/
	
	TimerFooter();
	
	return 0;
}
