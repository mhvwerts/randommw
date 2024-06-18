/* Ziggurat Method Generator of Zero-Mean Gaussians --- test moments
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

/* generate some sample moments, up to the 8th */

#include <stdio.h> 
#include "zmg.h"

#define N1 50000
#define N2 100000
int main()
{
   int i,j;
   double x,y;
   double m1, m2, m3, m4, m5, m6, m7, m8;

   ZMG_STATE rng;
   seedzmgf(&rng);

   m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = 0.0;
   for (j=0; j<N1; j++)
   {
      for (i=0; i<N2; i++)
      {
         x = (double)zmgf(&rng);
         m1 += x;
         m2 += (y=x*x);
         m3 += (y=y*x);
         m4 += (y=y*x);
         m5 += (y=y*x);
         m6 += (y=y*x);
         m7 += (y=y*x);
         m8 += (y=y*x);
      }
   }
   m1 /= N1; m1 /= N2;
   m2 /= N1; m2 /= N2;
   m3 /= N1; m3 /= N2;
   m4 /= N1; m4 /= N2;
   m5 /= N1; m5 /= N2;
   m6 /= N1; m6 /= N2;
   m7 /= N1; m7 /= N2;
   m8 /= N1; m8 /= N2;
   printf("After %ld trials:\n",(long int)(N1)*(long int)(N2));
   printf(" m1 = %lf, expect 0\n",m1);
   printf(" m2 = %lf, expect 1\n",m2);
   printf(" m3 = %lf, expect 0\n",m3);
   printf(" m4 = %lf, expect 3\n",m4);
   printf(" m5 = %lf, expect 0\n",m5);
   printf(" m6 = %lf, expect 15\n",m6);
   printf(" m7 = %lf, expect 0\n",m7);
   printf(" m8 = %lf, expect 105\n",m8);
   return 0;
}
