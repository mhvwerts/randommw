#include <stdio.h>
#include <stdlib.h>

#include <inttypes.h>


#include "zigrandom.h"
#include "zignor.h"

/* test_seed.c 
 *
 * Exploring & documenting how to set the Zignor MWC random seed
 *
 * This program takes 1 optional argument: the random seed
 *
 *
 *
 * Further code based on 'test_moments.c'
 * which returns the raw moments of the generated numbers
 * This part is based on code by C. D. Macfarland:
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

// test parameters (could be moved to command line params, in part)
// does not yet include the random seed

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

// Testing evaluation of somewhat cryptic expression in zigrandom.c
// 
void eval_RanSetSeed(int *piSeed, int cSeed)
{
	printf("evalRanSetSeed result: %d\n", piSeed && cSeed ? piSeed[0] : 0);
}


// zigrandom.c/GetInitialSeed() is called to set MWC initial state (256 uint values) 
//    This implements the Numerical Recipes 'randq1' 'even quicker & dirtier' PRNG.
//    It relies on 32 bit unsigned int arithmetic, so let's check that we indeed use
//    32 bit unsigned int arithmetic
void eval_sizeof_uint()
{
	printf("unsigned int size in bytes: %lld\n", sizeof(unsigned int));
}



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
			break;
		case 2:
			SeedZig = atoi(argv[1]);
			break;
		default:
			printf("ERROR. Unexpected number of arguments\n");
			return(1);
	}
	
	

	/*
		HOW TO SEED
	
	    The standard Zignor generator is initialized and
		seeded using 
		    RanNormalSetSeedZig(int *piSeed, int cSeed) 
		which calls
		first: zigNorInit() -> initialization of ziggurat
		then: RanSetSeed(int *piseed, int cSeed) 
			-> set seed of chosen random generator
		which is always MWC8222 for now
		and is handled by RanSetSeed_MWC8222(int *piSeed, inc cSeed)
		
		There, if cSeed == MWC_R (currently set at 256), piSeed
		is expected to point to a 256 (MWC_R)-element int array that initializes
		the MWC8222 state.
		
		Other values for cSeed will go to GetInitialSeeds
		with 256 (MWC_R) and piSeed[0] as parameters
		or just 256 and 0 if piSeed is not intialized or cSeed is 0
		
		Hence, there are three cases here:
		cSeed == MWC_R: initialize MWC8222 from *piSeed
		cSeed == 0: initialize MWC8222 using 0 as seed
		cSeed == 1: initialize MWC8222 using piSeed[0] as seed
	
		Therefore, if a seed is needed:
		cSeed == 1 and pass a pointer to the seed value
		
		The seed value is an int (32 bits) which is somewhere
		converted implicitly to an unsigned 32 bit int, through
		pointer exchange, conserving all bits
		
    */ 	
	
	// just pass the pointer to the variable containing the seed
	// and set cSeed to 1
	eval_RanSetSeed(&SeedZig, 1); // just checking
	RanNormalSetSeedZig(&SeedZig, 1); 

	// check 32-bitness of uint
	eval_sizeof_uint();

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
