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
    Test:  xfrac  xtrunc

    Uses:  atox  xfrac  xtrunc  xprxpr
    
    Input file:  data/xfrac.dat
*/

#include <stdio.h>
#include <stdlib.h>
#include "xpre.h"

#define BUFF_SIZE 1024		/* 1 Kb */
int decd = 30;

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
  printf ("     Test of the functions xfrac() and xtrunc()\n");
  while (fgets (buffer, BUFF_SIZE, fp) != NULL)
    {
      for (i = 0; buffer[i] != '\0'; i++);
      if (i > 0 && buffer[i - 1] == '\n')
	buffer[i - 1] = '\0';
      printf ("x\t   =  %s", buffer);
      x = atox (buffer);
      printf ("\nxfrac (x)  = ");
      xprxpr (xfrac (x), decd);
      printf ("\nxtrunc (x) = ");
      xprxpr (xtrunc (x), decd);
      printf ("\n\n");
    }
  fclose (fp);
  return 0;
}

/*  Test output

     Test of the functions xfrac() and xtrunc()
x	   =  7.0
xfrac (x)  = 0.000000000000000000000000000000e+0
xtrunc (x) = 7.000000000000000000000000000000e+0

x	   =  -7.0
xfrac (x)  = -0.000000000000000000000000000000e+0
xtrunc (x) = -7.000000000000000000000000000000e+0

x	   =  0.375
xfrac (x)  = 3.750000000000000000000000000000e-1
xtrunc (x) = 0.000000000000000000000000000000e+0

x	   =  -0.375
xfrac (x)  = -3.750000000000000000000000000000e-1
xtrunc (x) = 0.000000000000000000000000000000e+0

x	   =  16.0625
xfrac (x)  = 6.249999999999999999999999999999e-2
xtrunc (x) = 1.600000000000000000000000000000e+1

x	   =  -16.0625
xfrac (x)  = -6.249999999999999999999999999999e-2
xtrunc (x) = -1.600000000000000000000000000000e+1

x	   =  1158
xfrac (x)  = 0.000000000000000000000000000000e+0
xtrunc (x) = 1.158000000000000000000000000000e+3

x	   =  -1158
xfrac (x)  = -0.000000000000000000000000000000e+0
xtrunc (x) = -1.158000000000000000000000000000e+3

x	   =  1.0246e+8
xfrac (x)  = 0.000000000000000000000000000000e+0
xtrunc (x) = 1.024600000000000000000000000000e+8

x	   =  -1.0246e+8
xfrac (x)  = -0.000000000000000000000000000000e+0
xtrunc (x) = -1.024600000000000000000000000000e+8

x	   =  0.098e-20
xfrac (x)  = 9.800000000000000000000000000000e-22
xtrunc (x) = 0.000000000000000000000000000000e+0

x	   =  -0.098e-20
xfrac (x)  = -9.800000000000000000000000000000e-22
xtrunc (x) = 0.000000000000000000000000000000e+0

x	   =  0.096e-34
xfrac (x)  = 9.600000000000000000000000000000e-36
xtrunc (x) = 0.000000000000000000000000000000e+0

x	   =  -0.096e-34
xfrac (x)  = -9.600000000000000000000000000000e-36
xtrunc (x) = 0.000000000000000000000000000000e+0

x	   =  6.999999999e-0
xfrac (x)  = 9.999999990000000000000000000000e-1
xtrunc (x) = 6.000000000000000000000000000000e+0

x	   =  -6.999999999e-0
xfrac (x)  = -9.999999990000000000000000000000e-1
xtrunc (x) = -6.000000000000000000000000000000e+0

x	   =  6.999999999999999999999999999999999997
xfrac (x)  = 1.000000000000000000000000000000e+0
xtrunc (x) = 6.000000000000000000000000000000e+0

x	   =  -6.999999999999999999999999999999999997
xfrac (x)  = -1.000000000000000000000000000000e+0
xtrunc (x) = -6.000000000000000000000000000000e+0

x	   =  2.00000000000001
xfrac (x)  = 9.999999999999999999748673211922e-15
xtrunc (x) = 2.000000000000000000000000000000e+0

x	   =  -2.00000000000001
xfrac (x)  = -9.999999999999999999748673211922e-15
xtrunc (x) = -2.000000000000000000000000000000e+0

x	   =  2.000000000000000000000000000000000004
xfrac (x)  = 0.000000000000000000000000000000e+0
xtrunc (x) = 2.000000000000000000000000000000e+0

x	   =  -2.000000000000000000000000000000000004
xfrac (x)  = -0.000000000000000000000000000000e+0
xtrunc (x) = -2.000000000000000000000000000000e+0

x	   =  897.5E-1
xfrac (x)  = 7.500000000000000000000000000000e-1
xtrunc (x) = 8.900000000000000000000000000000e+1

x	   =  -897.5E-1
xfrac (x)  = -7.500000000000000000000000000000e-1
xtrunc (x) = -8.900000000000000000000000000000e+1

x	   =  897.5
xfrac (x)  = 4.999999999999999999999999999998e-1
xtrunc (x) = 8.970000000000000000000000000000e+2

x	   =  -897.5
xfrac (x)  = -4.999999999999999999999999999998e-1
xtrunc (x) = -8.970000000000000000000000000000e+2

x	   =  0.0
xfrac (x)  = 0.000000000000000000000000000000e+0
xtrunc (x) = 0.000000000000000000000000000000e+0

x	   =  -0.0
xfrac (x)  = 0.000000000000000000000000000000e+0
xtrunc (x) = 0.000000000000000000000000000000e+0

*/
