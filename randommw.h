/*==========================================================================
 * randommw.h 
 * combines Doornik's zigrandom.h and zignor.h, with modifications and
 * extensions
 *
 * - MWC8222 has been renamed to MWC256
 * - Xoshiro256+ PRNG has been added (64 bit)
 * - Splitmix64 PRNG has been added (64 bit)
 * - MELG19937-64 PRNG has been added (64 bit)
 *
 * Werts, 2024
 *==========================================================================*/

#include <stdint.h>



/*--------------------------------------
 * from zigrandom.h, with modifications 
 *--------------------------------------*/
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


/* MELG19937-64 Harase & Kimoto */
void RanSetSeed_MELG19937(uint64_t uSeed);
uint32_t IRan_MELG19937(void);
double DRan_MELG19937(void);
void RanJump_MELG19937(uint64_t uJumps); 

/* Xoshiro256+ Blackman & Vigna */
void RanSetSeed_xoshiro256p(uint64_t uSeed);
uint32_t IRan_xoshiro256p(void);
double DRan_xoshiro256p(void);
void RanJump_xoshiro256p(uint64_t uJumps); 

/* Splitmix64 */
void RanSetSeed_splitmix64(uint64_t uSeed);
uint32_t IRan_splitmix64(void);
double DRan_splitmix64(void);

/* MWC256 (aka MWC8222) George Marsaglia */
void RanSetSeed_MWC256(uint64_t uSeed);
uint32_t IRan_MWC256(void);
double DRan_MWC256(void);

/* plug-in RNG */
typedef double 		( * DRANFUN)(void);
typedef uint32_t 	( * IRANFUN)(void);
typedef void   		( * RANSETSEEDFUN)(uint64_t);
typedef void   		( * RANJUMPFUN)(uint64_t);

void    RanSetRan(const char *sRan);
void    RanSetRanExt(DRANFUN DRanFun, IRANFUN IRanFun, 
		             RANSETSEEDFUN RanSetSeedFun, RANJUMPFUN RanJumpFun);
void    RanSetSeed(uint64_t uSeed);
void    RanJumpRan(uint64_t uJumpsize);
double  DRanU(void);
uint32_t  IRanU(void);

/* from zignor.h */
double  DRanNormalZig(void);

/* further extensions */
void  RanInit(uint64_t uSeed);
