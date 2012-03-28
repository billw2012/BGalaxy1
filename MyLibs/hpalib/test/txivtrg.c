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
    Test:  xatan  xasin  xacos

    Uses:  xtan  xsin  xcos  dbltox  xprxpr
*/
#include <stdio.h>
#include "xpre.h"

int decd = 30;

int
main (void)
{
  struct xpr z, f, c;
  int k;

  printf ("     Test of Inverse Trig Functions\n");
  printf ("\n Atan:\n");
  for (k = 0; k < 3; ++k)
    {
      switch (k)
	{
	case 0:
	  z = dbltox (.5);
	  break;
	case 1:
	  z = dbltox (1.);
	  break;
	case 2:
	  z = dbltox (-2.);
	  break;
	}

      /* compute extended precision arctangent and tangent */
      f = xatan (z);
      c = xtan (f);

      printf (" %8.4f   ", xtodbl (z));
      xprxpr (f, decd);
      printf ("\n check tan= ");
      xprxpr (c, decd);
      putchar ('\n');
    }
  printf ("\n Asin:\n");
  for (k = 0; k < 3; ++k)
    {
      switch (k)
	{
	case 0:
	  z = dbltox (.5);
	  break;
	case 1:
	  z = dbltox (.75);
	  break;
	case 2:
	  z = dbltox (.25);
	  break;
	}

      /* compute extended precision arcsin and sine */
      f = xasin (z);
      c = xsin (f);

      printf (" %8.4f   ", xtodbl (z));
      xprxpr (f, decd);
      printf ("\n check sin= ");
      xprxpr (c, decd);
      putchar ('\n');
    }
  printf ("\n Acos:\n");
  for (k = 0; k < 3; ++k)
    {
      switch (k)
	{
	case 0:
	  z = dbltox (.5);
	  break;
	case 1:
	  z = dbltox (.75);
	  break;
	case 2:
	  z = dbltox (.25);
	  break;
	}

      /* compute extended precision arccos and cosine */
      f = xacos (z);
      c = xcos (f);
      printf (" %8.4f   ", xtodbl (z));
      xprxpr (f, decd);
      printf ("\n check cos= ");
      xprxpr (c, decd);
      putchar ('\n');
    }
  return 0;
}

/*  Test output

     Test of Inverse Trig Functions

 Atan:
   0.5000   4.636476090008061162142562314612e-1
 check tan= 5.000000000000000000000000000000e-1
   1.0000   7.853981633974483096156608458199e-1
 check tan= 1.000000000000000000000000000000e+0
  -2.0000   -1.107148717794090503017065460179e+0
 check tan= -2.000000000000000000000000000000e+0

 Asin:
   0.5000   5.235987755982988730771072305466e-1
 check sin= 5.000000000000000000000000000000e-1
   0.7500   8.480620789814810080529443389984e-1
 check sin= 7.500000000000000000000000000000e-1
   0.2500   2.526802551420786534856574369937e-1
 check sin= 2.500000000000000000000000000000e-1

 Acos:
   0.5000   1.047197551196597746154214461093e+0
 check cos= 5.000000000000000000000000000000e-1
   0.7500   7.227342478134156111783773526413e-1
 check cos= 7.500000000000000000000000000000e-1
   0.2500   1.318116071652817965745664254646e+0
 check cos= 2.500000000000000000000000000000e-1
*/
