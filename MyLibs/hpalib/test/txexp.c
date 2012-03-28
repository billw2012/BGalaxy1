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
    Test:  xexp  xexp2   xexp10

    Uses:  atox  xtodbl  xprcmp  xadd  xprxpr
*/
#include <stdio.h>
#include "xpre.h"

int decd = 30;

int
main (void)
{
  struct xpr z, h, f, u;

  printf ("     Test of Exp Functions\n");
  z = xneg (xOne);
  h = atox ("0.5");
  u = atox ("3.01");
  for (; xprcmp (&z, &u) < 0; z = xadd (z, h, 0))
    {
      /* compute extended precision exponential */
      f = xexp (z);
      printf (" %8.4f  ", xtodbl (z));
      xprxpr (f, decd);
      f = xexp2 (z);
      printf ("\n           ");
      xprxpr (f, decd);
      f = xexp10 (z);
      printf ("\n           ");
      xprxpr (f, decd);
      putchar ('\n');
    }
  return 0;
}

/*  Test output

     Test of Exp Functions
  -1.0000  3.678794411714423215955237701615e-1
           5.000000000000000000000000000000e-1
           1.000000000000000000000000000000e-1
  -0.5000  6.065306597126334236037995349912e-1
           7.071067811865475244008443621048e-1
           3.162277660168379331998893544433e-1
  -0.0000  1.000000000000000000000000000000e+0
           1.000000000000000000000000000000e+0
           1.000000000000000000000000000000e+0
   0.5000  1.648721270700128146848650787814e+0
           1.414213562373095048801688724210e+0
           3.162277660168379331998893544433e+0
   1.0000  2.718281828459045235360287471353e+0
           2.000000000000000000000000000000e+0
           1.000000000000000000000000000000e+1
   1.5000  4.481689070338064822602055460119e+0
           2.828427124746190097603377448419e+0
           3.162277660168379331998893544433e+1
   2.0000  7.389056098930650227230427460575e+0
           4.000000000000000000000000000000e+0
           1.000000000000000000000000000000e+2
   2.5000  1.218249396070347343807017595117e+1
           5.656854249492380195206754896839e+0
           3.162277660168379331998893544433e+2
   3.0000  2.008553692318766774092852965458e+1
           8.000000000000000000000000000000e+0
           1.000000000000000000000000000000e+3
*/
