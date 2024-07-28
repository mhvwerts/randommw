/* test_minimal.c : simple generation of normally distributed random numbers

*/
#include <stdio.h>
#include "randommw.h"

int main(void) {
	unsigned int i;
	uint64_t zigseed = 10;
	double rval;
		
	RanInit("", zigseed, 0);
	
	for(i = 0; i < 20; i++)	{
		rval = DRanNormalZig();
		printf("%10.6f\n", rval);
	}
	
	return 0;
}
