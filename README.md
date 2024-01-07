
# zignormw: Generator of normally distributed pseudo-random numbers


## Introduction

The numerical simulation of the Brownian motion of colloidal nanoparticles needs a source of random numbers. Moreover, these random numbers should have a Gaussian distribution. Computers generate random numbers by specific algorithms. As a result, the numbers generated are not *really* random (they are generated by a deterministic algorithm), but the sequences are (practically) indistinguishable from really random numbers, in terms of statistical properties, and can be used in many numerical applications requiring random numbers. Computer-generated random numbers are *pseudo-random numbers*. 

The choice of the algorithm for the generation of pseudo-random numbers is essential to the success of the numerical simulation.[1] There are many pseudo-random number generators (PRNGs) to choose from and new ones are still appearing.[2] Luckily, there are scientists specialized in testing these PRNGs, and specific test programs for PRNGs have been developed.[3][4][5] However, it is not always very clear to the unsuspecting consumer of PRNGs how to choose. Some developers are going so far as to give the impression that their PRNG algorithm is "the best" and all other algorithms are "not good". There is probably some sociology involved in as to how certain algorithms become the standard PRNG in scientific environments such as Python/numpy, Fortran and Julia. The details about PRNGs are somewhat hidden to the casual user of Python and other high-level languages, yet it is important to be aware and explicit about choosing PRNGs.

The diversity of PRNGs in modern scientific computing is a strength, especially with open source code, and any serious flaw in PRNGs for scientific computing will very likely be detected.[6] Also, it is elusive to look for "the best" PRNG, but one should at least find one that is "good enough" for the particular purpose. A PRNG for Brownian simulation can be good enough, while still failing certain very stringent statistical tests. A look in the literature and discussions with colleagues doing molecular dynamic simulations shows that currently the Mersenne Twister (MT19337) is often used. This does not mean that other generators such as PCG64 and Xoshiro256++ will not work. For instance, we have interchangeably used MT19937 and PCG64 in basic Brownian simulations,[7] with equal success. On the other hand, some older PRNGs have been shown to be of insufficient quality.[1][6]

Standard PRNGs generally generate uniformly distributed random numbers. Since we need a normal distribution for Brownian simulation, the initial uniform should be converted into a random with a Gaussian distribution. There are several ways to do this,[8] with the [Box-Muller transform](https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform) and the [ziggurat algorithm](https://en.wikipedia.org/wiki/Ziggurat_algorithm) being amongst the most well-known methods. Several modifications of the ziggurat algorithm now exist, improving statistical properties and/or numerical performance.[9][10] 

In conclusion, I discovered computer generation of random numbers to be a fascinating subject, and an important part of the history and future of scientific computing, with Monte Carlo methods coming to mind as an example. To gain a clearer view of present-day PRNGs, I decided to find structured and documented source code of PRNGs for normally distributed variables, compile them and use them for Brownian simulations, in parallel to our continued use of the routines provided by Python/numpy (`random.Generator.normal` and the MT19937 and PCG64 random number generators). The vehicle chosen for this touristic holiday excursion is the C programming language.


[1] D. Jones, "Good Practice in (Pseudo) Random Number Generation for
Bioinformatics Applications", http://www0.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf

[2] F. Panneton, P. L'Ecuyer, M. Matsumoto, "Improved long-period generators based on linear recurrences modulo 2", ACM Transactions on Mathematical Software 2006, 32, 1. https://doi.org/10.1145/1132973.1132974

[3] https://www.phy.duke.edu/~rgb/General/dieharder.php

[4] P. L'Ecuyer and R. Simard, "TestU01: A C Library for Empirical Testing of Random Number Generators", ACM Transactions on Mathematical Software 2007, 33, 22.

[5] http://simul.iro.umontreal.ca/testu01/tu01.html

[6] Click, T. H., Liu, A., & Kaminski, G. A., "Quality of random number generators significantly affects results of Monte Carlo simulations for organic and biological systems", Journal of computational chemistry 2011, 32, 513. https://doi.org/10.1002/jcc.21638

[7] [ddm-toolkit @ github]( https://github.com/mhvwerts/ddm-toolkit/blob/045fa3e8819490595d2bd37ccb79bda7330ddbe1/ddm_toolkit/simulation.py#L16C1-L16C1)

[8] D. B. Thomas, P. H. W. Leong, W. Luk, J. D. Villasenor, "Gaussian Random Number Generators", ACM Computing Surveys 2007, 39, 11. doi:10.1145/1287620.1287622. https://www.doc.ic.ac.uk/~wl/papers/07/csur07dt.pdf

[9] J. A. Doornik, "An Improved Ziggurat Method to Generate Normal Random Samples" (2005), https://www.doornik.com/research/ziggurat.pdf

[10] C. D. McFarland, "A modified ziggurat algorithm for generating exponentially and normally distributed pseudorandom numbers", Journal of Statistical Computation and Simulation 2016, 7, 1281. https://dx.doi.org/10.1080/00949655.2015.1060234


## Doornik's ziggurat code

A particularly portable, structured and relatively well-documented C code for the generation of normally distributed random numbers is provided by J. A. Doornik.[9] This code was found to compile and run correctly on both Linux and Windows gcc implementation, and will very likely work with other systems and C compilers. By default, it uses the MWC8222 (also called MWC256) PRNG as the source of a uniform random variable, which is then converted to a random variable with a Gaussian distribution using a ziggurat algorithm. We will use the Doornik ziggurat code as the starting point for our exploration and development.

In the folder `original_ziggurat_code_doornik` the source code of the original [ZIP archive](https://www.doornik.com/research/ziggurat_code.zip) by Doornik is conserved. The compiled executables from the ZIP file have been removed for security reasons, and the "makefile" folders for gcc have been renamed to emphasize the 32-bit *vs* 64-bit nature of the targeted executables. The file contents have been left intact.

The necessary files have been copied from `original_ziggurat_code_doornik` to the root folder. It is the objective to use them 'as received' without modification, although minor changes might be applied if really necessary.


## Usage

For simple generation of normally distributed random numbers (double precision), only `zignor.c`, `zignor.h`, `zigrandom.c` and `zigrandom.h` are needed. This includes an underlying MWC2588 uniform PRNG, which should be suitable for many applications. Only two functions are of relevance in this case: `RanNormalSetSeedZig()` for initialization and `DRanNormalZig()` for random numbers.

```c
#include <stdio.h>
#include "zignor.h"

int main(void) {
	unsigned int i;
	int zigseed = 10;
	double rval;
		
	RanNormalSetSeedZig(&zigseed, 1);
	
	for(i = 0; i < 20; i++)	{
		rval = DRanNormalZig();
		printf("%10.6f\n", rval);
	}
	
	return 0;
}
```

### `void RanNormalSetSeedZig(int *piSeed, int cSeed)`

Initialize the ziggurat algorithm and set the random seed for the underlying random number generator. At present, only the default MWC2588 PRNG is supported. The random seed is set via `piSeed` and `cSeed`, where `piSeed` points to an array of integers whose length is given by `cSeed`.

The default MWC2588 PRNG is initialized based on a single 32-bit unsigned integer seed. There are three cases:
- `cSeed == 0` and/or `piSeed == NULL` (pointer undefined). The value of the random seed is set to `0`.
- `cSeed == 1`. Then, `piSeed` should point to a (signed) integer containing the seed value, which can be negative. (Internally, the 32-bit signed integer is used as a 32-bit unsigned integer, but no bit is lost.)
- `cSeed == 256` (the value of `MWC_R`). *Untested*. `piSeed` is a 256-element integer array that completely defines the state of the random number generator. This may be used to restart the PRNG at a precise point, after a long run.

For randomly seeding the MWC2588 PRNG, we can use the conventional method using the system time (not entirely recommended in a multiprocessing environment, but good enough for now). This may be done as follows.

```c
#include <time.h>
#include "zignor.h"

(...)

    int zigseed; // signed seed as required by RanNormalSetSeedZig()
    unsigned int uzigseed; // store unsigned seed from time()
    
	// set seed based on time only (good enough for now)
	uzigseed = (unsigned int) time(NULL); 
    // convert unsigned to signed without losing one bit
    // (isentropic conversion)
    zigseed = (&uzigseed)[0];
    RanNormalSetSeedZig(&zigseed, 1);

```


### `double DRanNormalZig(void)`

Calculate and return the next random number in the normally distributed sequence.


## Development environment and compilation 

At present, the development will use `gcc` exclusively, both on Windows via [mingw-w64](https://www.mingw-w64.org/)/[w64devkit](https://github.com/skeeto/w64devkit) and on standard Linux (64 bit). The code relies on standard C (let's say C99, for the part that is supported by gcc). A single makefile takes care of everything for now.


## Status 

At present, we are working towards basic usage of the PRNG for normally distributed numbers. The organization of the code repository is in a preliminary state. Several essential steps need still to be made. Documentation of the test procedures is for now done inside the code.


## To do

- Hook up to DDM Toolkit for use in actual Brownian simulation (e.g. program taking parameters: seed, number of Gaussian PRNs, filename for binary output; adapt DDM toolkit to load random numbers sequence from file or call the program directly then load the file)


## Suggestions for future work

- Focus on the 'pure' ZIGNOR algorithm without the [V][I]ZIGNOR optimizations, which do not seem to bring much acceleration on modern 64-bit systems. The original code could then be stripped down.
- Clean up to better specify integer types (`int64_t` instead of `long long int` etc.), if and where necessary.
- Specify `uint32_t` instead of `unsigned int` in `zigrandom.c/GetInitialSeeds()`, since the arithmetic here relies specifically on the 32-bitness of the variable.
- Plug in other uniform PRNGs as the random source (for example, WELL512, WELL1024a and the like?).
- Include programs that explicitly test quality of randomness (e.g., see [8] for feeding output to standard random test suites) and normal-ness of generated normally distributed random numbers, instead of relying of reported tests by Doornik.


