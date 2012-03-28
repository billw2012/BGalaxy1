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
    Test:  xfrexp  xpr2

    Uses:  atox  xprxpr

    Input file: data/xfrex.dat
*/
#include <stdio.h>
#include <stdlib.h>
#include "xpre.h"

int decd = 30;

int
main (int na, char **av)
{
  struct xpr s;
  int e;
  char str[64];
  FILE *fp;

  if (na != 2)
    {
      printf ("para: data_file\n");
      exit (-1);
    }
  fp = fopen (*++av, "r");
  while (fscanf (fp, "%s", str) != EOF)
    {
      s = atox (str);
      printf (" s-in= ");
      xprxpr (s, decd);

      /* extract exponent of extended precision number */
      s = xfrexp (s, &e);
      printf ("\n\n exp= %d\n", e);
      printf (" <s>= ");
      xprxpr (s, decd);

      /* restore exponent of extended precision number */
      s = xpr2 (s, e);
      printf ("\n\n  s1= ");
      xprxpr (s, decd);
      printf ("\n\n");
    }
  return 0;
}

/*  Test output

 s-in= 1.234500000000000000000000000000e+0

 exp= 1
 <s>= 6.172500000000000000000000000000e-1

  s1= 1.234500000000000000000000000000e+0

 s-in= -2.123450987654321000000000000000e+0

 exp= 2
 <s>= -5.308627469135802500000000000000e-1

  s1= -2.123450987654321000000000000000e+0

*/
