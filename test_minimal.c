/* test_minimal.c : simple generation of normally distributed random numbers

*/
#include <stdio.h>
#include "randommw.h"

int main(void) {
	unsigned int i;
	int zigseed = 10;
	double rval;
		
	RanNormalSetSeedZig(&zigseed, 1);
	
	for(i = 0; i < 20; i++)	{
		rval = DRanNormalZig();
		printf("%10.6f\n", rval);
	}
	
	return 0;
}
