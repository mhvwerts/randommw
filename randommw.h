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


/* PRNG interface */

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

/* Fully initialize PRNG */
void  RanInit(const char *sRan, uint64_t uSeed, uint64_t uJumpsize);
