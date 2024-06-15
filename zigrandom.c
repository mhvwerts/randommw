/*==========================================================================
 *  Modified version of zigrandom.c
 *  Martinus H. V. Werts, 2024
 *
 *  - Fixed a gcc warning
 *  - Added MWC_52 random number generator, which is a full-precision
 *    generator of random doubles in (0, 1) from pairs of 64-bit unsigned
 *    integers coming from MWC_8222 (= MWC_256)
 *==========================================================================*/

/*==========================================================================
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

#include <limits.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "zigrandom.h"

/*---------------------------- GetInitialSeeds -----------------------------*/
void GetInitialSeeds(uint32_t auiSeed[], int32_t cSeed,
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
/*-------------------------- END GetInitialSeeds ---------------------------*/


/*------------------------ George Marsaglia MWC ----------------------------*/
#define MWC_R  256
#define MWC_A  809430660ull // unsigned long long is 64 bits
#define MWC_AI 809430660
#define MWC_C  362436
static uint32_t s_uiStateMWC = MWC_R - 1;
static uint32_t s_uiCarryMWC = MWC_C;
static uint32_t s_auiStateMWC[MWC_R];

void RanSetSeed_MWC8222(int *piSeed, int cSeed)
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
		GetInitialSeeds(s_auiStateMWC, MWC_R, piSeed && cSeed ? piSeed[0] : 0, 0);
	}
}
uint32_t IRan_MWC8222(void)
{
	uint64_t t;

	s_uiStateMWC = (s_uiStateMWC + 1) & (MWC_R - 1);
	t = MWC_A * s_auiStateMWC[s_uiStateMWC] + s_uiCarryMWC;
	s_uiCarryMWC = (uint32_t)(t >> 32);
	s_auiStateMWC[s_uiStateMWC] = (uint32_t)t;
    return (uint32_t)t;
}
double DRan_MWC8222(void)
{
	uint64_t t;

	s_uiStateMWC = (s_uiStateMWC + 1) & (MWC_R - 1);
	t = MWC_A * s_auiStateMWC[s_uiStateMWC] + s_uiCarryMWC;
	s_uiCarryMWC = (uint32_t)(t >> 32);
	s_auiStateMWC[s_uiStateMWC] = (uint32_t)t;
	return RANDBL_32new(t);
}
double DRan_MWC_52(void)
/* Generate random doubles with full-precision 52-bit mantissa using MWC8222 */
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


/*------------------- normal random number generators ----------------------*/
static int s_cNormalInStore = 0;		     /* > 0 if a normal is in store */

/* Default MWC_52 uniform generator (MWC8222 with 52 bits mantissa) */
static DRANFUN s_fnDRanu = DRan_MWC_52;
static IRANFUN s_fnIRanu = IRan_MWC8222;
static RANSETSEEDFUN s_fnRanSetSeed = RanSetSeed_MWC8222;

double  DRanU(void)
{
    return (*s_fnDRanu)();
}
uint32_t IRanU(void)
{
    return (*s_fnIRanu)();
}
void    RanSetSeed(int *piSeed, int cSeed)
{
   	s_cNormalInStore = 0;
	(*s_fnRanSetSeed)(piSeed, cSeed);
}
void    RanSetRan(const char *sRan)
{
   	s_cNormalInStore = 0;
	
	/* BEGIN if ... else if ... else block */
	if (strcmp(sRan, "MWC8222") == 0)
	{
		s_fnDRanu = DRan_MWC8222;
		s_fnIRanu = IRan_MWC8222;
		s_fnRanSetSeed = RanSetSeed_MWC8222;
	}
	else if (strcmp(sRan, "MWC_52") == 0)
	{
		s_fnDRanu = DRan_MWC_52;
		s_fnIRanu = IRan_MWC8222;
		s_fnRanSetSeed = RanSetSeed_MWC8222;
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
