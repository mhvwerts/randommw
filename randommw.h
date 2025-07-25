/*==========================================================================
 *==========================================================================
 * Random number generators with uniform and Gaussian dstributions
 * 
 * A re-mix of various tried and tested routines, refactored into a single
 * header-only source file, with a coherent interface giving access to all 
 * practical aspects of random number generation for scientific 
 * applications, while still being convenient to use.
 * 
 * by Martinus H. V. Werts, 2024-2025, with selected code from various 
 * authors cited below.
 *
 * See `README.md` for further information on the scope and background of
 * this software.
 *
 * Distributed under the CeCILL-C license (see LICENSE)
 *
 *==========================================================================
 *
 * This header-only source file is organized as follows.
 *
 * 0. Definitions and prototypes
 * A. MELG19937-64 PRNG by Harase & Kimoto
 * B. Xoshiro256+ PRNG by Vigna & Blackman, and the splitmix64 PRNG (for
 *    initialization of other PRNGs using a single 64-bit seed)
 * C. Lehmer64 PRNG from Lemire
 * D. PCG64DXSM by O'Neill
 * E. MWC8222 by Marsaglia, from Doornik's 'zigrandom.c' (Marsaglia originally
      called this generator MWC256, but there is now a different algorithm
	  with that name.)
 * F. Doornik's ziggurat algorithm for generation of normally distributed 
 *    random numbers (from 'zignor.c')
 * G. Additional functionality: 'zigtimer.c' timing functions etc.
 *
 *==========================================================================
 *==========================================================================*/

#ifndef RANDOMMW_H
#define RANDOMMW_H



#ifdef __cplusplus
extern "C" {
#endif



#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>
#include <string.h>

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

double  DRanNormalZig(void);

void  RanInit(const char *sRan, uint64_t uSeed, uint64_t uJumpsize);

void 	StartTimer(void);
void 	StopTimer(void);
char * GetLapsedTime(void);

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


/* static definitions

   External interface to PRNG should go via
   RanInit, DranU, U32RanU, DRanNormalZig
   
*/


/* MELG19937-64 Harase & Kimoto */
static void RanSetSeed_MELG19937(uint64_t uSeed);
static uint32_t U32Ran_MELG19937(void);
static double DRan_MELG19937(void);
static void RanJump_MELG19937(uint64_t uJumps);
static void RanSeedJump_MELG19937(uint64_t uSeed, uint64_t uJumpsize);

/* Xoshiro256+ Blackman & Vigna */
static void RanSetSeed_xoshiro256p(uint64_t uSeed);
static uint32_t U32Ran_xoshiro256p(void);
static double DRan_xoshiro256p(void);
static void RanJump_xoshiro256p(uint64_t uJumps);
static void RanSeedJump_xoshiro256p(uint64_t uSeed, uint64_t uJumpsize);

/* Lehmer64 */
static void RanSetSeed_lehmer64(uint64_t uSeed);
static uint32_t U32Ran_lehmer64(void);
static double DRan_lehmer64(void);
static void RanSeedJump_lehmer64(uint64_t uSeed, uint64_t uJumpsize);

/* PCG64DXSM by O'Neill */
static void RanSetSeed_pcg64dxsm(uint64_t uSeed);
static uint32_t U32Ran_pcg64dxsm(void);
static double DRan_pcg64dxsm(void);
static void RanJump_pcg64dxsm(uint64_t uJumps);
static void RanSeedJump_pcg64dxsm(uint64_t uSeed, uint64_t uJumpsize);

/* MWC8222 George Marsaglia */
static void RanSetSeed_MWC8222(uint64_t uSeed);
static uint32_t U32Ran_MWC8222(void);
static double DRan_MWC8222(void);
static void RanSeedJump_MWC8222(uint64_t uSeed, uint64_t uJumpsize);

/* Splitmix64 for internal use */
static void RanSetSeed_splitmix64(uint64_t uSeed);
static uint32_t U32Ran_splitmix64(void);
// static double DRan_splitmix64(void); // not needed



/*==========================================================================
 * MELG19937-64 pseudo-random number generator
 *
 * 64-bit maximally equidistributed pseudorandom number generator 
 * of the "Mersenne Twister" type
 *
 * see: https://github.com/sharase/melg-64
 *
 * Original code by Harase & Kimoto with cosmetic changes (renaming of 
 * variables and functions) and routines for interfacing to zigrandom.c
 *
 * Modifications by M. H. V. Werts, 2024
 *
 *==========================================================================*/

/* ***************************************************************************** */
/* A C-program for MELG19937-64                                                  */
/* Copyright:   Shin Harase, Ritsumeikan University                              */
/*              Takamitsu Kimoto                                                 */
/* Notice:      This code can be used freely for personal, academic,             */
/*              or non-commercial purposes. For commercial purposes,             */
/*              please contact S. Harase at: harase @ fc.ritsumei.ac.jp          */
/* Reference:   S. Harase and T. Kimoto, "Implementing 64-bit maximally          */ 
/*              equidistributed F2-linear generators with Mersenne prime period",*/ 
/*              ACM Transactions on Mathematical Software, Volume 44, Issue 3,   */ 
/*              April 2018, Article No. 30, 11 Pages.                            */
/* Remark:      We recommend using the most significant bits (not taking the     */
/*              least significant bits) because our generators are optimized     */
/*              preferentially from the most significant bits,                   */
/*              see Remark 4.1 in the above paper for details.                   */
/* ***************************************************************************** */

#define NN 311 // N-1
#define MM 81 // M
#define MATRIX_A 0x5c32e06df730fc42ULL
#define P 33 // W-r
#define W 64
#define MASKU (0xffffffffffffffffULL << (W-P))
#define MASKL (~MASKU)
#define MAT3NEG(t, v) (v ^ (v << ((t))))
#define MAT3POS(t, v) (v ^ (v >> ((t))))
#define LAG1 19 // L
#define SHIFT1 16 // s_3
#define MASK1 0x6aede6fd97b338ecULL // b
#define LAG1over 292 // NN-LAG1

static uint64_t melg[NN]; 
static int melgi;
static uint64_t lung; //extra state variable
static uint64_t mag01[2]={0ULL, MATRIX_A};
static uint64_t x;

static uint64_t melg_case_1(void);
static uint64_t melg_case_2(void);
static uint64_t melg_case_3(void);
static uint64_t melg_case_4(void);
uint64_t (*melg_next_uint64)(void);

struct melg_state{
	uint64_t lung;
	uint64_t melg[NN];
	int melgi;
	uint64_t (*function_p)(void);
};

void melg_jump(void); //jump ahead by 2^256 steps
static void melg_add(struct melg_state *state);

/* initializes melg[NN] and lung with a seed */
void melg_init_uint64seed(uint64_t seed)
{
    melg[0] = seed;
    for (melgi=1; melgi<NN; melgi++) {
        melg[melgi] = (6364136223846793005ULL * (melg[melgi-1] ^ (melg[melgi-1] >> 62)) + melgi);
    }
    lung = (6364136223846793005ULL * (melg[melgi-1] ^ (melg[melgi-1] >> 62)) + melgi);
    melgi = 0;
    melg_next_uint64 = melg_case_1;
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
void melg_init_uint64array(uint64_t init_key[],
		     uint64_t key_length)
{
	uint64_t i, j, k;
    melg_init_uint64seed(19650218ULL);
    i=1; j=0;
    k = (NN>key_length ? NN : key_length);
    for (; k; k--) {
        melg[i] = (melg[i] ^ ((melg[i-1] ^ (melg[i-1] >> 62)) * 3935559000370003845ULL))
          + init_key[j] + j; /* non linear */
        i++; j++;
        if (i>=NN) { melg[0] = melg[NN-1]; i=1; }
        if (j>=key_length) j=0;
    }
    for (k=NN-1; k; k--) {
        melg[i] = (melg[i] ^ ((melg[i-1] ^ (melg[i-1] >> 62)) * 2862933555777941757ULL))
          - i; /* non linear */
        i++;
        if (i>=NN) { melg[0] = melg[NN-1]; i=1; }
    }
    lung = (lung ^ ((melg[NN-1] ^ (melg[NN-1] >> 62)) * 2862933555777941757ULL))
	  - NN; /* non linear */
    melg[0] = (melg[0] | (1ULL << 63)); /* MSB is 1; assuring non-zero initial array. Corrected.  */
    melgi = 0;
}

static uint64_t melg_case_1(void) {
    x = (melg[melgi] & MASKU) | (melg[melgi+1] & MASKL);
    lung = (x >> 1) ^ mag01[(int)(x & 1ULL)] ^ melg[melgi+MM] ^ MAT3NEG(23, lung);
    melg[melgi] = x ^ MAT3POS(33, lung);
    x = melg[melgi] ^ (melg[melgi] << SHIFT1);
    x = x ^ (melg[melgi + LAG1] & MASK1);
    ++melgi;
    if (melgi == NN - MM) melg_next_uint64 = melg_case_2;
    return x;
}

static uint64_t melg_case_2(void) {
    x = (melg[melgi] & MASKU) | (melg[melgi+1] & MASKL);
    lung = (x >> 1) ^ mag01[(int)(x & 1ULL)] ^ melg[melgi+(MM-NN)] ^ MAT3NEG(23, lung);
    melg[melgi] = x ^ MAT3POS(33, lung);
    x = melg[melgi] ^ (melg[melgi] << SHIFT1);
    x = x ^ (melg[melgi + LAG1] & MASK1);
    ++melgi;
    if (melgi == LAG1over) melg_next_uint64 = melg_case_3;
    return x;
}

static uint64_t melg_case_3(void) {
    x = (melg[melgi] & MASKU) | (melg[melgi+1] & MASKL);
    lung = (x >> 1) ^ mag01[(int)(x & 1ULL)] ^ melg[melgi+(MM-NN)] ^ MAT3NEG(23, lung);
    melg[melgi] = x ^ MAT3POS(33, lung);
    x = melg[melgi] ^ (melg[melgi] << SHIFT1);
    x = x ^ (melg[melgi - LAG1over] & MASK1);
    ++melgi;
    if (melgi == NN-1) melg_next_uint64 = melg_case_4;
    return x;
}

static uint64_t melg_case_4(void) {
    x = (melg[NN-1] & MASKU) | (melg[0] & MASKL);
    lung = (x >> 1) ^ mag01[(int)(x & 1ULL)] ^ melg[MM-1] ^ MAT3NEG(23, lung);
    melg[NN-1] = x ^ MAT3POS(33, lung);
    x = melg[melgi] ^ (melg[melgi] << SHIFT1);
    x = x ^ (melg[melgi - LAG1over] & MASK1);
    melgi = 0;
    melg_next_uint64 = melg_case_1;
    return x;
}

/*----------------------------------------------------------------
  Original melg19337-4.c type conversion functions
  ---------------------------------------------------------------*/

/*-----

// generates a random number on [0, 2^63-1]-interval 
long long melg_next_int63(void)
{
    return (long long)(melg_next_uint64() >> 1);
}

// generates a random number on [0,1]-real-interval 
double melg_next_real1(void)
{
    return (melg_next_uint64() >> 11) * (1.0/9007199254740991.0);
}

// generates a random number on [0,1)-real-interval
double melg_next_real2(void)
{
    return (melg_next_uint64() >> 11) * (1.0/9007199254740992.0);
}

// generates a random number on (0,1)-real-interval
double melg_next_real3(void)
{
    return ((melg_next_uint64() >> 12) + 0.5) * (1.0/4503599627370496.0);
}

// generates a random number on [0,1)-real-interval using a union trick
double melg_next_fast_res52(void)
{
    union {
	uint64_t u;
	double d;
    } conv;
	
	conv.u = (melg_next_uint64() >> 12) | 0x3FF0000000000000ULL;
	return (conv.d - 1.0);
}

// generates a random number on (0,1)-real-interval using a union trick
double melg_next_fast_res52_open(void)
{
    union {
	uint64_t u;
	double d;
    } conv;
	
	conv.u = (melg_next_uint64() >> 12) | 0x3FF0000000000001ULL;
	return (conv.d - 1.0);
}

// generates a random number on [0,1)-real-interval with 53-bit significant bits
double melg_next_res53(void)
{
	return (melg_next_uint64() >> 11) * 0x1.0p-53;
}

 *-----*/


/* This is a jump function for the generator. It is equivalent
   to 2^256 calls to melg_next_uint64(). */
void melg_jump(void)
{
	struct melg_state *melg_state_init;
	int i, j;
	int bits, mask;
	
	//jump size 2^256
	char jump_string[] = 
        "1510de5f1aeb1b349b7d2f3dc278bf1e6358d09c083c53b2b5"
        "2b0b37aa42ec96ae92d9199e5ddb4f8f19419a1ae8d41d208c"
        "c209439db14c17bc032c1aa482b589174bb3ac3964a128c742"
        "017ff511a9ddd720f397969f0c4dc862608725d5465dd0d257"
        "99d29ff579515657f3b7f58f5f6090d3c2c283b9e1cc517b48"
        "d4df4f03db955624557939ba23ff0b68b195a7a7413dcb3029"
        "25711acc4fbc5554193ddcf43bfd9deeda0e3a684770ef6b11"
        "b8129f937e0c41e8c7c435bb76c6ca0518d6cd8809410c33a5"
        "f5f39573f7ed9479abe9a5ee7bf09e189b1737f6fe53897026"
        "d792327de7e2c9ca050fa66f23eab9a0a83b67a9e6d54d70ce"
        "46664dbc4af7cee88756fc50f16b841b76167c66613ef43b00"
        "b775aeed0e260fde67da03f6051ba11dbfa2070447f3aba151"
        "e001404a11d3049e53f177ee4c275cffcf4c6e5c7b8a1e8db0"
        "86731abb01ea50ec8440bc45fdd3c23679a68b29b2457d0013"
        "878d8a7f1dccc595f99e656b64da2715a392eb68a517989be2"
        "4c663dcbfb663ff38c567fa6b5fe8bdccbd30163524a9a1d63"
        "cf609eb93a1fe3cca5e1220bd05e4dcb611a459d6ee70bbf57"
        "86d6fb887aea96e70e78af7f50dcbc638664ac28efcab6356d"
        "ed959bb79355c5bc5e189a20bb8f64e5fcb444c2f29c57fce7"
        "a70208115da1b8a663c8062cbc98e353526b1d72371c07fb0c"
        "ad50a923eef2c5c865d733be91978e1279cc45ea20f534e428"
        "422f72c30957e7fab79da909526d097b4a3a790c2b3cae28ef"
        "52e5eb4302858110e1bcc31187bdbf79012e770ff95126a7a0"
        "4b4059e2a9f9f885a6af3d5d067148e05bdd01bdc8f7a33b47"
        "5631f89a08e92e61a25618846b55a2f42ab42c56ce3d3948fd"
        "f515b90b344f726bfe8543a93367cd5d95b08d4da0bcc7b2fc"
        "65384a51eb16766ee2ee3bdf82b6cf24c7a81e826d2e9f81e8"
        "1917ead9c3ca2b0ea0a2395cf4804080dd0cbf4698e412b7a2"
        "49ddc89bc939e34857437be5fc1586f932a0a10c48121eb5e8"
        "3a1d4e4bd682d9674d6d42f8ec190dada2ba9c4c0c25392b1c"
        "fc32916c9f7dd5978badc53796d2c2843880adfaff7d83b73c"
        "5959b9a7424715d2f7a47e1c0363c7d3f60c332c8bb39b8656"
        "08c1035c2773f53a0edc2582182a5cffaa5acd15820daeff16"
        "58c64ac4b579f8134fd1db297c1d4d4dd03b4f063a293a2cbd"
        "a3aaf381e6cf54a0cd949e5ed2473852484566db89de18654d"
        "8efa020ed963c9d26dbba50a3de5f0c3b6e72b477c8f26284d"
        "cf561c3df5780cef6197039cc076391022a0d57845e992e3b5"
        "2189c95e92172461838b14f014f452ab24460be82113d41f31"
        "47e210c03f8430b223836d1efe5ef96bf56708dbad033d57fa"
        "74beb1314c1abf1b328b4145c359bc4b6befc94c6bec8762f5"
        "feaa4f14f309e5e51415479d1f16821528b707599eb530a898"
        "6b751ccce0d17055894116cd032af55860af016dff76fa14ce"
        "b606c4b277f5968f897d91b544db7cf0de9fb237d599000751"
        "7e0aab7a73866d498e76f772006d3bf2387c552ba3d72e3a6a"
        "a324edeea5989a45b0468ec514127156141de06e22c78347d6"
        "dc48c07dd42b1a9c543deed9006daa8ae676dc328f7dbc5d90"
        "02d2f481f9cc4c7b9a433377bf61d0d75eae143ff8c7e7e0f0"
        "9a805ee12e187c02724a9c5e6789dd2a5300753bdfcc1c964c"
        "818d2a45e13e4ba89ea90fdd45b40a1b76079cbcbfc717162e"
        "b27d7a902f213646ed65e7f00e5fbc0cd74bb099e00ed350b4"
        "93225e88e5693d999244b8d0f1f9bbfad03e5223416fd790bc"
        "c6e047abd1523245c6a46d397f63b38ecebaf79234b53b9b02"
        "374cdf7bcaa9558043e1018eb14ec31b1fb56a7e6aa6730108"
        "12cf5abc0ed2ec1df75a615632f59968a92de6cc183c4c1555"
        "3fe5ca263cf3cffd1342e60975ac2de843f5b5a6314e382dd6"
        "a6887b87e29f9b31b0d7a2dc31e9f07212fa0c2e69db50d30b"
        "d676460a94a9822f5aaf5af01bc566136da7138ba69554577a"
        "2ef2f5d91051ec7ee3645a0df47bbea49e2a47c1279e3510e0"
        "8c89c9d5b20966125b582469b13d99308119423dab451f29b8"
        "b4f6ebeff94a06c74d9f6e040c269c39b1c5942cd96f812b35"
        "b047357ddb08863649a13cb38a4e10d047b8aa84a81870de3c"
        "d774a4b6174291bc3731437aefa7dbbf2af9c497dec0a90a36"
        "55395944fc6a0c3e46326a10d905fbd5cd90ccd46baac32cff"
        "4f6e48936de047e3eb24cf7e7e64ac7616ed8fe0ad751daee7"
        "bc8e09ab4447718355e92fbd583a3165466d722c4fb0f904d8"
        "65b77b99053db2709ae3c721b714ae8bbdac87fc0b81a5c5dd"
        "c2e042e3155801276efc14e508e5fff27ad21ff1c975657373"
        "20b1344df216188bb3872a28c11ecc1aabce8cdf9749b6bc67"
        "39628e3f35b531a32dac218196becb2945904b35079ce2bbd9"
        "7f811fb71c2fa1d9cc5ea65a9d88ee77ab2a52e48e8aaf4e4d"
        "91679618ffe441b8c319bf6c6589e118f3abd0f8c22fc930af"
        "64e1b0e4616c1f5f94c50ea240ea8cdd7d57f9b7ee11c3516f"
        "16115bc995e586f3483ca5be4bbf1c1fe4578934f77c03e307"
        "f6096854e9a93d28cd7331ce91371a2f50ae608d1f0348f8ce"
        "3ce48eaaf83f7195ea7b3fbcf4b331d4a2c7f21843b745164e"
        "4b71678b8ea41580feef7db43f090915ec7edae77eb058d37f"
        "a04571f4bad32d08d364301a7f0fc633fdfe3f9695f0edf8de"
        "2187dee171988c47da64da030fcbcfd8fc3b77a59943d46927"
        "c869e6065b237a0d9e32a72cf0e15ae969b0672a5f5835cdba"
        "88ce9173abe094d95ae7acee85e176fb826b9ffe01ca860f95"
        "06540e6f415a9c5ba8ad9a8dd306188fc1973dcd33f75c4b58"
        "f5d6a6df6a5ed88f4514690dee844b77c5fc6bb2090d5b6364"
        "fc31b0ec50e29cca44752024bc3270f553570ac196066eb1f0"
        "4e09be04b7301a915080ebeaea4c749c04f2d4cf79c5805d08"
        "beb34b966fbc5e153f80a00101883c93861bbee60c52470053"
        "546aeb57e487092b60884ab20f738f87c9ab6bca2a3370ffaf"
        "745ccbc44bae13befd29deacddb38d0124e02ef8aa656a87f7"
        "47e0deac35e7fe2f191ed119a6908a909222deffb028e5e12f"
        "ea7c3be122fb684ebf83f8adcba142affa7753e27370b493fe"
        "d258a4db5068042a9e4db38d160f388f4064dfd13b3bbfe95b"
        "cd6176ce99fef56573fc8141bc4a290202b2437df2886f2dcf"
        "b693d3110b78220a7007b695bfda744a356cbce15814d2eaf7"
        "1e322e9542d4933c7051e83f5a1636c72bda12822d803ca4da"
        "a66e5baa793271a6b301d1ec7a818a4b5ddca7d1141d830883"
        "cd1586b50b0cdee0f4d445752b2716b5cc44d8b2e1149b4ec4"
        "ca06f87fa7be9b4aad509804b64f3edebba10fc687f20d238a"
        "39f3b219c2e8f8f6f3533671843a521a457df1dbccc54b624b"
        "a0609fed10acfb9b3442bbf93f5689415d4243a06f53958e06"
        "f28b7b4e5d08ea178bc92eee27adb94f002b7d0bbc0da40075"
        "2421ab4edcce592d9996d2472b967043d20";
	
	/*allocates melg_state_init*/
	melg_state_init = (struct melg_state *)malloc(sizeof(struct melg_state));
	
	/*initializes melg_state_init*/
	melg_state_init->lung = 0ULL;
	for(i = 0; i < NN; i++) melg_state_init->melg[i] = 0ULL;
	melg_state_init->melgi = melgi;
	melg_state_init->function_p = melg_next_uint64;
	
	for (i = 0; i < ceil((double)(NN*W+P)/4); i++) {
	bits = jump_string[i];
	if (bits >= 'a' && bits <= 'f') {
	    bits = bits - 'a' + 10;
	} else {
	    bits = bits - '0';
	}
	bits = bits & 0x0f;
	mask = 0x08;
	for (j = 0; j < 4; j++) {
	    if ((bits & mask) != 0) {
			melg_add(melg_state_init);
			}
			melg_next_uint64();
			mask = mask >> 1;
		}
	}
	
	/*updates the new initial state*/
	lung = melg_state_init->lung;
	for(i = 0; i < NN; i++) melg[i] = melg_state_init->melg[i];
	melgi = melg_state_init->melgi;
	melg_next_uint64 = melg_state_init->function_p;
	
	free(melg_state_init);
}

static void melg_add(struct melg_state *state)
{
	int i;
	int n1, n2;
	int diff1, diff2;
	
	/*adds the lung*/
	state->lung ^= lung;
	
	n1 = state->melgi;
	n2 = melgi;

	/*adds the states*/
	if(n1 <= n2)
	{
		diff1 = NN - n2 + n1;
		diff2 = n2 - n1;
		
		for(i = n1; i < diff1; i++)
			state->melg[i] ^= melg[i + diff2];
		
		for(; i < NN; i++)
			state->melg[i] ^= melg[i - diff1];

		for(i = 0; i < n1; i++)
			state->melg[i] ^= melg[i + diff2];
	} else {
		diff1 = NN - n1 + n2;
		diff2 = n1 - n2;
		
		for(i = n1; i < NN; i++)
			state->melg[i] ^= melg[i - diff2];
		
		for(i = 0; i < diff2; i++)
			state->melg[i] ^= melg[i + diff1];
	
		for(; i < n1; i++)
			state->melg[i] ^= melg[i - diff2];
	}
}


/*----------------------------------------------------------------
 * Interface between MELG19937 and zigrandom
 *----------------------------------------------------------------*/

static void RanSetSeed_MELG19937(uint64_t uSeed)
{
	melg_init_uint64seed(uSeed);
}

static void RanJump_MELG19937(uint64_t uJumps)
{
	uint64_t i;
	for (i=0; i<uJumps; i++)
		melg_jump();
}

static void RanSeedJump_MELG19937(uint64_t uSeed, uint64_t uJumpsize)
{
	RanSetSeed_MELG19937(uSeed);
	if (uJumpsize > 0)
	{
		RanJump_MELG19937(uJumpsize);
	}
}

/* The 32-bit unsigned integer U32Ran random routine uses only
   the upper 32 bits of MELG19937. */
static uint32_t U32Ran_MELG19937(void)
{
	return (uint32_t)(melg_next_uint64() >> 32);
}

/* This is the same uint64 to (0,1) double converter as used
   for Xoshiro256
   
   We may also test the conversion routines included in the
   MELG code (compare for speed, may be less portable) */ 
static double DRan_MELG19937(void)
{
	uint64_t xx;
	
	/*
	xx = 0;
	while (xx == 0)
		xx = (melg_next_uint64() >> 11);
	*/
	
	while ((xx = (melg_next_uint64() >> 11)) == 0)
		;
	
	return (xx * 0x1.0p-53);
}



/*==========================================================================
 * xoshiro256+ and splitmix64
 *
 * see: https://prng.di.unimi.it/
 *
 *
 * Xoshiro256+ for the generation of 32-bit unsigned integers 
 * and 53-bit mantissa doubles
 * 
 * Splitmix64 will be used internally for initialisation of other PRNGs
 * from a single 64-bit initial seed. Splitmix64 passes statistical tests
 * (https://github.com/lemire/testingRNG), and has only 64 bits of state.
 *
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
	for(unsigned int i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
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
	for(unsigned int i = 0; i < sizeof LONG_JUMP / sizeof *LONG_JUMP; i++)
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
 * from a single 64-bit seed, and also to initialize other PRNGs
 *
 * It has been reported to pass statistical tests
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

static void RanSetSeed_xoshiro256p(uint64_t uSeed)
{
	RanSetSeed_splitmix64(uSeed); // seed splitmix
	
	// use splitmix to fully seed xoshiro256p
	xoshiro256p_s[0] = splitmix64_next();
	xoshiro256p_s[1] = splitmix64_next();
	xoshiro256p_s[2] = splitmix64_next();
	xoshiro256p_s[3] = splitmix64_next();
}

static void RanJump_xoshiro256p(uint64_t uJumps)
{
	uint64_t i;
	for (i=0; i<uJumps; i++)
		xoshiro256p_long_jump();
}

static void RanSeedJump_xoshiro256p(uint64_t uSeed, uint64_t uJumpsize)
{
	RanSetSeed_xoshiro256p(uSeed);
	if (uJumpsize > 0)
	{
		RanJump_xoshiro256p(uJumpsize);
	}
}

/* The 32-bit unsigned integer U32Ran random routine uses only
   the upper 32 bits of Xoshiro256+, which are of highest
   random quality, and should pass all randomness tests. */
static uint32_t U32Ran_xoshiro256p(void)
{
	return (uint32_t)(xoshiro256p_next() >> 32);
}

static double DRan_xoshiro256p(void)
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
 * Interface to splitmix64
 *----------------------------------------------------------------*/

static void RanSetSeed_splitmix64(uint64_t uSeed)
{
	splitmix64_x = uSeed; // seed splitmix
}

static uint32_t U32Ran_splitmix64(void)
{
	return (uint32_t)(splitmix64_next() >> 32);
}

/*
//   unused legacy function
double DRan_splitmix64(void)
{
	uint64_t xx;
	
	while ((xx = (splitmix64_next() >> 11)) == 0)
		;
	
	return (xx * 0x1.0p-53);
}
*/

/*==========================================================================*/



/*==========================================================================
 *  Lehmer generator provided by D. Lemire, 2019
 *
 *  It generates 64-bit unsigned integers using a 128-bit state, and
 *  has been reported to pass BigCrush
 *
 *  https://lemire.me/blog/2019/03/19/the-fastest-conventional-random-number-generator-that-can-pass-big-crush/
 
 *  https://github.com/lemire/testingRNG/blob/master/source/lehmer64.h
 *  
 *
 *  Adapted by M. H. V. Werts, 2024
  ==========================================================================*/


static __uint128_t g_lehmer64_state;

/*
* D. H. Lehmer, Mathematical methods in large-scale computing units.
* Proceedings of a Second Symposium on Large Scale Digital Calculating
* Machinery;
* Annals of the Computation Laboratory, Harvard Univ. 26 (1951), pp. 141-146.
*
* P L'Ecuyer,  Tables of linear congruential generators of different sizes and
* good lattice structure. Mathematics of Computation of the American
* Mathematical
* Society 68.225 (1999): 249-260.
*/


/*
// replaced by RanSetSeed_lehmer64()

static inline void lehmer64_seed(uint64_t uSeed)
{
	RanSetSeed_splitmix64(uSeed); // seed Splitmix64
	// initialize the 128-bit state using 2x Splitmix64
	g_lehmer64_state = (((__uint128_t)splitmix64_next()) << 64) +
                          splitmix64_next();
}
*/

static inline uint64_t lehmer64()
{
	g_lehmer64_state *= UINT64_C(0xda942042e4dd58b5);
	return g_lehmer64_state >> 64;
}


/*----------------------------------------------------------------
 * Interface between lehmer64 and zigrandom
 *  based on the xoshiro256+ interface
 *----------------------------------------------------------------*/

static void RanSetSeed_lehmer64(uint64_t uSeed)
{
	RanSeedJump_lehmer64(uSeed, 0);
}

/* Lehmer64 does not support 'jumps'

   Here, we provide a mechanism to obtain a differently 
   initialized Lehmer64 from the same seed, but using 'uJumpsize'
   to forward the initializing Splitmix generator.
   
   There is a tiny, tiny chance that the different streams are
   not completely independent. This should still be tested exhaustively,
   but at present it seems the best method to obtain reproducible
   independent streams with this generator.

*/
static void RanSeedJump_lehmer64(uint64_t uSeed, uint64_t uJumpsize)
{
	unsigned int i;

	RanSetSeed_splitmix64(uSeed); // seed Splitmix64
	
	// forward the Splitmix generator
	for (i = 0; i < uJumpsize; i++)
	{
		splitmix64_next();
		splitmix64_next();
	}
	
	// initialize the 128-bit state using 2x Splitmix64
	g_lehmer64_state = (((__uint128_t)splitmix64_next()) << 64) +
                          splitmix64_next();
}

static uint32_t U32Ran_lehmer64(void)
{
	return (uint32_t)(lehmer64() >> 32);
}

static double DRan_lehmer64(void)
{
	uint64_t xx;
	
	while ((xx = (lehmer64() >> 11)) == 0)
		;
	
	return (xx * 0x1.0p-53);
}

/*==========================================================================*/




/*==========================================================================*
	PCG64DXSM by Melissa O'Neill
	Based on the Numpy C version by M. O'Neill & R. Kern
 *==========================================================================*/

/*
 * PCG64DXSM Random Number Generation for C. 
 *
 * Modified code, based on Numpy code 
 *  https://github.com/numpy/numpy/blob/main/numpy/random/src/pcg64/pcg64.c
 *  https://github.com/numpy/numpy/blob/main/numpy/random/src/pcg64/pcg64.h
 *  (August 2024)
 *
 * Copyright 2014 Melissa O'Neill <oneill@pcg-random.org>
 * Copyright 2015 Robert Kern <robert.kern@gmail.com>
 *
 * Modifications 2024  Martinus Werts <martinus.werts@univ-angers.fr> 
 * - Code contains exclusively the PCG64DXSM generator, all other generator
 *   code has been removed. 
 * - Emulation code for 128-bit operations has been removed. The code now
 *   explicitly requires `__uint128_t` to be supported by the compiler.
 *   gcc and clang support this.
 * - This version was tested and confirmed to give the same random number
 *   sequence and state changes as Python/Numpy's PCG64DXSM generator, 
 *   also when 'jumping'. This was done for 
 *       rng = numpy.random.PCG64DXSM(seed = 0)
 *   by setting the state/increment to exactly the same values as in
 *   Numpy before the sequence generation and jumps.
 * - Initialization/seeding of PCG64DXSM in Numpy depends on Numpy's
 *   PRNG infrastructure. When using this stand-alone PCG64DXSM, the 
 *   initialization can be done, e.g., using a sequence from SplitMix64.
 *   This means that there will be no compatibility in seed values
 *   between Numpy and the stand-alone PCG64DXSM, except if the whole
 *   state/increment is set explicitly.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For additional information about the PCG random number generation scheme,
 * including its license and other licensing options, visit
 *
 *     https://www.pcg-random.org
 *
 * Relicensed MIT in May 2019
 *
 * The MIT License
 *
 * PCG Random Number Generation for C.
 *
 * Copyright 2014 Melissa O'Neill <oneill@pcg-random.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


/*-----------------------------------------------------------
   Modified pcg64dxsm.h header
  -----------------------------------------------------------*/

#if defined(__GNUC_GNU_INLINE__) && !defined(__cplusplus)
#error Nonstandard GNU inlining semantics. Compile with -std=c99 or better.
#endif

typedef __uint128_t pcg128_t;
#define PCG_128BIT_CONSTANT(high, low) (((pcg128_t)(high) << 64) + low)

typedef struct { pcg128_t state; } pcg_state_128;

typedef struct {
  pcg128_t state;
  pcg128_t inc;
} pcg_state_setseq_128;

#define PCG_DEFAULT_MULTIPLIER_HIGH 2549297995355413924ULL
#define PCG_DEFAULT_MULTIPLIER_LOW 4865540595714422341ULL

#define PCG_DEFAULT_MULTIPLIER_128                                             \
  PCG_128BIT_CONSTANT(PCG_DEFAULT_MULTIPLIER_HIGH, PCG_DEFAULT_MULTIPLIER_LOW)
#define PCG_DEFAULT_INCREMENT_128                                              \
  PCG_128BIT_CONSTANT(6364136223846793005ULL, 1442695040888963407ULL)
#define PCG_STATE_SETSEQ_128_INITIALIZER                                       \
  {                                                                            \
    PCG_128BIT_CONSTANT(0x979c9a98d8462005ULL, 0x7d3e9cb6cfe0549bULL)          \
    , PCG_128BIT_CONSTANT(0x0000000000000001ULL, 0xda3e39cb94b95bdbULL)        \
  }

#define PCG_CHEAP_MULTIPLIER_128 (0xda942042e4dd58b5ULL)


static inline void pcg_cm_step_r(pcg_state_setseq_128 *rng) {
  rng-> state = rng->state * PCG_CHEAP_MULTIPLIER_128 + rng->inc;
}

static inline uint64_t pcg_output_cm_128_64(pcg128_t state) {
  uint64_t hi = state >> 64;
  uint64_t lo = state;

  lo |= 1;
  hi ^= hi >> 32;
  hi *= 0xda942042e4dd58b5ULL;
  hi ^= hi >> 48;
  hi *= lo;
  return hi;
}

static inline void pcg_cm_srandom_r(pcg_state_setseq_128 *rng, pcg128_t initstate, pcg128_t initseq) {
  rng->state = 0U;
  rng->inc = (initseq << 1u) | 1u;
  pcg_cm_step_r(rng);
  rng->state += initstate;
  pcg_cm_step_r(rng);
}

static inline uint64_t pcg_cm_random_r(pcg_state_setseq_128* rng)
{
    uint64_t ret = pcg_output_cm_128_64(rng->state);
    pcg_cm_step_r(rng);
    return ret;
}

/*-----------------------------------------------------------
   End of modified pcg64dxsm.h header
  -----------------------------------------------------------*/


/*-----------------------------------------------------------
   Modified pcg64dxsm.c code
  -----------------------------------------------------------*/

/* Multi-step advance functions (jump-ahead, jump-back)
 *
 * The method used here is based on Brown, "Random Number Generation
 * with Arbitrary Stride,", Transactions of the American Nuclear
 * Society (Nov. 1994).  The algorithm is very similar to fast
 * exponentiation.
 *
 * Even though delta is an unsigned integer, we can pass a
 * signed integer to go backwards, it just goes "the long way round".
 */

pcg128_t pcg_advance_lcg_128(pcg128_t state, pcg128_t delta, pcg128_t cur_mult,
                             pcg128_t cur_plus) {
  pcg128_t acc_mult = 1u;
  pcg128_t acc_plus = 0u;
  while (delta > 0) {
    if (delta & 1) {
      acc_mult *= cur_mult;
      acc_plus = acc_plus * cur_mult + cur_plus;
    }
    cur_plus = (cur_mult + 1) * cur_plus;
    cur_mult *= cur_mult;
    delta /= 2;
  }
  return acc_mult * state + acc_plus;
}


static inline void pcg_cm_advance_r(pcg_state_setseq_128 *rng, pcg128_t delta) {
    rng->state = pcg_advance_lcg_128(rng->state, delta,
                                     PCG_128BIT_CONSTANT(0, PCG_CHEAP_MULTIPLIER_128),
                                     rng->inc);
}

/*-----------------------------------------------------------
   PCG64DXSM interface routines M. Werts 2024
  -----------------------------------------------------------*/

/*
Jumping the PCG64DXSM generator for parallelization
adapted from numpy.random.PCG64DXSM.jumped()

Background:
https://numpy.org/doc/stable/reference/random/parallel.html
*/
static void _pcg64dxsm_jump(pcg_state_setseq_128 *stateptr, uint64_t Njumps)
{
	pcg128_t delta, step;

	// from numpy.random.PCG64DXSM.jump_inplace()
	step = PCG_128BIT_CONSTANT(0x9e3779b97f4a7c15, 0xf39cc0605cedc835);
	
	// calculate delta from Njumps 
	// (see  PCG64DXSM.jump_inplace()  and PCG64DXSM.advance()
	delta = step * (pcg128_t)Njumps;
	
	// execute jump-ahead
	pcg_cm_advance_r(stateptr, delta);
}


/*
Initialize the PCG64DXSM state from the uint64_t seed[2] and inc[2] arrays

*/
static void _pcg64dxsm_seed_state(pcg_state_setseq_128 *stateptr, uint64_t *seed, uint64_t *inc)
{
  pcg128_t s, i;
  s = (((pcg128_t)seed[0]) << 64) | seed[1];
  i = (((pcg128_t)inc[0]) << 64) | inc[1];

  pcg_cm_srandom_r(stateptr, s, i);
  /* Originally in Numpy code:
     pcg64_srandom_r(stateptr, s, i);
   * That is what the PCG64DXSM Numpy interface code calls.
	 However, in the original C code there is also a specific 'cm'
	 initialization function pcg_cm_srandom_r() and we use that here. 
	 The present version of the code faithfully reproduces the random
	 sequences and jump behaviour of Numpy's PCG64DXSM RNG, if
	 initialized to exactly the same state.
  */
}


/*
Simple interface, with globally stored state (only single generator possible
per process)
*/

// single, globally stored state vector for PCG64DXSM
pcg_state_setseq_128 pcg64dxsm_state;


// Get raw uint64 from PCG64DXSM
extern uint64_t pcg64dxsm_next()
{
	return pcg_cm_random_r(&pcg64dxsm_state);
}

// Get state for PCG64DXSM, for diagnostic purpose
extern void pcg64dxsm_getstateinc(uint64_t *dataptr)
{
	dataptr[0] = (uint64_t)(pcg64dxsm_state.state >> 64);
	dataptr[1] = (uint64_t)pcg64dxsm_state.state;

	dataptr[2] = (uint64_t)(pcg64dxsm_state.inc >> 64);
	dataptr[3] = (uint64_t)pcg64dxsm_state.inc;
}

/*----------------------------------------------------------------
 * Interface between PCG64DXSM and zigrandom
 *----------------------------------------------------------------*/
	
/*
	**State and Seeding**
	(adapted from Numpy documentation and source)

    The `PCG64DXSM` state vector consists of 2 unsigned 128-bit values,
    which are represented externally as Python ints. One is the state of the
    PRNG, which is advanced by a linear congruential generator (LCG). The
    second is a fixed odd increment used in the LCG.

    In the Numpy implementation, the input seed is processed by `SeedSequence`
	to generate both values. The increment is not independently settable.

	    # Seed the _bitgen
        val = self._seed_seq.generate_state(4, np.uint64)
        pcg64_set_seed(&self.rng_state,
                       <uint64_t *>np.PyArray_DATA(val),
                       (<uint64_t *>np.PyArray_DATA(val) + 2))

	The pcg64_set_seed() ensures that inc is odd and otherwise properly
	populated. It also generates a suitable value for state.

	Here, we use a similar (but numerically different) approach where the
	values are generated using Splitmix64 which is seeded with the input seed. 
	
	However, there is no correspondence in the precise PCG64DXSM state after
	intialization with a given initial seed value between Numpy and 
	randommw.c. To enable testing and comparison, we have included two cases 
	(uSeed==0, uSeed==12345) where the state of PCG64DXSM is exactly
	initialized as in numpy.random.PCG64DXSM.
*/

static void RanSetSeed_pcg64dxsm(uint64_t uSeed)
{
	uint64_t seed[2];
	uint64_t inc[2];

	if (uSeed==0)
	{
		// For the special case uSeed = 0,
		// adjust state to match numpy.randomPCG64DXSM(seed = 0)
		pcg64dxsm_state.state = 
			PCG_128BIT_CONSTANT(0x1aa1b5345996452d, 0x09585eb7a69561e3);
		pcg64dxsm_state.inc   =
			PCG_128BIT_CONSTANT(0x418ddadb3af71a82, 0x588133bc447873a9);
	}
	else if (uSeed==12345)
	{
		// For the special case uSeed = 12345,
		// adjust state to match numpy.randomPCG64DXSM(seed = 12345)
		pcg64dxsm_state.state = 
			PCG_128BIT_CONSTANT(0x1905e0335aae9634, 0x9199b0d09775add5);
		pcg64dxsm_state.inc   =
			PCG_128BIT_CONSTANT(0xc9c7353e6e2b1f28, 0x7d761f2d4027fae7);
	}
	else
	{
		RanSetSeed_splitmix64(uSeed); // seed splitmix
		
		// use splitmix to fill required seed, inc for PCG64DXSM
		// Numpy uses a similar initialization, but not using
		// SplitMix
		seed[0] = splitmix64_next();
		seed[1] = splitmix64_next();
		inc[0]  = splitmix64_next();
		inc[1]  = splitmix64_next();

		_pcg64dxsm_seed_state(&pcg64dxsm_state, seed, inc);
	}
}

static void RanJump_pcg64dxsm(uint64_t uJumps)
{
	_pcg64dxsm_jump(&pcg64dxsm_state, uJumps);
}

static void RanSeedJump_pcg64dxsm(uint64_t uSeed, uint64_t uJumpsize)
{
	RanSetSeed_pcg64dxsm(uSeed);
	if (uJumpsize > 0)
	{
		RanJump_pcg64dxsm(uJumpsize);
	}
}


/* The 32-bit unsigned integer U32Ran random routine uses only
   the upper 32 bits of PCG64DXSM,, and should pass randomness
   tests. */
static uint32_t U32Ran_pcg64dxsm(void)
{
	return (uint32_t)(pcg_cm_random_r(&pcg64dxsm_state) >> 32);
}

static double DRan_pcg64dxsm(void)
{
	uint64_t xx;
	
	/*
	xx = 0;
	while (xx == 0)
		xx = (pcg_cm_random_r(&pcg64dxsm_state) >> 11);
	*/
	
	while ((xx = (pcg_cm_random_r(&pcg64dxsm_state) >> 11)) == 0)
		;
	
	return (xx * 0x1.0p-53);
}

/*==========================================================================*/



/*==========================================================================
 *  Modified version of zigrandom.c
 *  original code by J. A. Doornik, 2005
 *
 *  modifications by M. H. V. Werts, 2024
 *  - Fixed a gcc warning
 *  - Implemented full 52-bit mantissa precision generator of random doubles
 *    in (0, 1) from pairs of 32-bit unsignedintegers coming from MWC8222
 *    (Doornik callled this generator "MWC_52")
 *  - Use stdint.h for exact integer widths
 *  - Fully initialize the generator (256 uint32s) from a single 64-bit
 *    seed, using SplitMix64
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


/* New-style RanSetSeed interface (single unsigned 64-bit integer seed)
 
   MWC8222 needs to be initialized with 256 unsigned 32-bit integers.
   These are obtained from SplitMix64.
*/
static void RanSetSeed_MWC8222(uint64_t uSeed)
{
	RanSeedJump_MWC8222(uSeed, 0);
}

/* MWC8222 does not support 'jumps'

   Here, we provide a mechanism to obtain a (radically) differently 
   initialized MWC8222 from the same seed, but using 'uJumpsize'
   to forward the initializing Splitmix generator.
   
   There is a tiny, tiny chance that the different streams are
   not completely independent. This should still be tested exhaustively,
   but at present it seems the best method to obtain reproducible
   independent streams with this generator.

*/
static void RanSeedJump_MWC8222(uint64_t uSeed, uint64_t uJumpsize)
{
	unsigned int i, j;
	
	// Do not forget to (re-)initialize all state variables.
	s_uiStateMWC = MWC_R - 1;
	s_uiCarryMWC = MWC_C;

	// Initialize Splitmix using uSeed
	RanSetSeed_splitmix64(uSeed);
	
	// Forward Splitmix64 'uJumpsize' times a full initialization,
	// i.e. MWC_R * uJumpsize
	for (j = 0; j < uJumpsize; j++)
	{
		for (i = 0; i < MWC_R; i++)
		{
			U32Ran_splitmix64(); // use this for consistency
		}
	}

	// Use the forwarded SplitMix64 to generate the initial state for MWC8222
	for (i = 0; i < MWC_R; ++i)
	{
		s_auiStateMWC[i] = U32Ran_splitmix64(); // get uint32 from splitmix64
	}
}

static uint32_t U32Ran_MWC8222(void)
{
	uint64_t t;

	s_uiStateMWC = (s_uiStateMWC + 1) & (MWC_R - 1);
	t = MWC_A * s_auiStateMWC[s_uiStateMWC] + s_uiCarryMWC;
	s_uiCarryMWC = (uint32_t)(t >> 32);
	s_auiStateMWC[s_uiStateMWC] = (uint32_t)t;
    return (uint32_t)t;
}

static double DRan_MWC8222(void)
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


/*------------------- uniform random number generators ----------------------*/

/* Set default to MWC8222 uniform generator 
   (doubles with 52 bits mantissa randomness) */
static DRANFUN s_fnDRanu = DRan_MWC8222;
static U32RANFUN s_fnU32Ranu = U32Ran_MWC8222;
static RANSETSEEDFUN s_fnRanSetSeed = RanSetSeed_MWC8222;
static RANJUMPFUN s_fnRanJump = NULL;
static RANSEEDJUMPFUN s_fnRanSeedJump = RanSeedJump_MWC8222;

double  DRanU(void)
{
    return (*s_fnDRanu)();
}

uint32_t U32RanU(void)
{
    return (*s_fnU32Ranu)();
}

void    RanSetSeed(uint64_t uSeed)
{
	(*s_fnRanSetSeed)(uSeed);
}

/* Direct jumps currently only supported by Xoshiro256+ and MELG19937.

   Calling RanJumpRan for other generators will crash your program!
*/
void    RanJumpRan(uint64_t uJumpsize)
{
	(*s_fnRanJump)(uJumpsize);
}

/* Combined setting of random seed and 'jumping'

   This provides a way to emulate jumps for PRNG that do not support
   RanJumpRan, by initializing the PRNG using numbers further on
   in the sequence of initialization random numbers from
   Splitmix64.
*/
void	RanSeedJump(uint64_t uSeed, uint64_t uJumpsize)
{
	(*s_fnRanSeedJump)(uSeed, uJumpsize);
}


void    RanSetRan(const char *sRan)
{
	/* BEGIN if ... else if ... else block */
	if (strcmp(sRan, "MWC8222") == 0)
	{
		s_fnDRanu = DRan_MWC8222;
		s_fnU32Ranu = U32Ran_MWC8222;
		s_fnRanSetSeed = RanSetSeed_MWC8222;
		s_fnRanJump = NULL;
		s_fnRanSeedJump = RanSeedJump_MWC8222;
	}
	else if (strcmp(sRan, "Lehmer64") == 0)
	{
		s_fnDRanu = DRan_lehmer64;
		s_fnU32Ranu = U32Ran_lehmer64;
		s_fnRanSetSeed = RanSetSeed_lehmer64;
		s_fnRanJump = NULL;
		s_fnRanSeedJump = RanSeedJump_lehmer64;
	}
	else if (strcmp(sRan, "PCG64DXSM") == 0)
	{
		s_fnDRanu = DRan_pcg64dxsm;
		s_fnU32Ranu = U32Ran_pcg64dxsm;
		s_fnRanSetSeed = RanSetSeed_pcg64dxsm;
		s_fnRanJump = RanJump_pcg64dxsm;
		s_fnRanSeedJump = RanSeedJump_pcg64dxsm;
	}
	else if (strcmp(sRan, "Xoshiro256+") == 0)
	{
		s_fnDRanu = DRan_xoshiro256p;
		s_fnU32Ranu = U32Ran_xoshiro256p;
		s_fnRanSetSeed = RanSetSeed_xoshiro256p;
		s_fnRanJump = RanJump_xoshiro256p;
		s_fnRanSeedJump = RanSeedJump_xoshiro256p;
	}
	else if (strcmp(sRan, "MELG19937") == 0)
	{
		s_fnDRanu = DRan_MELG19937;
		s_fnU32Ranu = U32Ran_MELG19937;
		s_fnRanSetSeed = RanSetSeed_MELG19937;
		s_fnRanJump = RanJump_MELG19937;
		s_fnRanSeedJump = RanSeedJump_MELG19937;	
	}
	else // DEFAULT = FAULT
	{
		s_fnDRanu = NULL;
		s_fnU32Ranu = NULL;
		s_fnRanSetSeed = NULL;
		s_fnRanJump = NULL;
		s_fnRanSeedJump = NULL;
	}
	/* END if ... else if ... else block */
}


void    RanSetRanExt(DRANFUN DRanFun, U32RANFUN U32RanFun, 
		             RANSETSEEDFUN RanSetSeedFun, RANJUMPFUN RanJumpFun,
					 RANSEEDJUMPFUN RanSeedJumpFun)
{
	s_fnDRanu = DRanFun;
	s_fnU32Ranu = U32RanFun;
	s_fnRanSetSeed = RanSetSeedFun;
	s_fnRanJump = RanJumpFun;
	s_fnRanSeedJump = RanSeedJumpFun;
}
/*---------------- END uniform random number generators --------------------*/

/*==========================================================================*/


/*==========================================================================
 *  Modified version of zignor.c
 *  original code by J. A. Doornik, 2005
 *  modifications by M. H. V. Werts, 2024
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
		i = U32RanU() & 0x7F;
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


/*==========================================================================*/


/*==========================================================================
 *  Random utility functions
 *  M. H. V. Werts, 2024
 *==========================================================================*/
 
/* Fully initialize PRNG and ziggurat algorithm */
void  RanInit(const char *sRan, uint64_t uSeed, uint64_t uJumpsize)
{
	zigNorInit(ZIGNOR_C, ZIGNOR_R, ZIGNOR_V);
	if (strlen(sRan) > 0)
	{
		RanSetRan(sRan);
	}
	RanSeedJump(uSeed, uJumpsize);
}

/*==========================================================================*/


/*==========================================================================
 *  Modified version of zigtimer.c
 *  original code by J. A. Doornik, 2005
 *  small modifications by M. H. V. Werts, 2024
 *
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


/*--------------------------- timer functions ------------------------------*/
#include <time.h>

#if defined(__linux__) | defined (__APPLE__)
#include <sys/times.h>
#include <time.h>

clock_t unix_clock (void)
{
    struct tms buf;
    if (times (&buf) < 0)
        return clock();

    return buf.tms_utime + buf.tms_stime;
}

#define clock unix_clock
#include <unistd.h>
#ifdef CLOCKS_PER_SEC
#undef CLOCKS_PER_SEC /* to stop warnings, always 1000000 under POSIX */
#endif
#define CLOCKS_PER_SEC (sysconf (_SC_CLK_TCK))
#endif

static char s_sTime[32];
static clock_t s_clockStart, s_clockStop;
static double s_dClockPerHundred = 0;


void    StartTimer(void)
{
	if (s_dClockPerHundred == 0)
		s_dClockPerHundred = 100.0 / CLOCKS_PER_SEC;
		
	s_clockStop = s_clockStart = clock();
}
void    StopTimer(void)
{
    s_clockStop = clock();
}

static double  ClockStartDblHsec(void)
{
    return s_clockStart * s_dClockPerHundred;
}
static double  ClockStopDblHsec(void)
{
    return s_clockStop * s_dClockPerHundred;
}

static char *  GetTimeSpan(double dTime2, double dTime1)
{
    int m, s, h, len, bzeros;  unsigned long tnew;
    double  duration;
       
    duration = fabs(dTime2 - dTime1);             /* difference in hundreds */
    tnew = (unsigned long)(duration / 100.0);      /* difference in seconds */
    
    h = (int)(duration - 100.0 * tnew);                         /* hundreds */
    s = tnew % 60;  tnew /= 60;                                  /* seconds */
    m = tnew % 60;  tnew /= 60;             /* minutes, now tnew is # hours */
    
    len = bzeros = 0;
    if (tnew)
    {   sprintf(s_sTime, "%2d:", (int)tnew);  len = (int)strlen(s_sTime);
        bzeros = 1;
    }
    if (m || tnew)
    {   sprintf(s_sTime + len, bzeros ? "%02d'" : "%2d'", m);  len += 3;
        bzeros = 1;
    }
    sprintf(s_sTime + len, bzeros ? "%02d" : "%2d", s); len += 2;
    if (h)
	{
		sprintf(s_sTime + len, ".%02d", h); len += 3;
	}
	if (!(m || tnew))
	{
		sprintf(s_sTime + len, " s"); len += 2;
	}	
	return s_sTime;
}
char * GetLapsedTime(void)
{
    return GetTimeSpan(ClockStopDblHsec(), ClockStartDblHsec());
}
/*-------------------------- END timer functions ---------------------------*/

/*==========================================================================*/




#ifdef __cplusplus
}
#endif



#endif // RANDOMMW_H