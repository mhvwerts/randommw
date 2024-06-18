/* Ziggurat Method Generator of Zero-Mean Gaussians -- Kolmogorov-Smirnov test
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

/* Perform a Kolmogorov-Smirnov test */

#define N 1000000

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "zmg.h"

#define MAX(x,y) ((x)<(y)?(y):(x))

#define ROOTHALF 0.7071067811865475244

int dcompare(const void *a, const void *b)
{
  if (*(double *)a == *(double *)b) return 0;
  else if (*(double *)a > *(double *)b) return 1;
  else return -1;
}

double F(double x)  /* cdf of Gaussian */
{
   return 0.5*erfc(-x*ROOTHALF);
}

int main()
{
   ZMG_STATE rng;
   double v[N];
   double s,cdf;
   int i;

   seedzmgf(&rng);
   for (i=0; i<N; ++i)
      v[i] = (double)zmgf(&rng);
   qsort(v,N,sizeof(double),&dcompare);
   s = 0.0;
   for (i=0; i<N; ++i)
   {
      cdf = F(v[i]);
      s = MAX(s, MAX( fabs(cdf - (double)i/(double)N),
                      fabs(cdf - (double)(i+1)/(double)N)));
   }
   printf("Dn=%f for n=%d\n",s,N);
   printf("p(alpha)-values:\n");
   printf("p(0.001) = %f\n",1.94947/sqrt((double)N));
   printf("p(0.01) = %f\n",1.62762/sqrt((double)N));
   printf("p(0.02) = %f\n",1.51743/sqrt((double)N));
   printf("p(0.05) = %f\n",1.35810/sqrt((double)N));
   printf("p(0.1) = %f\n",1.22385/sqrt((double)N));
   printf("p(0.15) = %f\n",1.13795/sqrt((double)N));
   printf("p(0.2) = %f\n",1.07275/sqrt((double)N));
   return 0;
}
