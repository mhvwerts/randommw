/*
First attempts towards using a 'jump-able' PRNG with zignor

xoshiro256+ which is good enough for the 32-bit unsigned integers
and 53-bit mantissa doubles

see: https://prng.di.unimi.it/

The basic generators and type conversions seem to be working.
The rest is for future work.
*/

#include <stdint.h>
#include <stdio.h>

/*----------------------------------------------------------------
 * xoshiro256plus
 *
 * This weakly scrambled 64-bit generator is good enough to generate
 * decent 32-bit uints (discard lower 32 bits)
 * and 52-bit mantissa doubles (0,1) interval
 *
 * global variable names and function names were adapted (Werts, 2024)
 *----------------------------------------------------------------*/

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
   generation, as it is slightly faster than xoshiro256++/xoshiro256**. It
   passes all tests we are aware of except for the lowest three bits,
   which might fail linearity tests (and just those), so if low linear
   complexity is not considered an issue (as it is usually the case) it
   can be used to generate 64-bit outputs, too.

   We suggest to use a sign test to extract a random Boolean value, and
   right shifts to extract subsets of bits.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */

// #include <stdint.h>

static inline uint64_t rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}


static uint64_t xoshiro256_s[4];

uint64_t xoshiro256_next(void) {
	const uint64_t result = xoshiro256_s[0] + xoshiro256_s[3];

	const uint64_t t = xoshiro256_s[1] << 17;

	xoshiro256_s[2] ^= xoshiro256_s[0];
	xoshiro256_s[3] ^= xoshiro256_s[1];
	xoshiro256_s[1] ^= xoshiro256_s[2];
	xoshiro256_s[0] ^= xoshiro256_s[3];

	xoshiro256_s[2] ^= t;

	xoshiro256_s[3] = rotl(xoshiro256_s[3], 45);

	return result;
}


/* This is the jump function for the generator. It is equivalent
   to 2^128 calls to next(); it can be used to generate 2^128
   non-overlapping subsequences for parallel computations. */

void xoshiro256_jump(void) {
	static const uint64_t JUMP[] = { 0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c };

	uint64_t s0 = 0;
	uint64_t s1 = 0;
	uint64_t s2 = 0;
	uint64_t s3 = 0;
	for(int i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
		for(int b = 0; b < 64; b++) {
			if (JUMP[i] & UINT64_C(1) << b) {
				s0 ^= xoshiro256_s[0];
				s1 ^= xoshiro256_s[1];
				s2 ^= xoshiro256_s[2];
				s3 ^= xoshiro256_s[3];
			}
			xoshiro256_next();	
		}
		
	xoshiro256_s[0] = s0;
	xoshiro256_s[1] = s1;
	xoshiro256_s[2] = s2;
	xoshiro256_s[3] = s3;
}


/* This is the long-jump function for the generator. It is equivalent to
   2^192 calls to next(); it can be used to generate 2^64 starting points,
   from each of which jump() will generate 2^64 non-overlapping
   subsequences for parallel distributed computations. */

void xoshiro256_long_jump(void) {
	static const uint64_t LONG_JUMP[] = { 0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241, 0x39109bb02acbe635 };

	uint64_t s0 = 0;
	uint64_t s1 = 0;
	uint64_t s2 = 0;
	uint64_t s3 = 0;
	for(int i = 0; i < sizeof LONG_JUMP / sizeof *LONG_JUMP; i++)
		for(int b = 0; b < 64; b++) {
			if (LONG_JUMP[i] & UINT64_C(1) << b) {
				s0 ^= xoshiro256_s[0];
				s1 ^= xoshiro256_s[1];
				s2 ^= xoshiro256_s[2];
				s3 ^= xoshiro256_s[3];
			}
			xoshiro256_next();	
		}
		
	xoshiro256_s[0] = s0;
	xoshiro256_s[1] = s1;
	xoshiro256_s[2] = s2;
	xoshiro256_s[3] = s3;
}



/*----------------------------------------------------------------
 * splitmix64
 *
 * Can be used to generate a 4 x 64-bit seed for xoshiro256+
 * from a single 64-bit seed
 * 
 * global variable  and function names were adapted (Werts, 2024)
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

// #include <stdint.h>

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
 * Future interface between xoshiro256+ and zigrandom
 *----------------------------------------------------------------*/

void RanSetSeed_xoshiro256(uint64_t uSeed)
{
	splitmix64_x = uSeed; // seed splitmix
	
	// use splitmix to fully seed xoshiro256
	xoshiro256_s[0] = splitmix64_next();
	xoshiro256_s[1] = splitmix64_next();
	xoshiro256_s[2] = splitmix64_next();
	xoshiro256_s[3] = splitmix64_next();
}

uint32_t IRan_xoshiro256(void)
{
	return (uint32_t)(xoshiro256_next() >> 32);
}

double DRan_xoshiro256(void)
{
	uint64_t xx;
	
	xx = 0;
	while (xx == 0)
		xx = (xoshiro256_next() >> 11);
	
	return (xx * 0x1.0p-53);
}



/*---------------------------------------------------------------- 
 * test & development code
 ----------------------------------------------------------------*/
int main(int argc, char **argv)
{
	uint64_t i;
	uint64_t Ni = 100000000;
	double dsum;
	
	printf("hello, world\n");

	RanSetSeed_xoshiro256(0x974ef530e0479120); 

	printf("\n");
	
	for (i = 0; i < 20; i++)
		printf("%u\n", IRan_xoshiro256());

	printf("\n");

	for (i = 0; i < 20; i++)
		printf("%f\n", DRan_xoshiro256());

	printf("\n");
	
	dsum = 0.0;
	for (i = 0; i < Ni; i++)
		dsum += (DRan_xoshiro256() - 0.5);
	
	printf("dsum = %f\n", dsum/Ni);

	return 0;
}
