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
    Test:  xfmod  xfrexp  xpr2

    Uses:  atox  xprxpr

    Input parameters:  x -> extended precision: first argument
                       y -> extended precision: second argument
*/
#include <stdio.h>
#include <stdlib.h>
#include "xpre.h"

int decd = 30;

int
main (int na, char **av)
{
  struct xpr s, t, f, q;
  int k;

  if (na != 3)
    {
      printf ("para: x y\n");
      exit (-1);
    }
  printf ("     Test of FMOD functions\n");
  s = atox (*++av);
  t = atox (*++av);
  printf (" inputs:\n");
  printf (" s=  ");
  xprxpr (s, decd);
  printf ("\n t=  ");
  xprxpr (t, decd);

  /* extended precision modular divide */
  f = xfmod (s, t, &q);
  printf ("\n s mod t = ");
  xprxpr (f, decd);
  printf ("\n       q = ");
  xprxpr (q, decd);
  printf ("\n\n s=  ");
  xprxpr (s, decd);

  /* extended precision exponent extraction */
  s = xfrexp (s, &k);
  printf ("\n exp= %d\n", k);
  printf (" <s>=  ");
  xprxpr (s, decd);

  /* extended precision power of two */
  s = xpr2 (s, k);
  printf ("\n   s=  ");
  xprxpr (s, decd);
  putchar ('\n');
  return 0;
}

/*  Test output

     Test of FMOD functions
 inputs:
 s=  1.250000000000000000000000000000e+1
 t=  3.000000000000000000000000000000e+0
 s mod t = 5.000000000000000000000000000000e-1
       q = 4.000000000000000000000000000000e+0

 s=  1.250000000000000000000000000000e+1
 exp= 4
 <s>=  7.812500000000000000000000000000e-1
   s=  1.250000000000000000000000000000e+1
*/
