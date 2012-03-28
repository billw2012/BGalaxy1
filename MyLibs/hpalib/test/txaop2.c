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
    Test:  x_neg  x_exp  xabs  xneg  xpr2

    Uses:  atox  xprxpr

    Input parameter:  x -> extended precision: test number
*/
#include <stdio.h>
#include <stdlib.h>
#include "xpre.h"

int decd = 30;

int
main (int na, char **av)
{
  struct xpr s, t;
  int k;

  if (na != 2)
    {
      printf ("para: test_num\n");
      exit (-1);
    }
  printf ("     Test of Utility Operations\n");
  s = atox (*++av);
  printf ("   input x= ");
  xprxpr (s, decd);

  /* t= -s */
  t = xneg (s);
  printf ("\n t=xneg(x)= ");
  xprxpr (t, decd);

  /* s -> -s */
  x_neg (&s);
  printf ("\n x_neg(x) = ");
  xprxpr (s, decd);

  /* extract exponent */
  k = x_exp (&s);
  printf ("\n x-exp= k = %d\n", k);
  s = xpr2 (s, -k);
  printf ("      <s> = ");
  xprxpr (s, decd);

  /* restore exponent */
  s = xpr2 (s, k);
  printf ("\n  <s>*2^k = ");
  xprxpr (s, decd);

  /* s -> |s| */
  t = xabs (s);
  printf ("\n t=abs(s) = ");
  xprxpr (t, decd);
  putchar ('\n');
  return 0;
}

/*  Test output

     Test of Utility Operations
   input x= -1.240000000000000000000000000000e-3
 t=xneg(x)= 1.240000000000000000000000000000e-3
 x_neg(x) = -1.240000000000000000000000000000e-3
 x-exp= k = -10
      <s> = -1.269760000000000000000000000000e+0
  <s>*2^k = -1.240000000000000000000000000000e-3
 t=abs(s) = 1.240000000000000000000000000000e-3
*/
