
# randommw: Generator of pseudo-random numbers with uniform or Gaussian distribution (in C)


## Description

Numerical simulations for scientific and technological applications regularly require the generation of sequences of (pseudo-)random numbers.[1] This is the case, for instance, for [Monte Carlo methods](https://en.wikipedia.org/wiki/Monte_Carlo_method).[2] Another example is the simulation of the motion of Brownian particles in fluids, where the sequence of numbers, in addition to being random, should follow a Gaussian distribution.[3]

This small C library provides all the basic functionality for such scientific random number generation. It is monolithic: only `randommw.c` and `randommw.h` need to be included in the project, and it does not need any other non-standard library. It is an integrated and curated collection of tried & tested code described in the literature. More background is provided at the end of this README document.

The library includes four different generators of uniformly distributed pseudo-random numbers: MWC256,[9][19][20] Lehmer64,[14] Xoshiro256+,[12][21] and MELG19937-64.[17][22] These have been reported to pass the relevant statistical tests (see the cited references). There is a ziggurat algorithm, ZIGNOR, coded by J. A. Doornik,[9] for obtaining random numbers with a Gaussian distribution. The quality of the generated Gaussian distributions has been checked via their raw moments, following McFarland.[10]

<p align="center">
  <img src="./tests/histogram.png" width="450">
</p>
<p align="center">
  <b>Figure.</b> Histogram (log scale) of 10 billion samples generated by randommw (ZIGNOR with MELG19937-64).
</p>


## Usage

### Minimal example

Here is a minimal example, which only uses two functions: `RanInit()` for initialization and `DRanNormalZig()` for normally-distributed random numbers ('full double precision' floating point values). By default, this uses the MWC256 uniform PRNG.

```c
#include <stdio.h>
#include "randommw.h"

int main(void) {
	unsigned int i;
	uint64_t zigseed = 10;
	double rval;
		
	RanInit("", zigseed, 0);
	
	for(i = 0; i < 20; i++)	{
		rval = DRanNormalZig();
		printf("%10.6f\n", rval);
	}
	
	return 0;
}

```

### Important warning

Choose, initialize and use only a single PRNG from `randommw.c` in each C program. The library was designed to provide a single pseudo-random number stream from a single PRNG in a single process. Simple (but quite effective) parallelization of simulations is possible by running several instances of the same program in parallel, using the same PRNG with the same seed `uSeed`, but a different `uJumpsize` (see `RanInit()`) for each processes. For comparison purposes, it is possible to switch to a different PRNG in the same program, but each switch completely re-initializes and re-starts the PRNG.


### `void RanInit(const char *sRan, uint64_t uSeed, uint64_t uJumpsize)`

Initialize the ziggurat algorithm, set the PRNG and its random seed, and optionally "fast-forward" the generator. The random seed should always be supplied by the user, in order to have reproducible random number streams. If a different stream is needed, provide a different seed.

If `sRan` is an empty string, the default generator will be used: MWC256. At present, the possible choices for `sRan` are `"MWC256"`, `"Lehmer64"`, `"Xoshiro256+"` and `"MELG19937"`. The string is case-sensitive, and should correspond exactly to one of these options; else, your program will crash. 

The random seed `uSeed` is always an unsigned 64-bit integer, independently of the specific random number generator. A PRNG-specific routine uses this seed to fully initialize the PRNG. 

For `uJumpsize > 0`, the initialization routine will "fast-forward" the generator, starting from the initially seeded state.  This mechanism, often called "(block) splitting", is of importance for reliable parallelization of computer simulations.[2] In the case of `"Xoshiro256+"` and`"MELG19937"`, long "jumps" of the generator are performed algorithmically. Each of the `uJumpsize` jumps fast-forwards the PRNG by 2^192 (Xoshiro256+) or 2^256 (MELG19937) steps, giving access to a stream of random numbers that is guaranteed to be independent of the other streams from the same seed. `"MWC256"` and `"Lehmer64"` do not provide such algorithmic jumps to independent sequences from the same seed. For these PRNGs, we have resorted to using `uJumpsize` to initialize these generators differently from the same `uSeed` by forwarding the internal Splitmix64 generator used for initialization of these PRNGs. This very probably leads to independent sequences, although there is no formal guarantee in this case. It is still better than simply using different seeds.


### `double DRanNormalZig(void)`

Calculate and return the next random number in the normally distributed sequence using the ziggurat algorithm.


### `double DRanU(void)`

Obtain a double-precision floating point random number from a uniform distribution (0, 1) using the active PRNG. Full 52-bit mantissa randomness.


### `uint32_t IranU(void)`

Obtain an unsigned 32-bit integer random number from the active PRNG. Only 32-bit unsigned values are supplied, for historic reasons: the ZIGNOR algorithm relies on 32-bit unsigned integers. Interfaces supplying other types of random numbers may be developed.


## Compilation, development and testing

At present, the development uses `gcc` exclusively, both on Windows via [mingw-w64](https://www.mingw-w64.org/)/[w64devkit](https://github.com/skeeto/w64devkit) and on standard Linux (64 bit). The code relies on standard C (C99, for the part that is supported by `gcc`). There is a Makefile in the root folder, and a separate Makefile for the test programs in `../tests`.


## Status 

We have validated the PRNGs and are using them for normally distributed random numbers in numerical simulations of colloidal systems, The code is functional and is now contained in a monolithic module (`randommw.c`) that can be easily included in a scientific computing project in C. The random numbers have a good Gaussian distribution (tested up to 8 raw moments, see `tests/test_moments.c`). They are generated with high throughput, using MWC256, Lehmer64, Xoshiro256+ or MELG19937-64 as underlying uniform PRNG.

Generated normally distributed pseudo-random numbers can be written to a binary file using `genzignor.c`. These numbers have been used successfully for Brownian simulations in [DDM Toolkit](https://github.com/mhvwerts/ddm-toolkit), giving consistent results between the simulation and subsequent DDM analysis of the simulated image stack.


### Suggestions for future work

- The presently used raw moments test (`tests/test_moments.c`) by McFarland [10] should already be quite robust for testing the quality of the Gaussian distribution, but inspiration for additional tests may be found [here](https://cran.r-project.org/web/packages/RcppZiggurat/vignettes/RcppZiggurat.pdf) and [here](https://www.seehuhn.de/pages/ziggurat.html).
- Include programs that explicitly test final quality of randomness of generated normally distributed random numbers. See, e.g., [8] for feeding output to standard random test suites. Also simply test the underlying uniform PRNG, using, *e.g.*, [Lemire's testingRNG](https://github.com/lemire/testingRNG).



## Background

The numerical simulation of the Brownian motion of colloidal nanoparticles needs a source of random numbers. Moreover, these random numbers should have a Gaussian distribution. Computers generate random numbers by specific algorithms. As a result, the numbers generated are not *really* random (they are generated by a deterministic algorithm), but the sequences are (practically) indistinguishable from really random numbers, in terms of statistical properties, and can be used in many numerical applications requiring random numbers. Computer-generated random numbers are *pseudo-random numbers*. 

The choice of the algorithm for the generation of pseudo-random numbers is essential to the success of the numerical simulation.[1][2][4] There are many pseudo-random number generators (PRNGs) to choose from and new ones are still appearing.[2][12][16][17][23] However, it is not always very clear to the unsuspecting consumer of PRNGs how to choose. Some developers are going so far as to give the impression that their PRNG algorithm is "the best" and all other algorithms are "not good". There is probably some sociology involved in as to how certain algorithms become the standard PRNG in scientific environments such as Python/numpy, Fortran and Julia. The details about PRNGs are somewhat hidden to the casual user of Python and other high-level languages, yet it is important to be aware and explicit about choosing PRNGs, in particular when performing scientific simulations. Luckily, there are scientists specialized in testing PRNGs, and specific test programs for PRNGs have been developed.[5][6][18] 

The diversity of PRNGs in modern scientific computing is a strength, especially with open source code, and any serious flaw in PRNGs for scientific computing will very likely be detected.[4][15] Also, it is elusive to look for "the best" PRNG, but one should at least find one that is "good enough" for the particular purpose. A PRNG can be good enough for Brownian simulation, while still failing certain very stringent statistical tests. A look in the literature and discussions with colleagues doing molecular dynamic simulations shows that currently the Mersenne Twister (MT19337) is often used. This does not mean that other generators such as PCG64 and Xoshiro256++ will not work. For instance, we have interchangeably used MT19937 and PCG64 in basic Brownian simulations,[7] with equal success. On the other hand, some older PRNGs have been shown to be of insufficient quality in certain applications such as molecular dynamics or Monte Carlo simulations.[1][4][15]

Standard PRNGs generally generate uniformly distributed random numbers. Since we need a normal distribution for Brownian simulation, the initial uniform should be converted into a random with a Gaussian distribution. There are several ways to do this,[8] with the [Box-Muller transform](https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform) and the [ziggurat algorithm](https://en.wikipedia.org/wiki/Ziggurat_algorithm) being amongst the most well-known methods. Several modifications of the ziggurat algorithm now exist, improving statistical properties and/or numerical performance.[9][10] 


### Doornik's ziggurat code

A particularly portable, structured and relatively well-documented C code for the generation of normally distributed random numbers is provided by J. A. Doornik.[9] It has been tried and tested independently in the literature.[10] The Doornik code was found to compile and run correctly on both Linux and Windows gcc implementation, and will very likely work with other systems and C compilers. By default, it uses Marsaglias's MWC256 (sometimes called MWC8222) PRNG as the source of a uniform random variable, which is then converted to a random variable with a Gaussian distribution using a ziggurat algorithm. We used the Doornik ziggurat code as the starting point for our development.

In the folder `original_ziggurat_code_doornik` the source code of the original [ZIP archive](https://www.doornik.com/research/ziggurat_code.zip) by Doornik is conserved. The compiled executables from the ZIP file have been removed for security reasons, and the "makefile" folders for gcc have been renamed to emphasize the 32-bit *vs* 64-bit nature of the targeted executables. The file contents have been left intact.

The files necessary for development (`zignor.c`, `zignor.h`, `zigrandom.c` and `zigrandom.h`) have been copied from `original_ziggurat_code_doornik` to the root folder. `zignor` and `zigrandom` were merged into `randommw` and have undergone some changes. The modifications only concern the structure of the code, not the fundamental algorithms. The MWC8222 routines have been renamed to MWC256. The function`DRan_MWC256()` generates random doubles with full 52-bit mantissa resolution (instead of 32-bit in Doornik's original) via two iterations of the MWC256 generator (see [11]).


### Other ziggurat codes

Many implementations of ziggurat algorithms for generation of normally distributed numbers from a uniform PRNG can be found on the Internets. Here are two well-documented and reliable examples.

- [Kschischang](https://www.comm.utoronto.ca/~frank/ZMG/) has made a very nicely documented and well-structured (yet platform-dependent) C implementation of McFarland's 2016 [10] ziggurat algorithm.

- [Voss](https://www.seehuhn.de/pages/ziggurat.html) provides a concise and well-structured ziggurat code that is part of the GNU Scientific Library ([function `gsl_ran_gaussian_ziggurat()`](https://www.gnu.org/software/gsl/doc/html/randist.html#c.gsl_ran_gaussian_ziggurat) ). It is based on the original algorithm by Marsaglia & Tsang,[13] with some simplifications, and might suffer from the same minor randomness problems.[9]



## References

[1] D. Jones, "Good Practice in (Pseudo) Random Number Generation for
Bioinformatics Applications", http://www0.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf

[2] H. Bauke, S. Mertens, "Random Numbers for Large-Scale Distributed Monte Carlo Simulations", Phys. Rev. E 2007, 75, 066701. doi:10.1103/PhysRevE.75.066701.

[3] G. Volpe, G. Volpe, "Simulation of a Brownian Particle in an Optical Trap", American Journal of Physics 2013, 81, 224–230. doi:10.1119/1.4772632.

[4] Click, T. H., Liu, A., & Kaminski, G. A., "Quality of random number generators significantly affects results of Monte Carlo simulations for organic and biological systems", Journal of computational chemistry 2011, 32, 513. https://doi.org/10.1002/jcc.21638

[5] P. L'Ecuyer and R. Simard, "TestU01: A C Library for Empirical Testing of Random Number Generators", ACM Transactions on Mathematical Software 2007, 33, 22.

[6] http://simul.iro.umontreal.ca/testu01/tu01.html

[7] [ddm-toolkit @ github]( https://github.com/mhvwerts/ddm-toolkit/blob/045fa3e8819490595d2bd37ccb79bda7330ddbe1/ddm_toolkit/simulation.py#L16C1-L16C1)

[8] D. B. Thomas, P. H. W. Leong, W. Luk, J. D. Villasenor, "Gaussian Random Number Generators", ACM Computing Surveys 2007, 39, 11. doi:10.1145/1287620.1287622. https://www.doc.ic.ac.uk/~wl/papers/07/csur07dt.pdf

[9] J. A. Doornik, "An Improved Ziggurat Method to Generate Normal Random Samples" (2005), https://www.doornik.com/research/ziggurat.pdf

[10] C. D. McFarland, "A modified ziggurat algorithm for generating exponentially and normally distributed pseudorandom numbers", Journal of Statistical Computation and Simulation 2016, 7, 1281. https://dx.doi.org/10.1080/00949655.2015.1060234

[11] J.A. Doornik, "Conversion of High-Period Random Numbers to Floating Point", ACM Trans. Model. Comput. Simul. 2007, 17, 3. https://dx.doi.org/10.1145/1189756.1189759

[12] https://prng.di.unimi.it/

[13] G. Marsaglia and W. W. Tsang, "The Ziggurat Method for Generating Random Variables", Journal of Statistical Software 2000, 5, 1-7. https://doi.org/10.18637/jss.v005.i08

[14] https://github.com/lemire/testingRNG

[15] A.M. Ferrenberg, D. P. Landau, D. P., Y. J. Wong, "Monte Carlo Simulations: Hidden Errors from ‘“Good”’ Random Number Generators", Phys. Rev. Lett. 1992, 69, 3382–3384. doi:10.1103/PhysRevLett.69.3382.

[16] F. Panneton, P. L'Ecuyer, M. Matsumoto, "Improved long-period generators based on linear recurrences modulo 2", ACM Transactions on Mathematical Software 2006, 32, 1. https://doi.org/10.1145/1132973.1132974

[17] S. Harase, T. Kimoto, "Implementing 64-Bit Maximally Equidistributed F 2 -Linear Generators with Mersenne Prime Period", ACM Trans. Math. Softw. 2018, 44, 1–11. doi:10.1145/3159444.

[18] https://www.phy.duke.edu/~rgb/General/dieharder.php

[19]  Collins, J. C. "Testing, Selection, and Implementation of Random Number Generators", Defense Technical Information Center: Fort Belvoir, VA, 2008. doi:[10.21236/ADA486379](https://doi.org/10.21236/ADA486379).

[20] G. Marsaglia, [post to sci.math Usenet group, 25 Feb 2003](https://groups.google.com/g/sci.math/c/k3kVM8KwR-s/m/jxPdZl8XWZkJ )

[21] H. Bauke, "Tina’s Random Number Generator Library Version 4.24". Documentation. https://www.numbercrunch.de/trng/trng.pdf

[22] F. Le Floc’h, "Entropy of Mersenne-Twisters", arXiv [doi:10.48550/arXiv.2101.11350](https://doi.org/10.48550/arXiv.2101.11350)

[23] https://www.pcg-random.org/
