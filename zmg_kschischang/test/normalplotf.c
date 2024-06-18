/* Ziggurat Method Generator of Zero-Mean Gaussians -- generate a normal plot
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

/* generate a normal plot */

#include <stdio.h>  /* for printf() */
#include <stdlib.h> /* for qsort() */
#include "zmg.h"

#define N 200

int fcompare(const void *a, const void *b)
{
  if (*(float *)a == *(float *)b) return 0;
  else if (*(float *)a > *(float *)b) return 1;
  else return -1;
}

int main()
{
   int i;
   float x[N];
   ZMG_STATE rng;
   float rankit[N] = { -2.74604, -2.41365, -2.23, -2.09991, -1.99782,
                    -1.91308, -1.84019, -1.77594, -1.71828, -1.66583,
                    -1.6176, -1.57287, -1.53109, -1.49182, -1.45472,
                    -1.41953, -1.38602, -1.35399, -1.3233, -1.29381,
                    -1.2654, -1.23798, -1.21146, -1.18577, -1.16084,
                    -1.13661, -1.11303, -1.09005, -1.06763, -1.04574,
                    -1.02434, -1.0034, -0.982896, -0.962793, -0.943072,
                    -0.923711, -0.904691, -0.885993, -0.8676, -0.849496,
                    -0.831666, -0.814098, -0.796777, -0.779692, -0.762832,
                    -0.746186, -0.729745, -0.713499, -0.697439, -0.681557,
                    -0.665845, -0.650297, -0.634903, -0.61966, -0.604558,
                    -0.589594, -0.57476, -0.560052, -0.545465, -0.530992,
                    -0.51663, -0.502374, -0.488219, -0.474162, -0.460197,
                    -0.446322, -0.432532, -0.418825, -0.405195, -0.39164,
                    -0.378158, -0.364743, -0.351394, -0.338107, -0.32488,
                    -0.311709, -0.298592, -0.285527, -0.27251, -0.259539,
                    -0.246611, -0.233725, -0.220877, -0.208066, -0.195289,
                    -0.182544, -0.169828, -0.15714, -0.144477, -0.131837,
                    -0.119218, -0.106619, -0.0940356, -0.0814676, -0.0689124,
                    -0.0563681, -0.0438327, -0.0313042, -0.0187805, -0.00625985,
                     0.00625985, 0.0187805, 0.0313042, 0.0438327, 0.0563681,
                     0.0689124, 0.0814676, 0.0940356, 0.106619, 0.119218,
                     0.131837, 0.144477, 0.15714, 0.169828, 0.182544,
                     0.195289, 0.208066, 0.220877, 0.233725, 0.246611,
                     0.259539, 0.27251, 0.285527, 0.298592, 0.311709,
                     0.32488, 0.338107, 0.351394, 0.364743, 0.378158,
                     0.39164, 0.405195, 0.418825, 0.432532, 0.446322,
                     0.460197, 0.474162, 0.488219, 0.502374, 0.51663,
                     0.530992, 0.545465, 0.560052, 0.57476, 0.589594,
                     0.604558, 0.61966, 0.634903, 0.650297, 0.665845,
                     0.681557, 0.697439, 0.713499, 0.729745, 0.746186,
                     0.762832, 0.779692, 0.796777, 0.814098, 0.831666,
                     0.849496, 0.8676, 0.885993, 0.904691, 0.923711,
                     0.943072, 0.962793, 0.982896, 1.0034, 1.02434,
                     1.04574, 1.06763, 1.09005, 1.11303, 1.13661,
                     1.16084, 1.18577, 1.21146, 1.23798, 1.2654,
                     1.29381, 1.3233, 1.35399, 1.38602, 1.41953,
                     1.45472, 1.49182, 1.53109, 1.57287, 1.6176,
                     1.66583, 1.71828, 1.77594, 1.84019, 1.91308,
                     1.99782, 2.09991, 2.23, 2.41365, 2.7460};

   seedzmgf(&rng);
   for (i=0; i<N; ++i)
      x[i] = zmgf(&rng);
   qsort(x,N,sizeof(float),&fcompare);
   for (i=0; i<N; ++i)
      printf("%f %f\n",(double)rankit[i],(double)x[i]);
   return 0;
}
