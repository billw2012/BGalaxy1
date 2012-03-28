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
    Test:  xchcof  xevtch

    Uses:  xdiv  xmul  xadd  inttox  xpr2  xtodbl  xprxpr
           xsin  xsqrt  x_exp

    Input parameter:  m -> integer: maximum degree of
                                    Tchebycheff polynominal
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "xpre.h"

struct xpr isin (struct xpr z);

int
main (int na, char **av)
{
  struct xpr z, dz, u, h;
  int j, m;
  struct xpr *c;
  double y;

  if (na != 2)
    {
      printf ("para: ncof\n");
      exit (-1);
    }
  printf ("     Test of Tchebycheff Expansion\n");
  m = atoi (*++av);

  /* compute Tchebycheff expansion coefficients */
  c = xchcof (m, isin);
  printf (" series coefficients\n");
  for (j = 0; j <= m;)
    {
      printf (" %2d  ", j);
      xpr_print (stdout, c[j++], 1, 1, 30);
      putchar ('\n');
    }
  c[0] = xpr2 (c[0], -1);
  z = xZero;
  dz = xdiv (xOne, inttox (10));
  printf ("     z          sin(z)             Tchebycheff series\n");
  for (j = 0; j < 16; ++j, z = xadd (z, dz, 0))
    {
      /* scale expansion variables */
      h = xdiv (z, xPi2);
      u = xadd (xpr2 (xmul (h, h), 1), xOne, 1);

      /* evaluate Tchebycheff series for sin(x) */
      u = xmul (h, xevtch (u, c, m));
      y = xtodbl (z);
      printf (" %6.2f %17.15f  ", y, sin (y));
      xprxpr (u, 25);
      putchar ('\n');
    }
  free ((void*)c);
  return 0;
}

/* evaluate sin((pi/2)*y)/y with y=sqrt((1+z)/2) */
struct xpr
isin (struct xpr z)
{
  z = xsqrt (xpr2 (xadd (z, xOne, 0), -1));
  if (x_exp (&z) == -16383)
    return xPi2;
  else
    return xdiv (xsin (xmul (xPi2, z)), z);
}

/*  Test output

     Test of Tchebycheff Expansion
 series coefficients
  0  +2.552557924804531760415273944172e+0
  1  -2.852615691810360095702940903036e-1
  2  +9.118016006651802497767922609504e-3
  3  -1.365875135419666724364765329542e-4
  4  +1.184961857661690108290062477114e-6
  5  -6.702791603827441236048376155605e-9
  6  +2.667278599019659364897350084318e-11
  7  -7.872922121718594384361661445215e-14
  8  +1.792294735924872741814445918645e-16
  9  -3.242712736631485672296795646963e-19
 10  +4.774743247331700352283058230130e-22
 11  -5.833273589373253665487745391160e-25
 12  +6.008135424889910153613249185946e-28
     z          sin(z)             Tchebycheff series
   0.00 0.000000000000000  0.0000000000000000000000000e+0
   0.10 0.099833416646828  9.9833416646828152306814198e-2
   0.20 0.198669330795061  1.9866933079506121545941263e-1
   0.30 0.295520206661340  2.9552020666133957510532075e-1
   0.40 0.389418342308650  3.8941834230865049166631176e-1
   0.50 0.479425538604203  4.7942553860420300027328794e-1
   0.60 0.564642473395035  5.6464247339503535720094545e-1
   0.70 0.644217687237691  6.4421768723769105367261435e-1
   0.80 0.717356090899523  7.1735609089952276162717461e-1
   0.90 0.783326909627483  7.8332690962748338846138232e-1
   1.00 0.841470984807896  8.4147098480789650665250232e-1
   1.10 0.891207360061435  8.9120736006143533995180258e-1
   1.20 0.932039085967226  9.3203908596722634967013444e-1
   1.30 0.963558185417193  9.6355818541719296470134863e-1
   1.40 0.985449729988460  9.8544972998846018065947458e-1
   1.50 0.997494986604054  9.9749498660405443094172337e-1
*/
