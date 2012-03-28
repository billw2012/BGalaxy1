/*
   Copyright (C)  2000    Daniel A. Atkinson  <DanAtk@aol.com>
   Copyright (C)  2004    Ivano Primi  <ivprimi@libero.it>    

   This file is part of the HPA Library.

   The HPA Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The HPA Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the HPA Library; if not, write to the Free
   Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
   02110-1301 USA.
*/

/*
    Test:  xadd  xmul  xdiv

    Uses:  atox  xprxpr  xtodbl

    Input file:  data/aop1.dat
*/

#include <stdio.h>
#include <stdlib.h>
#include "xpre.h"

int decd = 30;

int
main (int na, char **av)
{
  struct xpr s, t, f;
  char nbx[64], nby[64];
  FILE *fp;

  if (na != 2)
    {
      printf ("para: input_file\n");
      exit (-1);
    }
  fp = fopen (*++av, "r");
  printf ("     Test of Elementary Operations\n");
  while (fscanf (fp, "%s %s", nbx, nby) != EOF)
    {
      s = atox (nbx);
      t = atox (nby);
      printf ("\n x= ");
      xprxpr (s, decd);
      printf ("\n y= ");
      xprxpr (t, decd);

      /* extended precision addition */
      printf ("\nadd\n");
      f = xadd (s, t, 0);
      printf (" %16.10f\n", xtodbl (f));
      xprxpr (f, decd);

      /* extended precision subtraction */
      printf ("\nsubtract\n");
      f = xadd (s, t, 1);
      printf (" %16.10f\n", xtodbl (f));
      xprxpr (f, decd);

      /* extended precision multiplication */
      printf ("\nmultiply\n");
      f = xmul (s, t);
      printf (" %16.10f\n", xtodbl (f));
      xprxpr (f, decd);

      /* extended precision division */
      printf ("\ndivide\n");
      f = xdiv (s, t);
      printf (" %16.10f\n", xtodbl (f));
      xprxpr (f, decd);
    }
  putchar ('\n');
  return 0;
}

/*  Test output

     Test of Elementary Operations

 x= 1.000000000000000000000000000000e+0
 y= 4.000000000000000000000000000000e+0
add
     5.0000000000
5.000000000000000000000000000000e+0
subtract
    -3.0000000000
-3.000000000000000000000000000000e+0
multiply
     4.0000000000
4.000000000000000000000000000000e+0
divide
     0.2500000000
2.500000000000000000000000000000e-1
 x= -2.300000000000000000000000000000e+0
 y= 3.147159000000000000000000000000e+0
add
     0.8471590000
8.471590000000000000000000000000e-1
subtract
    -5.4471590000
-5.447159000000000000000000000000e+0
multiply
    -7.2384657000
-7.238465700000000000000000000000e+0
divide
    -0.7308178583
-7.308178582651845680501048723627e-1
 x= 2.750000000000000000000000000000e+1
 y= -3.360000000000000000000000000000e+1
add
    -6.1000000000
-6.100000000000000000000000000000e+0
subtract
    61.1000000000
6.110000000000000000000000000000e+1
multiply
  -924.0000000000
-9.240000000000000000000000000000e+2
divide
    -0.8184523810
-8.184523809523809523809523809524e-1
 x= -1.110000000000000000000000000000e+1
 y= -4.000000000000000000000000000000e-1
add
   -11.5000000000
-1.150000000000000000000000000000e+1
subtract
   -10.7000000000
-1.070000000000000000000000000000e+1
multiply
     4.4400000000
4.440000000000000000000000000000e+0
divide
    27.7500000000
2.775000000000000000000000000000e+1
*/
