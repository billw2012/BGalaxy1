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
    Test:  xdiv  xmul

    Uses:  dbltox

    Prompted input:  at prompt  'x y? : '
                         enter  x -> real: numerator
                                y -> real: denominator (y!=0)
                                y = 0 terminates session
*/
#include <stdio.h>
#include "xpre.h"

int decd = 30;

int
main (void)
{
  struct xpr f, s, t;
  double x, y;
  int k;

  while (1)
    {
      fprintf (stderr, "x y? : ");
      scanf ("%lf %lf", &x, &y);
      if (y == 0.)
	break;
      s = dbltox (x);
      t = dbltox (y);
      printf (" inputs: s= ");
      xprxpr (s, decd);
      printf ("\n         t= ");
      xprxpr (t, decd);

      /* extended precision division */
      f = xdiv (s, t);
      printf ("\ns/t: ");
      xprxpr (f, decd);

      /* check division with a multiply */
      s = xmul (f, t);
      printf ("\n  s: ");
      xprxpr (s, decd);
      putchar ('\n');
    }
  return 0;
}

/*  Test output

 inputs: s= 1.000000000000000000000000000000e+0
         t= 3.000000000000000000000000000000e+0
s/t: 3.333333333333333333333333333333e-1
  s: 1.000000000000000000000000000000e+0
 inputs: s= 4.000000000000000000000000000000e+0
         t= 7.000000000000000000000000000000e+0
s/t: 5.714285714285714285714285714286e-1
  s: 4.000000000000000000000000000000e+0
 inputs: s= 2.000000000000000000000000000000e+0
         t= 1.600000000000000000000000000000e+1
s/t: 1.250000000000000000000000000000e-1
  s: 2.000000000000000000000000000000e+0
*/
