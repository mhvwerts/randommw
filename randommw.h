/*==========================================================================
 * randommw.h 
 * combines Doornik's zigrandom.h and zignor.h, with modifications and
 * extensions
 *
 * Werts, 2024
 *==========================================================================*/




/* from zigrandom.h */
#ifndef ZIGRANDOM_H
#define ZIGRANDOM_H

#include <stdint.h>

#define M_RAN_INVM30	9.31322574615478515625e-010			  /* 1.0 / 2^30 */
#define M_RAN_INVM32	2.32830643653869628906e-010			  /* 1.0 / 2^32 */
#define M_RAN_INVM48	3.55271367880050092936e-015			  /* 1.0 / 2^48 */
#define M_RAN_INVM52	2.22044604925031308085e-016			  /* 1.0 / 2^52 */
#define M_RAN_INVM64	5.42101086242752217004e-020			  /* 1.0 / 2^64 */

#define RANDBL_32new(iRan1)                   \
    ((int)(iRan1) * M_RAN_INVM32 + (0.5 + M_RAN_INVM32 / 2))
#define RANDBL_48new(iRan1, iRan2)            \
    ((int)(iRan1) * M_RAN_INVM32 + (0.5 + M_RAN_INVM48 / 2) + \
        (int)((iRan2) & 0x0000FFFF) * M_RAN_INVM48)
#define RANDBL_52new(iRan1, iRan2)            \
    ((int)(iRan1) * M_RAN_INVM32 + (0.5 + M_RAN_INVM52 / 2) + \
        (int)((iRan2) & 0x000FFFFF) * M_RAN_INVM52)

/* xoshiro256+ */
void RanSetSeed_xoshiro256p(uint64_t uSeed);
uint32_t IRan_xoshiro256p(void);
double DRan_xoshiro256p(void);


/* MWC8222 George Marsaglia */

/* void 	GetInitialSeeds(uint32_t auiSeed[], int32_t cSeed,
	                    uint32_t uiSeed, uint32_t uiMin);
*/

void RanSetSeed_MWC8222(uint64_t uSeed);
uint32_t IRan_MWC8222(void);
double DRan_MWC8222(void);
double DRan_MWC_52(void);

/* plug-in RNG */
typedef double 		( * DRANFUN)(void);
typedef uint32_t 	( * IRANFUN)(void);
typedef void   		( * RANSETSEEDFUN)(uint64_t);

void    RanSetRan(const char *sRan);
void    RanSetRanExt(DRANFUN DRanFun, IRANFUN IRanFun, RANSETSEEDFUN RanSetSeedFun);
double  DRanU(void);
uint32_t  IRanU(void);
void    RanSetSeed(uint64_t uSeed);

/* normal probabilities */
double  DProbNormal(double x);

/* polar standard normal RNG */
double  DRanNormalPolar(void);
double  FRanQuanNormal(void);
double  DRanQuanNormal(void);

#endif /* ZIGRANDOM_H */


/* from zignor.h */
double  DRanNormalZig(void);
double  DRanQuanNormalZig(void);

/* xoshiro256 */

/* further extensions */
void  RanInit(uint64_t uSeed);
