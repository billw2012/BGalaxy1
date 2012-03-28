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

#include<stdio.h>
#include"xpre.h"

void
canonic_form (struct xpr *px)
{
  unsigned short *p, u;
  short e, i, j, skip;

  p = (unsigned short *) px;
  e = (*p & xM_exp);		/* biased exponent of x */
  u = (*p & xM_sgn);		/* sign of x            */
  if (e < xBias - 1)
    return;
  else
    {
      unsigned short mask = 0xffff;

      /* e >= xBias - 1 */
      for (i = 1, skip = e + 1 - xBias; skip / 16 > 0; i++, skip -= 16);
      if (i <= XDIM)
	{
	  /* skip = 0, ..., 15 */
	  mask >>= skip;
	  if ((p[i] & mask) != mask)
	    return;
	  else
	    {
	      for (j = i + 1; j <= XDIM && p[j] == 0xffff; j++);
	      if (j > XDIM)
		{
		  p[i] -= mask;
		  for (j = i + 1; j <= XDIM; p[j] = 0, j++);
		  if (!(p[1] & 0x8000))
		    {
		      p[1] = 0x8000;
		      *p = ++e;
		      *p |= u;
		    }
		  else if ((u))
		    *px = xadd (*px, xOne, 1);
		  else
		    *px = xadd (*px, xOne, 0);
		}
	    }
	}			/* end   if(i <= XDIM ) */
    }				/* end outer else */
}

int
main (void)
{
  struct xpr six = {
    0x4001, 0xbfff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
  };
  struct xpr seven = {
    0x4001, 0xdfff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
  };
  struct xpr eight = {
    0x4001, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
  };
  struct xpr one_two_seven = {
    0x4005, 0xfdff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
  };

  struct xpr msix = {
    0xc001, 0xbfff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
  };
  struct xpr mseven = {
    0xc001, 0xdfff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
  };
  struct xpr meight = {
    0xc001, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
  };
  struct xpr mone_two_seven = {
    0xc005, 0xfdff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
  };

  struct xpr one_half = {
    0x3ffd, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
  };
  struct xpr _1_ = {
    0x3ffe, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
  };
  struct xpr almost_seven = {
    0x4001, 0xdfff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xfffe
  };
  struct xpr near_seven = {
    0x4001, 0xdffe, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
  };

  struct xpr Zero = xZero;
  struct xpr One = xOne;

  printf ("\nsix=\n");
  xbprint (stdout, six);
  canonic_form (&six);
  printf ("six=\n");
  xbprint (stdout, six);

  printf ("msix=\n");
  xbprint (stdout, msix);
  canonic_form (&msix);
  printf ("msix=\n");
  xbprint (stdout, msix);

  printf ("seven=\n");
  xbprint (stdout, seven);
  canonic_form (&seven);
  printf ("seven=\n");
  xbprint (stdout, seven);

  printf ("mseven=\n");
  xbprint (stdout, mseven);
  canonic_form (&mseven);
  printf ("mseven=\n");
  xbprint (stdout, mseven);

  printf ("eight=\n");
  xbprint (stdout, eight);
  canonic_form (&eight);
  printf ("eight=\n");
  xbprint (stdout, eight);

  printf ("meight=\n");
  xbprint (stdout, meight);
  canonic_form (&meight);
  printf ("meight=\n");
  xbprint (stdout, meight);

  printf ("one_two_seven=\n");
  xbprint (stdout, one_two_seven);
  canonic_form (&one_two_seven);
  printf ("one_two_seven=\n");
  xbprint (stdout, one_two_seven);

  printf ("mone_two_seven=\n");
  xbprint (stdout, mone_two_seven);
  canonic_form (&mone_two_seven);
  printf ("mone_two_seven=\n");
  xbprint (stdout, mone_two_seven);

  printf ("one_half=\n");
  xbprint (stdout, one_half);
  canonic_form (&one_half);
  printf ("one_half=\n");
  xbprint (stdout, one_half);

  printf ("_1_=\n");
  xbprint (stdout, _1_);
  canonic_form (&_1_);
  printf ("_1_=\n");
  xbprint (stdout, _1_);

  printf ("almost_seven=\n");
  xbprint (stdout, almost_seven);
  canonic_form (&almost_seven);
  printf ("almost_seven=\n");
  xbprint (stdout, almost_seven);

  printf ("near_seven=\n");
  xbprint (stdout, near_seven);
  canonic_form (&near_seven);
  printf ("near_seven=\n");
  xbprint (stdout, near_seven);

  printf ("zero=\n");
  xbprint (stdout, Zero);
  canonic_form (&Zero);
  printf ("Zero=\n");
  xbprint (stdout, Zero);

  printf ("One=\n");
  xbprint (stdout, One);
  canonic_form (&One);
  printf ("One=\n");
  xbprint (stdout, One);
  return 0;
}

/* Test output


six=
0100000000000001.1011111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 
six=
0100000000000001.1100000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 
msix=
1100000000000001.1011111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 
msix=
1100000000000001.1100000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 
seven=
0100000000000001.1101111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 
seven=
0100000000000001.1110000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 
mseven=
1100000000000001.1101111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 
mseven=
1100000000000001.1110000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 
eight=
0100000000000001.1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 
eight=
0100000000000010.1000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 
meight=
1100000000000001.1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 
meight=
1100000000000010.1000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 
one_two_seven=
0100000000000101.1111110111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 
one_two_seven=
0100000000000101.1111111000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 
mone_two_seven=
1100000000000101.1111110111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 
mone_two_seven=
1100000000000101.1111111000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 
one_half=
0011111111111101.1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 
one_half=
0011111111111101.1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 
_1_=
0011111111111110.1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 
_1_=
0011111111111111.1000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 
almost_seven=
0100000000000001.1101111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111110 
almost_seven=
0100000000000001.1101111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111110 
near_seven=
0100000000000001.1101111111111110 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 
near_seven=
0100000000000001.1101111111111110 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 
zero=
0000000000000000.0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 
Zero=
0000000000000000.0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 
One=
0011111111111111.1000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 
One=
0011111111111111.1000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 
*/
