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
    Test:  dbltox  xtodbl  inttox

    Uses:  xprxpr

    Input file:  data/xcvt.dat
*/
#include <stdio.h>
#include <stdlib.h>
#include "xpre.h"

int decd = 30;

int
main (int na, char **av)
{
  char f[4];
  double x, z;
  int k;
  struct xpr s;
  FILE *fp;

  if (na != 2)
    {
      printf ("para: input_file\n");
      exit (-1);
    }
  fp = fopen (*++av, "r");
  printf ("     Test of Conversion Functions\n");
  while (fscanf (fp, "%s", f) != EOF)
    {
      switch (*f)
	{
	  /* convert between double and extended precision */
	case 'd':
	  fscanf (fp, "%lf", &x);
	  s = dbltox (x);
	  z = xtodbl (s);
	  printf (" in: x= %.15e  out: z= %.15e\n", x, z);
	  xprxpr (s, decd);
	  putchar ('\n');
	  break;

	  /* convert integer to extended precision */
	case 'i':
	  fscanf (fp, "%d", &k);
	  s = inttox (k);
	  printf (" k= %d\n", k);
	  xprxpr (s, decd);
	  putchar ('\n');
	  break;
	}
    }
  return 0;
}

/*  Test output

     Test of Conversion Functions
 in: x= 1.375000000000000e+00  out: z= 1.375000000000000e+00
1.375000000000000000000000000000e+0
 in: x= 1.570796326794897e+00  out: z= 1.570796326794897e+00
1.570796326794896557998981734272e+0
 in: x= 2.000000000000000e+00  out: z= 2.000000000000000e+00
2.000000000000000000000000000000e+0
 in: x= 1.537000000000000e+25  out: z= 1.537000000000000e+25
1.536999999999999915694489600000e+25
 k= 1
1.000000000000000000000000000000e+0
 k= 123
1.230000000000000000000000000000e+2
 k= 214675321
2.146753210000000000000000000000e+8
*/
