/* Ziggurat Method Generator of Zero-Mean Gaussians -- speed test
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

/* generate many samples, print out the last one, to be used with time(1) */


#include <stdio.h>
#include "zmg.h"
#define N1 50000
#define N2 100000
int main()
{
   int i,j;
   float x;
   ZMG_STATE rng;
   seedzmgf(&rng);

   for (j=0; j<N1; j++)
   {
      for (i=0; i<N2; i++)
      {
         x = zmgf(&rng);
      }
   }
   printf("After %ld trials, last number=%lf\n",(long int)(N1)*(long int)(N2),
          (double)x);
   return 0;
}
