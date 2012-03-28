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
  Test: cxreset  cxconv  cxre cxim cxswap  dctocx  fctocx  ictocx  
        cxtodc   cxtofc  CXRE CXIM CXSWAP  CXCONV  CXRESET  

  Uses: All the tested functions and macros  +  atox  xpr_print
*/

#include<stdio.h>
#include<cxpre.h>

#define DECD 30

int
main (void)
{
  struct xpr re, im;
  struct cxpr z;
#define DIM 7

  char *nlist[DIM][2] = {
    {"+2.0", "+0.0"},
    {"+0.0", "+7.0"},
    {"+3.0", "+4.0"},
    {"+8.91e3", "-10.25e-4"},
    {"-7.5", "-788.61"},
    {"-6.5524", "+99.2e5"},
    {"-556.72", "-98.76"}
  };
  double dlist[DIM][2] = {
    {0.0, 0.0},
    {3.0, 0.0},
    {0.0, -4.0},
    {3.0, 4.0},
    {6.7, -9.4},
    {-112.43e5, -78.543e-5},
    {-4.63e204, 3.97e-201}
  };
  float flist[DIM][2] = {
    {0.0, 0.0},
    {3.0, 0.0},
    {0.0, -4.0},
    {3.0, 4.0},
    {6.7, -9.4},
    {-112.43e5, -78.543e-5},
    {-4.63e20, 3.97e-21}
  };
  long ilist[DIM][2] = {
    {0, 0},
    {7, 0},
    {0, -6},
    {9, 3},
    {51, -76},
    {-4578, -8965},
    {-6541, 32000}
  };
  int i;
  double dr, di;
  float fr, fi;

  printf ("TEST OF THE BASIC COMPLEX FUNCTIONS AND MACROS");
  for (i = 0; i < DIM; i++)
    {
      printf ("\n\nz     =  %s%si\n", nlist[i][0], nlist[i][1]);
      re = atox (nlist[i][0]);
      im = atox (nlist[i][1]);
      z = cxreset (re, im);
      printf ("re(z) = ");
      xpr_print (stdout, cxre (z), 0, 1, DECD);
      printf ("\nim(z) = ");
      xpr_print (stdout, cxim (z), 0, 1, DECD);
      z = cxswap (z);
      printf ("\nAFTER SWAPPING\n");
      printf ("re(z) = ");
      xpr_print (stdout, cxre (z), 0, 1, DECD);
      printf ("\nim(z) = ");
      xpr_print (stdout, cxim (z), 0, 1, DECD);
      printf ("\nONLY THE REAL PART\n");
      z = cxconv (re);
      printf ("re(z) = ");
      xpr_print (stdout, cxre (z), 0, 1, DECD);
      printf ("\nim(z) = ");
      xpr_print (stdout, cxim (z), 0, 1, DECD);
    }
  printf ("\n\nNOW WE ARE GOING TO TEST THE MACROS\n");
  for (i = 0; i < DIM; i++)
    {
      printf ("\n\nz     =  %s%si\n", nlist[i][0], nlist[i][1]);
      re = atox (nlist[i][0]);
      im = atox (nlist[i][1]);
      z = CXRESET (re, im);
      printf ("re(z) = ");
      xpr_print (stdout, CXRE (z), 0, 1, DECD);
      printf ("\nim(z) = ");
      xpr_print (stdout, CXIM (z), 0, 1, DECD);
      z = CXSWAP (z);
      printf ("\nAFTER SWAPPING\n");
      printf ("re(z) = ");
      xpr_print (stdout, CXRE (z), 0, 1, DECD);
      printf ("\nim(z) = ");
      xpr_print (stdout, CXIM (z), 0, 1, DECD);
      printf ("\nONLY THE REAL PART\n");
      z = CXCONV (re);
      printf ("re(z) = ");
      xpr_print (stdout, CXRE (z), 0, 1, DECD);
      printf ("\nim(z) = ");
      xpr_print (stdout, CXIM (z), 0, 1, DECD);
    }
  printf ("\n\nTESTS FOR TYPE CONVERSIONS\n");
  for (i = 0; i < DIM; i++)
    {
      printf ("\n\nz     =  %+G%+Gi\n", dlist[i][0], dlist[i][1]);
      z = dctocx (dlist[i][0], dlist[i][1]);
      printf ("re(z) = ");
      xpr_print (stdout, CXRE (z), 0, 1, DECD);
      printf ("\nim(z) = ");
      xpr_print (stdout, CXIM (z), 0, 1, DECD);
      cxtodc (&z, &dr, &di);
      printf ("\nz     =  %+G%+Gi\n", dr, di);

      printf ("\nz     =  %+G%+Gi\n", flist[i][0], flist[i][1]);
      z = fctocx (flist[i][0], flist[i][1]);
      printf ("re(z) = ");
      xpr_print (stdout, CXRE (z), 0, 1, DECD);
      printf ("\nim(z) = ");
      xpr_print (stdout, CXIM (z), 0, 1, DECD);
      cxtofc (&z, &fr, &fi);
      printf ("\nz     =  %+G%+Gi\n", fr, fi);

      printf ("\nz     =  %+d%+di\n", ilist[i][0], ilist[i][1]);
      z = ictocx (ilist[i][0], ilist[i][1]);
      printf ("re(z) = ");
      xpr_print (stdout, CXRE (z), 0, 1, DECD);
      printf ("\nim(z) = ");
      xpr_print (stdout, CXIM (z), 0, 1, DECD);
    }
  putchar ('\n');
  return 0;
}

/* Test output

TEST OF THE BASIC COMPLEX FUNCTIONS AND MACROS

z     =  +2.0+0.0i
re(z) = +2.000000000000000000000000000000
im(z) = +0.000000000000000000000000000000
AFTER SWAPPING
re(z) = +0.000000000000000000000000000000
im(z) = +2.000000000000000000000000000000
ONLY THE REAL PART
re(z) = +2.000000000000000000000000000000
im(z) = +0.000000000000000000000000000000

z     =  +0.0+7.0i
re(z) = +0.000000000000000000000000000000
im(z) = +7.000000000000000000000000000000
AFTER SWAPPING
re(z) = +7.000000000000000000000000000000
im(z) = +0.000000000000000000000000000000
ONLY THE REAL PART
re(z) = +0.000000000000000000000000000000
im(z) = +0.000000000000000000000000000000

z     =  +3.0+4.0i
re(z) = +3.000000000000000000000000000000
im(z) = +4.000000000000000000000000000000
AFTER SWAPPING
re(z) = +4.000000000000000000000000000000
im(z) = +3.000000000000000000000000000000
ONLY THE REAL PART
re(z) = +3.000000000000000000000000000000
im(z) = +0.000000000000000000000000000000

z     =  +8.91e3-10.25e-4i
re(z) = +8910.000000000000000000000000000
im(z) = -0.001025000000000000000000000000000
AFTER SWAPPING
re(z) = -0.001025000000000000000000000000000
im(z) = +8910.000000000000000000000000000
ONLY THE REAL PART
re(z) = +8910.000000000000000000000000000
im(z) = +0.000000000000000000000000000000

z     =  -7.5-788.61i
re(z) = -7.500000000000000000000000000000
im(z) = -788.6100000000000000000000000000
AFTER SWAPPING
re(z) = -788.6100000000000000000000000000
im(z) = -7.500000000000000000000000000000
ONLY THE REAL PART
re(z) = -7.500000000000000000000000000000
im(z) = +0.000000000000000000000000000000

z     =  -6.5524+99.2e5i
re(z) = -6.552400000000000000000000000000
im(z) = +9920000.000000000000000000000000
AFTER SWAPPING
re(z) = +9920000.000000000000000000000000
im(z) = -6.552400000000000000000000000000
ONLY THE REAL PART
re(z) = -6.552400000000000000000000000000
im(z) = +0.000000000000000000000000000000

z     =  -556.72-98.76i
re(z) = -556.7200000000000000000000000000
im(z) = -98.76000000000000000000000000000
AFTER SWAPPING
re(z) = -98.76000000000000000000000000000
im(z) = -556.7200000000000000000000000000
ONLY THE REAL PART
re(z) = -556.7200000000000000000000000000
im(z) = +0.000000000000000000000000000000

NOW WE ARE GOING TO TEST THE MACROS


z     =  +2.0+0.0i
re(z) = +2.000000000000000000000000000000
im(z) = +0.000000000000000000000000000000
AFTER SWAPPING
re(z) = +0.000000000000000000000000000000
im(z) = +2.000000000000000000000000000000
ONLY THE REAL PART
re(z) = +2.000000000000000000000000000000
im(z) = +0.000000000000000000000000000000

z     =  +0.0+7.0i
re(z) = +0.000000000000000000000000000000
im(z) = +7.000000000000000000000000000000
AFTER SWAPPING
re(z) = +7.000000000000000000000000000000
im(z) = +0.000000000000000000000000000000
ONLY THE REAL PART
re(z) = +0.000000000000000000000000000000
im(z) = +0.000000000000000000000000000000

z     =  +3.0+4.0i
re(z) = +3.000000000000000000000000000000
im(z) = +4.000000000000000000000000000000
AFTER SWAPPING
re(z) = +4.000000000000000000000000000000
im(z) = +3.000000000000000000000000000000
ONLY THE REAL PART
re(z) = +3.000000000000000000000000000000
im(z) = +0.000000000000000000000000000000

z     =  +8.91e3-10.25e-4i
re(z) = +8910.000000000000000000000000000
im(z) = -0.001025000000000000000000000000000
AFTER SWAPPING
re(z) = -0.001025000000000000000000000000000
im(z) = +8910.000000000000000000000000000
ONLY THE REAL PART
re(z) = +8910.000000000000000000000000000
im(z) = +0.000000000000000000000000000000

z     =  -7.5-788.61i
re(z) = -7.500000000000000000000000000000
im(z) = -788.6100000000000000000000000000
AFTER SWAPPING
re(z) = -788.6100000000000000000000000000
im(z) = -7.500000000000000000000000000000
ONLY THE REAL PART
re(z) = -7.500000000000000000000000000000
im(z) = +0.000000000000000000000000000000

z     =  -6.5524+99.2e5i
re(z) = -6.552400000000000000000000000000
im(z) = +9920000.000000000000000000000000
AFTER SWAPPING
re(z) = +9920000.000000000000000000000000
im(z) = -6.552400000000000000000000000000
ONLY THE REAL PART
re(z) = -6.552400000000000000000000000000
im(z) = +0.000000000000000000000000000000

z     =  -556.72-98.76i
re(z) = -556.7200000000000000000000000000
im(z) = -98.76000000000000000000000000000
AFTER SWAPPING
re(z) = -98.76000000000000000000000000000
im(z) = -556.7200000000000000000000000000
ONLY THE REAL PART
re(z) = -556.7200000000000000000000000000
im(z) = +0.000000000000000000000000000000

TESTS FOR TYPE CONVERSIONS


z     =  +0+0i
re(z) = +0.000000000000000000000000000000
im(z) = +0.000000000000000000000000000000
z     =  +0+0i

z     =  +0+0i
re(z) = +0.000000000000000000000000000000
im(z) = +0.000000000000000000000000000000
z     =  +0+0i

z     =  +0+0i
re(z) = +0.000000000000000000000000000000
im(z) = +0.000000000000000000000000000000

z     =  +3+0i
re(z) = +3.000000000000000000000000000000
im(z) = +0.000000000000000000000000000000
z     =  +3+0i

z     =  +3+0i
re(z) = +3.000000000000000000000000000000
im(z) = +0.000000000000000000000000000000
z     =  +3+0i

z     =  +7+0i
re(z) = +7.000000000000000000000000000000
im(z) = +0.000000000000000000000000000000

z     =  +0-4i
re(z) = +0.000000000000000000000000000000
im(z) = -4.000000000000000000000000000000
z     =  +0-4i

z     =  +0-4i
re(z) = +0.000000000000000000000000000000
im(z) = -4.000000000000000000000000000000
z     =  +0-4i

z     =  +0-6i
re(z) = +0.000000000000000000000000000000
im(z) = -6.000000000000000000000000000000

z     =  +3+4i
re(z) = +3.000000000000000000000000000000
im(z) = +4.000000000000000000000000000000
z     =  +3+4i

z     =  +3+4i
re(z) = +3.000000000000000000000000000000
im(z) = +4.000000000000000000000000000000
z     =  +3+4i

z     =  +9+3i
re(z) = +9.000000000000000000000000000000
im(z) = +3.000000000000000000000000000000

z     =  +6.7-9.4i
re(z) = +6.700000000000000177635683940025
im(z) = -9.400000000000000355271367880050
z     =  +6.7-9.4i

z     =  +6.7-9.4i
re(z) = +6.699999809265136718750000000000
im(z) = -9.399999618530273437500000000000
z     =  +6.7-9.4i

z     =  +51-76i
re(z) = +51.00000000000000000000000000000
im(z) = -76.00000000000000000000000000000

z     =  -1.1243E+07-0.00078543i
re(z) = -11243000.00000000000000000000000
im(z) = -0.0007854300000000000473929229194425
z     =  -1.1243E+07-0.00078543i

z     =  -1.1243E+07-0.00078543i
re(z) = -11243000.00000000000000000000000
im(z) = -0.0007854300201870501041412353515625
z     =  -1.1243E+07-0.00078543i

z     =  -4578-8965i
re(z) = -4578.000000000000000000000000000
im(z) = -8965.000000000000000000000000000

z     =  -4.63E+204+3.97E-201i
re(z) = -4630000000000000080273875551053000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
im(z) = +0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003969999999999999714276213254751
z     =  -4.63E+204+3.97E-201i

z     =  -4.63E+20+3.97E-21i
re(z) = -462999983594334584832.0000000000
im(z) = +0.000000000000000000003970000035572837868683469071326
z     =  -4.63E+20+3.97E-21i

z     =  -6541+32000i
re(z) = -6541.000000000000000000000000000
im(z) = +32000.00000000000000000000000000
*/
