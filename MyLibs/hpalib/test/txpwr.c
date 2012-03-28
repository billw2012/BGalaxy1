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
    Test:  xpwr

    Uses:  atox  xprxpr

    Input parameters:  test_num -> extended precision: test number
                       max_pwr -> integer: maximum power computed
*/
#include <stdio.h>
#include <stdlib.h>
#include "xpre.h"

int decd = 30;

int
main (int na, char **av)
{
  struct xpr s, t;
  int i, m;

  if (na != 3)
    {
      printf ("para: test_num max_pwr\n");
      exit (-1);
    }
  printf ("     Test of Extended Precision Integer Powers\n");
  s = atox (*++av);
  m = atoi (*++av);
  printf (" input s= ");
  xprxpr (s, decd);
  putchar ('\n');
  for (i = 2; i <= m; ++i)
    {
      /* compute ith power of input number */
      t = xpwr (s, i);
      printf (" s^%d = ", i);
      xprxpr (t, decd);
      putchar ('\n');
    }
  return 0;
}

/*  Test output

     Test of Extended Precision Integer Powers
 input s= -7.000000000000000000000000000000e-1
 s^2 = 4.900000000000000000000000000000e-1
 s^3 = -3.430000000000000000000000000000e-1
 s^4 = 2.401000000000000000000000000000e-1
 s^5 = -1.680700000000000000000000000000e-1
 s^6 = 1.176490000000000000000000000000e-1
*/
