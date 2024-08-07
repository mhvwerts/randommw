#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "randommw.h"

/* test_moments.c 
 *
 * Returns the raw moments of a Gaussian pseudorandom number generator.
 *
 *
 * This program takes 1 or 2 optional arguments: 
 * - the random seed
 * - the underlying uniform random number generator
 *
 *
 * Analysis of the raw moments of the generated numbers proceeds
 * following C. D. Macfarland:
 * - https://github.com/cd-mcfarland/fast_prng
 * - C. D. McFarland, "A modified ziggurat algorithm for generating
 *   exponentially and normally distributed pseudorandom numbers", 
 *   Journal of Statistical Computation and Simulation 2016, 7, 1281.
 *   https://dx.doi.org/10.1080/00949655.2015.1060234    
 *
 * This program helps in confirming that the random numbers generated are
 * normally distributed as intended. Unlike central moments, raw moments 
 * are always unbiased estimators of the expected value of the raw moment. 
 * (e.g. The sample variance is an unbiased estimator of variance only if 
 * adjusted by a factor of N/(N - 1).)
 *
 * */


// test parameters (could be moved to command line params)

#define PREPRINT		20
#define TRIALS 			1000000000 // pow(10, 12)
#define NUM_RAW_MOMENTS	8



// utility functions

int factorial(int n) {
    if (n<=1) return(1);
    else n=n*factorial(n-1);
    return(n);
}

int double_factorial(int n) {
    if (n<=1) return(1);
    else n=n*double_factorial(n-2);
    return(n);
}


// main program

int main(int argc, char *argv[])
{
	unsigned long int i, j;

	double ran;
	double val, X[NUM_RAW_MOMENTS], x_j;

	int SeedZig;

	int expected;
	
	switch(argc)
	{
		case 1:
			SeedZig = 0;
			RanInit("", SeedZig, 0);
			break;
		case 2:
			SeedZig = atoi(argv[1]);
			RanInit("", (uint64_t)SeedZig, 0);
			break;
		case 3:
			SeedZig = atoi(argv[1]);
			printf("%s pseudo-random number generator selected.\n", argv[2]);
			RanInit(argv[2], (uint64_t)SeedZig, 0);
			break;
		default:
			printf("ERROR. Unexpected number of arguments\n");
			return(1);
	}
	
	printf("seed = %d\n", SeedZig);


	// Print the first numbers generated, for visual inspection
	for (i = 0; i < PREPRINT; i++)
	{
		ran = DRanNormalZig();
		printf("%10.6f\n", ran);
	}
	
	// Calculate raw moments of the generated random numbers
	for (i=0; i<NUM_RAW_MOMENTS; i++) {
		X[i] = 0.0;
	}

	for (i=0; i<TRIALS; i++) {
		val = DRanNormalZig();
		for (j=0, x_j=val; j<NUM_RAW_MOMENTS; j++, x_j*=val) {
			X[j] += x_j;
		}
	}

	//Output moments
	printf("Created %ld normally distributed pseudo-random numbers...\n", (long)TRIALS);
	for (i=0; i<NUM_RAW_MOMENTS; i++) {
		expected = ( (i+1)%2 == 0 ? double_factorial(i) : 0 );
		printf("X%lu: %f (Expected %i)\n", i+1, X[i]/TRIALS, expected);	
	}
	return 0;
}
