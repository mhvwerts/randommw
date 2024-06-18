/* Ziggurat Method Generator of Zero-Mean Gaussians -- generate histogram
 *
 * Copyright 2019 Frank R. Kschischang <frank@ece.utoronto.ca>
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
 * For additional information about ZMG, visit
 *
 *     http://www.comm.utoronto.ca/frank/ZMG
 */

/* generate a histogram from ZMG */
/* compile with -DTHEORY to compute the corresponding theoretical histogram */

#define BINS 900
#define RANGE 5.5

#include <stdio.h>
#include <math.h>
#define ROOTHALF 0.7071067811865475244

#ifdef THEORY
int main()
{
   double a,b,z;
   int i;

   z = (2.0*RANGE)/(double)BINS;
   a = -(RANGE);
   for (i=0; i<BINS; ++i)
   {
      b = (i+1)*z - RANGE;
      printf("%g %g\n",0.5*(a+b),0.5*(erfc(ROOTHALF*a)-erfc(ROOTHALF*b)));
      a = b;
   }
   return 0;
}
#else
#include "zmg.h"
#define N1 50000
#define N2 100000
int main()
{
   long int count[BINS];
   long int discount;
   int i,j,k;
   double z;
   ZMG_STATE rng1, rng2;
   seedzmgd(&rng1,&rng2);

   z = (double)BINS/(2.0*RANGE);
   discount = 0L;
   for (i=0; i<BINS; ++i)
      count[i] = 0L;
   for (j=0; j<N1; j++)
   {
      for (i=0; i<N2; i++)
      {
         k = (int)floor((zmgd(&rng1,&rng2) + RANGE) * z);
         if (k >= 0 && k < BINS)
             ++count[k];
         else
            ++discount;
      }
   }
   for (i=0; i<BINS; ++i)
      printf("%g %g\n",((double)i + 0.5)/z - RANGE,
             (double)count[i]/((double)N1*(double)N2-(double)discount));
   printf("# after %ld trials, discounted %ld (expected %lf)\n",
          (long int)N1*(long int)N2, discount,
          (double)N1*(double)N2*erfc(ROOTHALF*RANGE));
   return 0;
}
#endif
