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
    Test:  xround  xfloor  xceil

    Uses:  atox  xround  xfloor  xceil  xprxpr
    
    Input file:  data/xround.dat
*/

#include <stdio.h>
#include <stdlib.h>
#include "xpre.h"

#define BUFF_SIZE 1024		/* 1 Kb */
int decd = 32;

int
main (int na, char **av)
{
  int i, n;
  struct xpr x;
  char buffer[BUFF_SIZE];
  FILE *fp;

  if (na != 2)
    {
      printf ("para: input_file\n");
      exit (-1);
    }
  fp = fopen (*++av, "r");
  printf ("     Test of the functions xround(), xfloor() and xceil()\n");
  while (fgets (buffer, BUFF_SIZE, fp) != NULL)
    {
      for (i = 0; buffer[i] != '\0'; i++);
      if (i > 0 && buffer[i - 1] == '\n')
	buffer[i - 1] = '\0';
      x = atox (buffer);
      printf ("\nx\t    = ");
      xprxpr (x, decd);
      printf ("\nxround (x)  = ");
      xprxpr (xround (x), decd);
      printf ("\nxfloor (x)  = ");
      xprxpr (xfloor (x), decd);
      printf ("\nxceil (x)   = ");
      xprxpr (xceil (x), decd);
      putchar ('\n');
    }
  return 0;
}

/*  Test output

     Test of the functions xround(), xfloor() and xceil()

x	    = 5.70000000000000000000000000000000e+1
xround (x)  = 5.70000000000000000000000000000000e+1
xfloor (x)  = 5.70000000000000000000000000000000e+1
xceil (x)   = 5.70000000000000000000000000000000e+1

x	    = 5.70000000000000000000000000000000e+1
xround (x)  = 5.70000000000000000000000000000000e+1
xfloor (x)  = 5.70000000000000000000000000000000e+1
xceil (x)   = 5.70000000000000000000000000000000e+1

x	    = -3.20000000000000000000000000000000e+1
xround (x)  = -3.20000000000000000000000000000000e+1
xfloor (x)  = -3.20000000000000000000000000000000e+1
xceil (x)   = -3.20000000000000000000000000000000e+1

x	    = -3.20000000000000000000000000000000e+1
xround (x)  = -3.20000000000000000000000000000000e+1
xfloor (x)  = -3.20000000000000000000000000000000e+1
xceil (x)   = -3.20000000000000000000000000000000e+1

x	    = 5.00000000000000000000000000000000e-1
xround (x)  = 1.00000000000000000000000000000000e+0
xfloor (x)  = 0.00000000000000000000000000000000e+0
xceil (x)   = 1.00000000000000000000000000000000e+0

x	    = -5.00000000000000000000000000000000e-1
xround (x)  = -1.00000000000000000000000000000000e+0
xfloor (x)  = -1.00000000000000000000000000000000e+0
xceil (x)   = 0.00000000000000000000000000000000e+0

x	    = 7.50000000000000000000000000000000e+0
xround (x)  = 8.00000000000000000000000000000000e+0
xfloor (x)  = 7.00000000000000000000000000000000e+0
xceil (x)   = 8.00000000000000000000000000000000e+0

x	    = -7.50000000000000000000000000000000e+0
xround (x)  = -8.00000000000000000000000000000000e+0
xfloor (x)  = -8.00000000000000000000000000000000e+0
xceil (x)   = -7.00000000000000000000000000000000e+0

x	    = 7.49999999999999999999999999999999e+0
xround (x)  = 8.00000000000000000000000000000000e+0
xfloor (x)  = 7.00000000000000000000000000000000e+0
xceil (x)   = 8.00000000000000000000000000000000e+0

x	    = -7.49999999999999999999999999999999e+0
xround (x)  = -8.00000000000000000000000000000000e+0
xfloor (x)  = -8.00000000000000000000000000000000e+0
xceil (x)   = -7.00000000000000000000000000000000e+0

x	    = 6.49999999999999999998999999999999e+0
xround (x)  = 6.00000000000000000000000000000000e+0
xfloor (x)  = 6.00000000000000000000000000000000e+0
xceil (x)   = 7.00000000000000000000000000000000e+0

x	    = -6.49999999999999999998999999999999e+0
xround (x)  = -6.00000000000000000000000000000000e+0
xfloor (x)  = -7.00000000000000000000000000000000e+0
xceil (x)   = -6.00000000000000000000000000000000e+0

x	    = 4.75000000000001000000000000000000e+1
xround (x)  = 4.80000000000000000000000000000000e+1
xfloor (x)  = 4.70000000000000000000000000000000e+1
xceil (x)   = 4.80000000000000000000000000000000e+1

x	    = -4.75000000000001000000000000000000e+1
xround (x)  = -4.80000000000000000000000000000000e+1
xfloor (x)  = -4.80000000000000000000000000000000e+1
xceil (x)   = -4.70000000000000000000000000000000e+1

x	    = 8.00000000000000019999999999999999e+0
xround (x)  = 8.00000000000000000000000000000000e+0
xfloor (x)  = 8.00000000000000000000000000000000e+0
xceil (x)   = 9.00000000000000000000000000000000e+0

x	    = -8.00000000000000019999999999999999e+0
xround (x)  = -8.00000000000000000000000000000000e+0
xfloor (x)  = -9.00000000000000000000000000000000e+0
xceil (x)   = -8.00000000000000000000000000000000e+0

x	    = 2.19999999999999999699999999999999e+1
xround (x)  = 2.20000000000000000000000000000000e+1
xfloor (x)  = 2.10000000000000000000000000000000e+1
xceil (x)   = 2.20000000000000000000000000000000e+1

x	    = -2.19999999999999999699999999999999e+1
xround (x)  = -2.20000000000000000000000000000000e+1
xfloor (x)  = -2.20000000000000000000000000000000e+1
xceil (x)   = -2.10000000000000000000000000000000e+1
*/
