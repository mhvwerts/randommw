# ZMG: Ziggurat Method Generator of Zero-Mean Gaussians

[ZMG website]: http://www.comm.utoronto.ca/frank/ZMG

## Extras

This directory contains a wolframscript for generating the parameters
of the ziggurat rectangles, trapezoids, and tail, in a form suitable
for including directly in zmgf.c and zmgd.c.

## Running

Assuming Mathematica is installed, simply type

    ./table.math n

where n is the number of bits to use in rectangle selection.  For example,
n=8 corresponds to 253 rectangles (nearly 2^8).  The output is written to
two files with names such as table08f.h and table08d.h (float and double
versions, respectively). 
