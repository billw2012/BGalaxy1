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
    Test:  xatanh  xasinh  xacosh

    Uses:  atox  xprcmp  xadd  xtodbl  xprxpr

*/
#include <stdio.h>
#include "xpre.h"

int decd = 30;

extern int xErrNo;

int
main (void)
{
  struct xpr z, w, f, u;
  int k;
  char cf[3];

  cf[0] = 's';
  cf[1] = 'c';
  cf[2] = 't';
  for (k = 0; k < 3; ++k)
    {
      switch (cf[k])
	{
	case 't':
	  printf ("     Test of Atanh Function\n");
	  break;
	case 's':
	  printf ("     Test of Asinh Function\n");
	  break;
	case 'c':
	  printf ("     Test of Acosh Function\n");
	  break;
	}
      z = atox ("-3.10");
      w = atox ("0.2");
      u = atox ("3");
      for (; xprcmp (&z, &u) < 0; z = xadd (z, w, 0))
	{
	  xErrNo = 0;
	  /* compute selected extended precision hyperbolic function */
	  switch (cf[k])
	    {
	    case 't':
	      f = xatanh (z);
	      break;
	    case 's':
	      f = xasinh (z);
	      break;
	    case 'c':
	      f = xacosh (z);
	      break;
	    }
	  if (!xErrNo)
	    {
	      printf (" %8.4f  ", xtodbl (z));
	      xprxpr (f, decd);
	      putchar ('\n');
	    }
	  else
	    printf ("*** Out of range\n");
	}
    }
  return 0;
}

/*  Test output

     Test of Asinh Function
  -3.1000  -1.849603679473178403404924022625e+0
  -2.9000  -1.786340199816898070076307640222e+0
  -2.7000  -1.719051851208927298040388668389e+0
  -2.5000  -1.647231146371095710624858610444e+0
  -2.3000  -1.570278543484978135111481318975e+0
  -2.1000  -1.487482836641271173630723524207e+0
  -1.9000  -1.397998365111434357649100132762e+0
  -1.7000  -1.300820426840646854388594687497e+0
  -1.5000  -1.194763217287109304111930828519e+0
  -1.3000  -1.078451058954896981725671055940e+0
  -1.1000  -9.503469298211342502700715942699e-1
  -0.9000  -8.088669356527824625093501673816e-1
  -0.7000  -6.526665660823557868086863441097e-1
  -0.5000  -4.812118250596034474977589134244e-1
  -0.3000  -2.956730475634224391027105297335e-1
  -0.1000  -9.983407889920756332730312470477e-2
   0.1000  9.983407889920756332730312470476e-2
   0.3000  2.956730475634224391027105297335e-1
   0.5000  4.812118250596034474977589134244e-1
   0.7000  6.526665660823557868086863441097e-1
   0.9000  8.088669356527824625093501673816e-1
   1.1000  9.503469298211342502700715942699e-1
   1.3000  1.078451058954896981725671055940e+0
   1.5000  1.194763217287109304111930828519e+0
   1.7000  1.300820426840646854388594687497e+0
   1.9000  1.397998365111434357649100132762e+0
   2.1000  1.487482836641271173630723524207e+0
   2.3000  1.570278543484978135111481318975e+0
   2.5000  1.647231146371095710624858610444e+0
   2.7000  1.719051851208927298040388668389e+0
   2.9000  1.786340199816898070076307640222e+0
     Test of Acosh Function
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
   1.1000  4.435682543851151891329110663525e-1
   1.3000  7.564329108569595862420768069687e-1
   1.5000  9.624236501192068949955178268487e-1
   1.7000  1.123230982587295889531145796228e+0
   1.9000  1.257195826600380434544695230560e+0
   2.1000  1.372859144242579840478282154314e+0
   2.3000  1.475044781241425079951043152436e+0
   2.5000  1.566799236972411078664056862580e+0
   2.7000  1.650193454979474903275879110697e+0
   2.9000  1.726710865622892121126949554563e+0
     Test of Atanh Function
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
  -0.9000  -1.472219489583220230004513715944e+0
  -0.7000  -8.673005276940531944271446904753e-1
  -0.5000  -5.493061443340548456976226184613e-1
  -0.3000  -3.095196042031117154740673490611e-1
  -0.1000  -1.003353477310755806357265520600e-1
   0.1000  1.003353477310755806357265520600e-1
   0.3000  3.095196042031117154740673490611e-1
   0.5000  5.493061443340548456976226184613e-1
   0.7000  8.673005276940531944271446904753e-1
   0.9000  1.472219489583220230004513715944e+0
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*** Out of range
*/
