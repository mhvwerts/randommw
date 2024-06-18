# ZMG: Ziggurat Method Generator of Zero-Mean Gaussians, User's Guide

[the ZMG website]: http://www.comm.utoronto.ca/frank/ZMG
[PCG-Random download site]: http://www.pcg-random.org/download.html

## Functions

Four functions are supplied, having the following prototypes:

* `float zmgf(ZMG_STATE *);`
* `double zmgd(ZMG_STATE *,ZMG_STATE *);`
* `void seedzmgf(ZMG_STATE *);`
* `void seedzmgd(ZMG_STATE *,ZMG_STATE *);`

The data type `ZMG_STATE` is defined in `zmg.h`.  The seed functions use
`getentropy()` to seed the random number generators.  The double precision
version uses two random number generators, hence pointers to two distinct
variables must be passed.

The random number generators can also be seeded directly (with a deterministic
seed value, or some other value such as the system time) by calling a function
with prototype

* `void ZMG_SEED(ZMG_STATE *, ZMG_SEED_t seed)`

This may be needed if `getentropy()` is absent.

In case it is useful, a 32-bit unsigned pseudorandom integer can be obtained,
after the random number generator has been seeded, by calling a function with
prototype

* `uint32_t ZMG_U32(ZMG_STATE *)`

## Separate Compilation and Linking

Ordinarily the functions can be compiled to an object file against which a
calling program can be linked.  The following `gcc` invocation shows the
recommended compiler flags:

* `gcc -Wall -O3 -flto -c zmgf.c`
* `gcc -Wall -O3 -flto -c zmgd.c`

The `-flto` flag enables link-time optimization, which can result in function
inlining, leading to a faster-running program.  In a user's program
(`myprogram.c`, for example) the user should simply `#include "zmg.h"` (both
`zmg.h` and `pcg_variants.h` are assumed to reside in the working directory) to
inform the compiler of the function prototypes.  The `-flto` flag is
recommended when compiling user programs and also when linking, for example:

* `gcc -Wall -O3 -flto -c myprogram.c`
* `gcc -O3 -flto myprogram.o zmgf.o -lm`

(Note that the standard math library is required.)

## Single Compilation and Linking

It is also possible to `#include "zmgf.c"` (or `zmgd.c`) directly into a user
program (without, then, including `zmg.h`.)  Users should then avoid using
identifiers starting with letters ZMG.  While this method will slow compilation
(since `zmgf.c` will be compiled whenever a user-program is compiled), it has
the advantage that the compiler may optimize `zmgf.c` jointly with the user
program.  With this method,

* `gcc -Wall -O3 -o myprogram myprogram.c -lm`

will combine compilation and linking into one command.

## Obtaining PCG

Navigate to [PCG-Random download site] and download the (full, not minimal) C
Implementation.  After unzipping, search for the file `include/pcg_variants.h`
and copy it to your ZMG working directory (which should already contain
`zmg.h`).  Thus, for example,

* `cp pcg-c-0.94/include/pcg_variants.h .`

## Substituting for PCG

It is possible to substitute PCG with another source of pseudorandom,
independent, uniformly-distributed, 32-bit unsigned integers.  The interface to
the RNG assumed by ZMG is as follows.

* Seeding the RNG is accomplished by calling a function with prototype
 `(void)ZMG_SEED(ZMG_STATE *rng_p, ZMG_SEED_t seed);`
* Generating a pseudorandom number is accomplished by calling a function
with prototype `uint32_t ZMG_U32(ZMG_STATE *rng_p);` where `ZMG_U32` is the
name of the RNG function, and `ZMG_STATE` is an RNG-implementation-specific
data type.

These names are set up for PCG in `zmg.h`  --- replacing the PCG definitions
can allow for a different RNG to be used.
