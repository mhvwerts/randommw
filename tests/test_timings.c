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

	(*DRanSeed)(12345);

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

	Timer("Warming up",		DRan_MWC256, 			RanSetSeed_MWC256, cm/10); 
	Timer("MWC256",			DRan_MWC256, 			RanSetSeed_MWC256, cm);
	Timer("Xoshiro256+",	DRan_xoshiro256p,		RanSetSeed_xoshiro256p, cm);

	RanSetRan("MWC256");
	Timer("ZIGNOR MWC256",			DRanNormalZig, 			RanInit, cm);
	
	RanSetRan("Xoshiro256+");
	Timer("ZIGNOR Xoshiro256+",		DRanNormalZig, 			RanInit, cm);
	
	RanSetRan("MWC256");
	Timer("ZIGNOR MWC256 (again)",	DRanNormalZig, 			RanInit, cm);

	
	/* Code snippet to see if the program indeed crashes gracefully if
	 * wrong string passed to RanSetRan                          */
	/*
	RanSetRan("CRASH!! SEGV...");
	Timer("CRASH!!",	DRanNormalZig, 			RanInit, cm);
	*/
	
	TimerFooter();
	
	return 0;
}
