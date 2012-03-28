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
    Test:  xprcmp

    Uses:  atox  xprxpr

    Input file:  data/xcmp.dat
*/
#include <stdio.h>
#include <stdlib.h>
#include "xpre.h"

int decd = 30;

int
main (int na, char **av)
{
  struct xpr s, t;
  int c;
  char nbx[64], nby[64];
  FILE *fp;

  if (na != 2)
    {
      printf ("para: input_file\n");
      exit (-1);
    }
  fp = fopen (*++av, "r");
  printf ("     Test of Extended Precision Comparison\n");
  while (fscanf (fp, "%s %s", nbx, nby) != EOF)
    {
      s = atox (nbx);
      t = atox (nby);
      printf (" x= ");
      xprxpr (s, decd);
      printf ("\n y= ");
      xprxpr (t, decd);

      printf ("\ncompare x and y   --   ");
      /* extended precision comparison */
      c = xprcmp (&s, &t);

      if (c == 1)
	printf (" x > y\n");
      else if (c == -1)
	printf (" x < y\n");
      else
	printf (" x = y\n");
    }
  return 0;
}

/*  Test output

     Test of Extended Precision Comparison
 x= 1.250000000000000000000000000000e+0
 y= 2.730000000000000000000000000000e+1
compare x and y   --    x < y
 x= -1.210000000000000000000000000000e-1
 y= 5.431000000000000000000000000000e-2
compare x and y   --    x < y
 x= 1.110000000000000000000000000000e+1
 y= -2.765400000000000000000000000000e+1
compare x and y   --    x > y
 x= -1.030000000000000000000000000000e-3
 y= -1.030000000000000000000000000000e-3
compare x and y   --    x = y
 x= -1.030000000000000000000000000000e-3
 y= -1.030000000000000000000000000000e-3
compare x and y   --    x = y
 x= 0.000000000000000000000000000000e+0
 y= 0.000000000000000000000000000000e+0
compare x and y   --    x = y
 x= 0.000000000000000000000000000000e+0
 y= 0.000000000000000000000000000000e+0
compare x and y   --    x = y
 x= 0.000000000000000000000000000000e+0
 y= 0.000000000000000000000000000000e+0
compare x and y   --    x = y
 x= 0.000000000000000000000000000000e+0
 y= 0.000000000000000000000000000000e+0
compare x and y   --    x = y
 x= 0.000000000000000000000000000000e+0
 y= -9.999999999999999999999999999996e-301
compare x and y   --    x > y
 x= 0.000000000000000000000000000000e+0
 y= 1.000000000000000000000000000000e-12
compare x and y   --    x < y
 x= 0.000000000000000000000000000000e+0
 y= -1.000000000000000000000000000000e-16
compare x and y   --    x > y
 x= 0.000000000000000000000000000000e+0
 y= 9.999999999999999999999999999997e-246
compare x and y   --    x < y
*/
