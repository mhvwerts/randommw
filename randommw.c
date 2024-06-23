/*==========================================================================
 *==========================================================================
 * randommw.c
 * Pseudo-random number generators with uniform and Gaussian dstributions
 * 
 * A re-mix of various tried and tested routines, refactored into a single
 * source file, giving access to all practical aspects of the random number
 * generation, while still being convenient to use.
 * 
 * by Martinus H. V. Werts, 2024, with code from various authors cited
 * below.
 *
 *==========================================================================
 *
 * This source file is organized as follows.
 *
 * A. xoshiro256+ PRNG by Vigna & Blackman, and splitmix64 PRNG
 * B. modified version of 'zigrandom.c' containing the MWC256 (aka MWC8222)
 *    PRNG, and interfacing functions for 'zignor.c'
 * C. modified version of 'zignor.c' with Doornik's ziggurat algorithm for
 *    generation of normally distributed random numbers
 * D. Additional content
 *
 *==========================================================================
 *==========================================================================*/

#include <limits.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "randommw.h"


/*==========================================================================
 * xoshiro256+ and splitmix64
 *
 * xoshiro256+ and splitmix64 for the generation of 32-bit unsigned integers 
 * and 53-bit mantissa doubles
 *
 * see: https://prng.di.unimi.it/
 *
 * Original code with cosmetic changes (renaming of variables and functions) 
 * and routines for direct output of uint32s and (0,1) doubles 
 *
 * Modifications by M. H. V. Werts, 2024
 *
 *==========================================================================*/

/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide.

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. */


/* This is xoshiro256+ 1.0, our best and fastest generator for floating-point
   numbers. We suggest to use its upper bits for floating-point
   generation, as it is slightly faster than xoshiro256++/xoshiro256p**. It
   passes all tests we are aware of except for the lowest three bits,
   which might fail linearity tests (and just those), so if low linear
   complexity is not considered an issue (as it is usually the case) it
   can be used to generate 64-bit outputs, too.

   We suggest to use a sign test to extract a random Boolean value, and
   right shifts to extract subsets of bits.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */


static inline uint64_t rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}


static uint64_t xoshiro256p_s[4];

uint64_t xoshiro256p_next(void) {
	const uint64_t result = xoshiro256p_s[0] + xoshiro256p_s[3];

	const uint64_t t = xoshiro256p_s[1] << 17;

	xoshiro256p_s[2] ^= xoshiro256p_s[0];
	xoshiro256p_s[3] ^= xoshiro256p_s[1];
	xoshiro256p_s[1] ^= xoshiro256p_s[2];
	xoshiro256p_s[0] ^= xoshiro256p_s[3];

	xoshiro256p_s[2] ^= t;

	xoshiro256p_s[3] = rotl(xoshiro256p_s[3], 45);

	return result;
}


/* This is the jump function for the generator. It is equivalent
   to 2^128 calls to next(); it can be used to generate 2^128
   non-overlapping subsequences for parallel computations. */

void xoshiro256p_jump(void) {
	static const uint64_t JUMP[] = { 0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c };

	uint64_t s0 = 0;
	uint64_t s1 = 0;
	uint64_t s2 = 0;
	uint64_t s3 = 0;
	for(int i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
		for(int b = 0; b < 64; b++) {
			if (JUMP[i] & UINT64_C(1) << b) {
				s0 ^= xoshiro256p_s[0];
				s1 ^= xoshiro256p_s[1];
				s2 ^= xoshiro256p_s[2];
				s3 ^= xoshiro256p_s[3];
			}
			xoshiro256p_next();	
		}
		
	xoshiro256p_s[0] = s0;
	xoshiro256p_s[1] = s1;
	xoshiro256p_s[2] = s2;
	xoshiro256p_s[3] = s3;
}


/* This is the long-jump function for the generator. It is equivalent to
   2^192 calls to next(); it can be used to generate 2^64 starting points,
   from each of which jump() will generate 2^64 non-overlapping
   subsequences for parallel distributed computations. */

void xoshiro256p_long_jump(void) {
	static const uint64_t LONG_JUMP[] = { 0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241, 0x39109bb02acbe635 };

	uint64_t s0 = 0;
	uint64_t s1 = 0;
	uint64_t s2 = 0;
	uint64_t s3 = 0;
	for(int i = 0; i < sizeof LONG_JUMP / sizeof *LONG_JUMP; i++)
		for(int b = 0; b < 64; b++) {
			if (LONG_JUMP[i] & UINT64_C(1) << b) {
				s0 ^= xoshiro256p_s[0];
				s1 ^= xoshiro256p_s[1];
				s2 ^= xoshiro256p_s[2];
				s3 ^= xoshiro256p_s[3];
			}
			xoshiro256p_next();	
		}
		
	xoshiro256p_s[0] = s0;
	xoshiro256p_s[1] = s1;
	xoshiro256p_s[2] = s2;
	xoshiro256p_s[3] = s3;
}


/*----------------------------------------------------------------
 * splitmix64
 *
 * Can be used to generate a 4 x 64-bit seed for xoshiro256+
 * from a single 64-bit seed
 *
 * And can also be used in its own right to generate random 
 * numbers, as it has also been reported to pass statistical tests
 * (https://github.com/lemire/testingRNG)
 * 
 * names of global variables and functions were adapted (Werts, 2024)
 *----------------------------------------------------------------*/

/*  Written in 2015 by Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide.

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. */

/* This is a fixed-increment version of Java 8's SplittableRandom generator
   See http://dx.doi.org/10.1145/2714064.2660195 and 
   http://docs.oracle.com/javase/8/docs/api/java/util/SplittableRandom.html

   It is a very fast generator passing BigCrush, and it can be useful if
   for some reason you absolutely want 64 bits of state. */

static uint64_t splitmix64_x; /* The state can be seeded with any value. */

uint64_t splitmix64_next() {
	uint64_t z = (splitmix64_x += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
	return z ^ (z >> 31);
}


/*----------------------------------------------------------------
 * Interface between xoshiro256+ and zigrandom
 *----------------------------------------------------------------*/

void RanSetSeed_xoshiro256p(uint64_t uSeed)
{
	RanSetSeed_splitmix64(uSeed); // seed splitmix
	
	// use splitmix to fully seed xoshiro256p
	xoshiro256p_s[0] = splitmix64_next();
	xoshiro256p_s[1] = splitmix64_next();
	xoshiro256p_s[2] = splitmix64_next();
	xoshiro256p_s[3] = splitmix64_next();
}

void RanJump_xoshiro256p(uint64_t uJumps)
{
	uint64_t i;
	for (i=0; i<uJumps; i++)
		xoshiro256p_long_jump();
}

/* The 32-bit unsigned integer IRan random routine uses only
   the upper 32 bits of Xoshiro256+, which are of highest
   random quality, and should pass all randomness tests. */
uint32_t IRan_xoshiro256p(void)
{
	return (uint32_t)(xoshiro256p_next() >> 32);
}

double DRan_xoshiro256p(void)
{
	uint64_t xx;
	
	/*
	xx = 0;
	while (xx == 0)
		xx = (xoshiro256p_next() >> 11);
	*/
	
	while ((xx = (xoshiro256p_next() >> 11)) == 0)
		;
	
	return (xx * 0x1.0p-53);
}



/*----------------------------------------------------------------
 * Interface between splitmix64 and zigrandom
 *  based on the xoshiro256+ interface
 *----------------------------------------------------------------*/

void RanSetSeed_splitmix64(uint64_t uSeed)
{
	splitmix64_x = uSeed; // seed splitmix
}

uint32_t IRan_splitmix64(void)
{
	return (uint32_t)(splitmix64_next() >> 32);
}

double DRan_splitmix64(void)
{
	uint64_t xx;
	
	while ((xx = (splitmix64_next() >> 11)) == 0)
		;
	
	return (xx * 0x1.0p-53);
}



/*==========================================================================*/


/*==========================================================================
 *  Modified version of zigrandom.c
 *  Martinus H. V. Werts, 2024
 *
 *  - Fixed a gcc warning
  *  - Implemented full 52-bit mantissa precision generator of random doubles
 *    in (0, 1) from pairs of 32-bit unsignedintegers coming from MWC256
 *    (Doornik callled this generator "MWC_52")
 *  - Renamed MWC8222 to MWC256
 *  - Use stdint.h for exact integer widths
 *
 *==========================================================================
 *  This code is Copyright (C) 2005, Jurgen A. Doornik.
 *  Permission to use this code for non-commercial purposes
 *  is hereby given, provided proper reference is made to:
 *		Doornik, J.A. (2005), "An Improved Ziggurat Method to Generate Normal
 *          Random Samples", mimeo, Nuffield College, University of Oxford,
 *			and www.doornik.com/research.
 *		or the published version when available.
 *	This reference is still required when using modified versions of the code.
 *  This notice should be maintained in modified versions of the code.
 *	No warranty is given regarding the correctness of this code.
 *==========================================================================*/


/*------------------------ George Marsaglia MWC ----------------------------*/
#define MWC_R  256
#define MWC_A  809430660ull // unsigned long long is 64 bits
#define MWC_AI 809430660
#define MWC_C  362436
static uint32_t s_uiStateMWC = MWC_R - 1;
static uint32_t s_uiCarryMWC = MWC_C;
static uint32_t s_auiStateMWC[MWC_R];


void GetInitialSeeds_MWC256(uint32_t auiSeed[], int32_t cSeed,
	uint32_t uiSeed, uint32_t uiMin)
{
	int i;
	uint32_t s = uiSeed;									/* may be 0 */

	for (i = 0; i < cSeed; )
	{	/* see Knuth p.106, Table 1(16) and Numerical Recipes p.284 (ranqd1)*/
		s = 1664525 * s + 1013904223;
		if (s <= uiMin)
			continue;
        auiSeed[i] = s;
		++i;
    }
}

/*
The MWC256 generator is initialized and seeded using 
	RanSetSeed_MWC256_original(int *piSeed, int cSeed)

If cSeed == MWC_R (currently set at 256), piSeed
is expected to point to a 256 (MWC_R)-element int array that initializes
the MWC256 state.

Other values for cSeed will go to GetInitialSeeds
with 256 (MWC_R) and piSeed[0] as parameters
or just 256 and 0 if piSeed is not intialized or cSeed is 0

Hence, there are three cases here:
- cSeed == MWC_R: initialize MWC256 from *piSeed
- cSeed == 0: initialize MWC256 using 0 as seed
- cSeed == 1: initialize MWC256 using piSeed[0] as seed

Therefore, if a single seed value is used:
cSeed == 1 and pass a pointer to the seed value

The seed value is a signed int (32 bits) which is somewhere
converted implicitly to an unsigned 32 bit int, through
pointer exchange, conserving all bits
*/ 	
void RanSetSeed_MWC256_original(int *piSeed, int cSeed)
{
	s_uiStateMWC = MWC_R - 1;
	s_uiCarryMWC = MWC_C;
	
	if (cSeed == MWC_R)
	{
		int i;
		for (i = 0; i < MWC_R; ++i)
		{
			s_auiStateMWC[i] = (uint32_t)piSeed[i];
		}
	}
	else
	{
		GetInitialSeeds_MWC256(s_auiStateMWC, MWC_R, piSeed && cSeed ? piSeed[0] : 0, 0);
	}
}

// New-style RanSetSeed interface (single unsigned 64-bit integer seed)
// interfaced to original RanSetSeed for MWC256
void RanSetSeed_MWC256(uint64_t uSeed)
{
	int iSeed;
	iSeed = (int)uSeed;
	RanSetSeed_MWC256_original(&iSeed, 1);
}

uint32_t IRan_MWC256(void)
{
	uint64_t t;

	s_uiStateMWC = (s_uiStateMWC + 1) & (MWC_R - 1);
	t = MWC_A * s_auiStateMWC[s_uiStateMWC] + s_uiCarryMWC;
	s_uiCarryMWC = (uint32_t)(t >> 32);
	s_auiStateMWC[s_uiStateMWC] = (uint32_t)t;
    return (uint32_t)t;
}

/*
// From the original 'zigrandom.c' by Doornik: 
// Obsolete 32-bit mantissa precision double random generator, which, 
// on 64-bit hardware, is only marginally faster than
// the newer 52-bit mantissa version below.

double DRan_MWC8222(void)
{
	uint64_t t;

	s_uiStateMWC = (s_uiStateMWC + 1) & (MWC_R - 1);
	t = MWC_A * s_auiStateMWC[s_uiStateMWC] + s_uiCarryMWC;
	s_uiCarryMWC = (uint32_t)(t >> 32);
	s_auiStateMWC[s_uiStateMWC] = (uint32_t)t;
	return RANDBL_32new(t);
}
*/

double DRan_MWC256(void)
/* Generate random doubles with full-precision 52-bit mantissa using MWC256 */
{
	uint64_t t1, t2;

	s_uiStateMWC = (s_uiStateMWC + 1) & (MWC_R - 1);
	t1 = MWC_A * s_auiStateMWC[s_uiStateMWC] + s_uiCarryMWC;
	s_uiCarryMWC = (uint32_t)(t1 >> 32);
	s_auiStateMWC[s_uiStateMWC] = (uint32_t)t1;
	
	s_uiStateMWC = (s_uiStateMWC + 1) & (MWC_R - 1);
	t2 = MWC_A * s_auiStateMWC[s_uiStateMWC] + s_uiCarryMWC;
	s_uiCarryMWC = (uint32_t)(t2 >> 32);
	s_auiStateMWC[s_uiStateMWC] = (uint32_t)t2;
	
	return RANDBL_52new(t1, t2);
}
/*----------------------- END George Marsaglia MWC -------------------------*/


/*------------------- uniform random number generators ----------------------*/
static int s_cNormalInStore = 0;		     /* > 0 if a normal is in store */

/* Default MWC256 uniform generator 
   (doubles with 52 bits mantissa randomness) */
static DRANFUN s_fnDRanu = DRan_MWC256;
static IRANFUN s_fnIRanu = IRan_MWC256;
static RANSETSEEDFUN s_fnRanSetSeed = RanSetSeed_MWC256;

double  DRanU(void)
{
    return (*s_fnDRanu)();
}

uint32_t IRanU(void)
{
    return (*s_fnIRanu)();
}

void    RanSetSeed(uint64_t uSeed)
{
   	s_cNormalInStore = 0;
	(*s_fnRanSetSeed)(uSeed);
}

/* keep generalized PRNG jumps for later
// jumps currently only supported by Xoshiro256+
// call RanJump_xoshiro256p() directly
void    RanJump(uint64_t uJumpsize)
{
	(*s_fnRanJump)(uJumpsize);
}
*/

void    RanSetRan(const char *sRan)
{
   	s_cNormalInStore = 0;
	
	/* BEGIN if ... else if ... else block */
	if (strcmp(sRan, "MWC256") == 0)
	{
		s_fnDRanu = DRan_MWC256;
		s_fnIRanu = IRan_MWC256;
		s_fnRanSetSeed = RanSetSeed_MWC256;
	}
	else if (strcmp(sRan, "Xoshiro256+") == 0)
	{
		s_fnDRanu = DRan_xoshiro256p;
		s_fnIRanu = IRan_xoshiro256p;
		s_fnRanSetSeed = RanSetSeed_xoshiro256p;
	}
	else if (strcmp(sRan, "Splitmix64") == 0)
	{
		s_fnDRanu = DRan_splitmix64;
		s_fnIRanu = IRan_splitmix64;
		s_fnRanSetSeed = RanSetSeed_splitmix64;
	}
	else // DEFAULT
	{
		s_fnDRanu = NULL;
		s_fnIRanu = NULL;
		s_fnRanSetSeed = NULL;
	}
	/* END if ... else if ... else block */
}

static uint32_t IRanUfromDRanU(void)
{
    return (uint32_t)(UINT_MAX * (*s_fnDRanu)());
}

static double DRanUfromIRanU(void)
{
    return RANDBL_32new( (*s_fnIRanu)() );
}

void    RanSetRanExt(DRANFUN DRanFun, IRANFUN IRanFun, RANSETSEEDFUN RanSetSeedFun)
{
	s_fnDRanu = DRanFun ? DRanFun : DRanUfromIRanU;
	s_fnIRanu = IRanFun ? IRanFun : IRanUfromDRanU;
	s_fnRanSetSeed = RanSetSeedFun;
}
/*---------------- END uniform random number generators --------------------*/


/*----------------------------- Polar normal RNG ---------------------------*/
#define POLARBLOCK(u1, u2, d)	              \
	do                                        \
	{   u1 = (*s_fnDRanu)();  u1 = 2 * u1 - 1;\
		u2 = (*s_fnDRanu)();  u2 = 2 * u2 - 1;\
		d = u1 * u1 + u2 * u2;                \
	} while (d >= 1);                         \
	d = sqrt( (-2.0 / d) * log(d) );       	  \
	u1 *= d;  u2 *= d

static double s_dNormalInStore;

double  DRanNormalPolar(void)                         /* Polar Marsaglia */
{
    double d, u1;

    if (s_cNormalInStore)
        u1 = s_dNormalInStore, s_cNormalInStore = 0;
    else
    {
        POLARBLOCK(u1, s_dNormalInStore, d);
        s_cNormalInStore = 1;
    }

return u1;
}

#define FPOLARBLOCK(u1, u2, d)	              \
	do                                        \
	{   u1 = (float)((*s_fnDRanu)());  u1 = 2 * u1 - 1;\
		u2 = (float)((*s_fnDRanu)());  u2 = 2 * u2 - 1;\
		d = u1 * u1 + u2 * u2;                \
	} while (d >= 1);                         \
	d = sqrt( (-2.0 / d) * log(d) );       	  \
	u1 *= d;  u2 *= d

static float s_fNormalInStore;

double  FRanNormalPolar(void)                         /* Polar Marsaglia */
{
    float d, u1;

    if (s_cNormalInStore)
        u1 = s_fNormalInStore, s_cNormalInStore = 0;
    else
    {
        POLARBLOCK(u1, s_fNormalInStore, d);
        s_cNormalInStore = 1;
    }

return (double)u1;
}
/*--------------------------- END Polar normal RNG -------------------------*/

/*------------------------------ DRanQuanNormal -----------------------------*/
static double dProbN(double x, int fUpper)
{
    double p;  double y;  // int fnegative = 0; // mw231228 remove gcc warning

    if (x < 0)
        x = -x, fUpper = !fUpper; // fnegative = 1,  // mw231228 remove gcc warning
    else if (x == 0)
        return 0.5;

    if ( !(x <= 8 || (fUpper && x <= 37) ) )
        return (fUpper) ? 0 : 1;

    y = x * x / 2;

    if (x <= 1.28)
    {
        p = 0.5 - x * (0.398942280444 - 0.399903438504 * y /
            (y + 5.75885480458 - 29.8213557808 /
            (y + 2.62433121679 + 48.6959930692 /
            (y + 5.92885724438))));
    }
    else
    {
        p = 0.398942280385 * exp(-y) /
            (x - 3.8052e-8 + 1.00000615302 /
            (x + 3.98064794e-4 + 1.98615381364 /
            (x - 0.151679116635 + 5.29330324926 /
            (x + 4.8385912808 - 15.1508972451 /
            (x + 0.742380924027 + 30.789933034 /
            (x + 3.99019417011))))));
    }
    return (fUpper) ? p : 1 - p;
}

double  DProbNormal(double x)
{
    return dProbN(x, 0);
}

double  DRanQuanNormal(void)
{
	return DProbNormal(DRanNormalPolar());
}

double  FRanQuanNormal(void)
{
	return DProbNormal(FRanNormalPolar());
}
/*----------------------------- END DRanQuanNormal -------------------------*/


/*==========================================================================*/


/*==========================================================================
 *  Modified version of zignor.c
 *  Martinus H. V. Werts, 2024
 *
 *  - VIZIGNOR and related optimizations have been removed
 *
 *==========================================================================
 *  This code is Copyright (C) 2005, Jurgen A. Doornik.
 *  Permission to use this code for non-commercial purposes
 *  is hereby given, provided proper reference is made to:
 *		Doornik, J.A. (2005), "An Improved Ziggurat Method to Generate Normal
 *          Random Samples", mimeo, Nuffield College, University of Oxford,
 *			and www.doornik.com/research.
 *		or the published version when available.
 *	This reference is still required when using modified versions of the code.
 *  This notice should be maintained in modified versions of the code.
 *	No warranty is given regarding the correctness of this code.
 *==========================================================================*/


/*------------------------------ General Ziggurat --------------------------*/
static double DRanNormalTail(double dMin, int iNegative)
{
	double x, y;
	do
	{	x = log(DRanU()) / dMin;
		y = log(DRanU());
	} while (-2 * y < x * x);
	return iNegative ? x - dMin : dMin - x;
}

#define ZIGNOR_C 128			       /* number of blocks */
#define ZIGNOR_R 3.442619855899	/* start of the right tail */
				   /* (R * phi(R) + Pr(X>=R)) * sqrt(2\pi) */
#define ZIGNOR_V 9.91256303526217e-3

/* s_adZigX holds coordinates, such that each rectangle has*/
/* same area; s_adZigR holds s_adZigX[i + 1] / s_adZigX[i] */
static double s_adZigX[ZIGNOR_C + 1], s_adZigR[ZIGNOR_C];

static void zigNorInit(int iC, double dR, double dV)
{
	int i;	double f;
	
	f = exp(-0.5 * dR * dR);
	s_adZigX[0] = dV / f; /* [0] is bottom block: V / f(R) */
	s_adZigX[1] = dR;
	s_adZigX[iC] = 0;

	for (i = 2; i < iC; ++i)
	{
		s_adZigX[i] = sqrt(-2 * log(dV / s_adZigX[i - 1] + f));
		f = exp(-0.5 * s_adZigX[i] * s_adZigX[i]);
	}
	for (i = 0; i < iC; ++i)
		s_adZigR[i] = s_adZigX[i + 1] / s_adZigX[i];
}

double  DRanNormalZig(void)
{
	uint32_t i;
	double x, u, f0, f1;
	
	for (;;)
	{
		u = 2 * DRanU() - 1;
		i = IRanU() & 0x7F;
		/* first try the rectangular boxes */
		if (fabs(u) < s_adZigR[i])		 
			return u * s_adZigX[i];
		/* bottom box: sample from the tail */
		if (i == 0)						
			return DRanNormalTail(ZIGNOR_R, u < 0);
		/* is this a sample from the wedges? */
		x = u * s_adZigX[i];		   
		f0 = exp(-0.5 * (s_adZigX[i] * s_adZigX[i] - x * x) );
		f1 = exp(-0.5 * (s_adZigX[i+1] * s_adZigX[i+1] - x * x) );
      	if (f1 + DRanU() * (f0 - f1) < 1.0)
			return x;
	}
}
/*--------------------------- END General Ziggurat -------------------------*/

/*--------------------------- functions for testing ------------------------*/
double  DRanQuanNormalZig(void)
{
	return DProbNormal(DRanNormalZig());
}

/*------------------------- END functions for testing ----------------------*/


/*==========================================================================*/


/*==========================================================================
 *  General utility functions
 *  Martinus H. V. Werts, 2024
 *==========================================================================*/
 
 
void  RanInit(uint64_t uSeed)
{
	zigNorInit(ZIGNOR_C, ZIGNOR_R, ZIGNOR_V);
	RanSetSeed(uSeed);
}
