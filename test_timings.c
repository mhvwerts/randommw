#include <stdio.h>
#include <stdlib.h>

#include "zigrandom.h"
#include "zigtimer.h"
#include "zignor.h"


void TimerHeader() 
{
	printf("------------------------------------------------------------------\n");
	printf("%-30s%8s %22s %10s\n", "Name", "time", "mean", "reps");

}
void Timer(char *sName, double (*DRan)(), void (*DRanSeed)(int *, int), unsigned int cM) 
{
	unsigned int i;
	double ran, mean;
	
	printf("%-29s", sName);

	(*DRanSeed)(NULL, 0);

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

	RanSetRan("MWC8222");
	Timer("ZIGNOR",			DRanNormalZig, 			RanNormalSetSeedZig, cm);

	return 0;
}
