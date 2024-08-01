/*==========================================================================
 * randommw.h 
 * combines Doornik's zigrandom.h and zignor.h, with modifications
 *
 * Some elements have been moved to randommw.c and made static.
 *
 * Werts, 2024
 *==========================================================================*/

#include <stdint.h>


/* PRNG interface */
typedef double 		( * DRANFUN)(void);
typedef uint32_t 	( * U32RANFUN)(void);
typedef void   		( * RANSETSEEDFUN)(uint64_t);
typedef void   		( * RANJUMPFUN)(uint64_t);
typedef void		( * RANSEEDJUMPFUN)(uint64_t, uint64_t);

void    RanSetRan(const char *sRan);
void    RanSetRanExt(DRANFUN DRanFun, U32RANFUN U32RanFun, 
		             RANSETSEEDFUN RanSetSeedFun, RANJUMPFUN RanJumpFun,
					 RANSEEDJUMPFUN RanSeedJumpFun);
void    RanSetSeed(uint64_t uSeed);
void    RanJumpRan(uint64_t uJumpsize);
void	RanSeedJump(uint64_t uSeed, uint64_t uJumpsize);
double  DRanU(void);
uint32_t  U32RanU(void);

/* from zignor.h */
double  DRanNormalZig(void);

/* Fully initialize PRNG */
void  RanInit(const char *sRan, uint64_t uSeed, uint64_t uJumpsize);
