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
  Test:  xisNaN  xisPinf  xisMinf  xisordnumb  xis0  xnot0  xsgn

  Uses:  xisNaN  xisPinf  xisMinf  xisordnumb  xis0  xnot0  xsgn  xprint
*/
#include <stdio.h>
#include <math.h>
#include "xpre.h"

int decd = 30;
#define __N__ 17

int
main (void)
{
  struct xpr a[__N__] = {
    xZero,
    xOne,
    xTwo,
    xPi,
    xZero,
    xZero,
    xZero,
    xZero,
    xZero,
    xZero,
    xZero,
    xZero,
    xZero,
    xPinf,
    xMinf,
    xNaN,
    xZero
  };
  int i;

  a[4] = xpr2 (xTen, 1000);
  a[6].nmm[0] = a[5].nmm[0] = 0x7ffe;
  a[6].nmm[XDIM] = 0x0001;
  a[7] = xneg (xOne);
  a[8] = xneg (xTwo);
  a[9] = xneg (xPi);
  a[10] = xneg (a[4]);
  a[12].nmm[0] = a[11].nmm[0] = 0xfffe;
  a[12].nmm[XDIM] = 0x0001;
  a[16].nmm[XDIM] = 0x0001;
  printf ("Test of Special Comparison Functions\n\n");
  for (i = 0; i < __N__; i++)
    {
      printf ("a[%2d] == ", i);
      xprint (stdout, a[i]);
      if ( (xisNaN(a + i)) )
	printf ("a[%2d] == NaN\n", i);
      else
	printf ("a[%2d] != NaN\n", i);
      /* *** */
      if ( (xisPinf(a + i)) )
	printf ("a[%2d] == +oo\n", i);
      else
	printf ("a[%2d] != +oo\n", i);
      /* *** */
      if ( (xisMinf(a + i)) )
	printf ("a[%2d] == -oo\n", i);
      else
	printf ("a[%2d] != -oo\n", i);
      /* *** */
      if ( (xisordnumb(a + i)) )
	printf ("a[%2d] is an ordinary number\n", i);
      else
	printf ("a[%2d] is not an ordinary number\n", i);
      /* *** */
      if ( (xis0(a + i)) )
	printf ("a[%2d] is zero\n", i);
      else
	printf ("a[%2d] is not zero\n", i);
      /* *** */
      if ( (xnot0(a + i)) )
	printf ("a[%2d] is not zero\n", i);
      else
	printf ("a[%2d] is zero\n", i);
      /* *** */
      printf ("sgn(a[%2d]) == %d\n\n", i, xsgn(a + i));
    }
  return 0;
}

/*  Test output

Test of Special Comparison Functions

a[ 0] == 0000.0000000000000000000000000000
a[ 0] != NaN
a[ 0] != +oo
a[ 0] != -oo
a[ 0] is an ordinary number
a[ 0] is zero
a[ 0] is zero
sgn(a[ 0]) == 0

a[ 1] == 3fff.8000000000000000000000000000
a[ 1] != NaN
a[ 1] != +oo
a[ 1] != -oo
a[ 1] is an ordinary number
a[ 1] is not zero
a[ 1] is not zero
sgn(a[ 1]) == 1

a[ 2] == 4000.8000000000000000000000000000
a[ 2] != NaN
a[ 2] != +oo
a[ 2] != -oo
a[ 2] is an ordinary number
a[ 2] is not zero
a[ 2] is not zero
sgn(a[ 2]) == 1

a[ 3] == 4000.c90fdaa22168c234c4c6628b80dc
a[ 3] != NaN
a[ 3] != +oo
a[ 3] != -oo
a[ 3] is an ordinary number
a[ 3] is not zero
a[ 3] is not zero
sgn(a[ 3]) == 1

a[ 4] == 43ea.a000000000000000000000000000
a[ 4] != NaN
a[ 4] != +oo
a[ 4] != -oo
a[ 4] is an ordinary number
a[ 4] is not zero
a[ 4] is not zero
sgn(a[ 4]) == 1

a[ 5] == 7ffe.0000000000000000000000000000
a[ 5] != NaN
a[ 5] != +oo
a[ 5] != -oo
a[ 5] is an ordinary number
a[ 5] is zero
a[ 5] is zero
sgn(a[ 5]) == 0

a[ 6] == 7ffe.0000000000000000000000000001
a[ 6] != NaN
a[ 6] != +oo
a[ 6] != -oo
a[ 6] is an ordinary number
a[ 6] is not zero
a[ 6] is not zero
sgn(a[ 6]) == 1

a[ 7] == bfff.8000000000000000000000000000
a[ 7] != NaN
a[ 7] != +oo
a[ 7] != -oo
a[ 7] is an ordinary number
a[ 7] is not zero
a[ 7] is not zero
sgn(a[ 7]) == -1

a[ 8] == c000.8000000000000000000000000000
a[ 8] != NaN
a[ 8] != +oo
a[ 8] != -oo
a[ 8] is an ordinary number
a[ 8] is not zero
a[ 8] is not zero
sgn(a[ 8]) == -1

a[ 9] == c000.c90fdaa22168c234c4c6628b80dc
a[ 9] != NaN
a[ 9] != +oo
a[ 9] != -oo
a[ 9] is an ordinary number
a[ 9] is not zero
a[ 9] is not zero
sgn(a[ 9]) == -1

a[10] == c3ea.a000000000000000000000000000
a[10] != NaN
a[10] != +oo
a[10] != -oo
a[10] is an ordinary number
a[10] is not zero
a[10] is not zero
sgn(a[10]) == -1

a[11] == fffe.0000000000000000000000000000
a[11] != NaN
a[11] != +oo
a[11] != -oo
a[11] is an ordinary number
a[11] is zero
a[11] is zero
sgn(a[11]) == 0

a[12] == fffe.0000000000000000000000000001
a[12] != NaN
a[12] != +oo
a[12] != -oo
a[12] is an ordinary number
a[12] is not zero
a[12] is not zero
sgn(a[12]) == -1

a[13] == 7fff.0000000000000000000000000000
a[13] != NaN
a[13] == +oo
a[13] != -oo
a[13] is not an ordinary number
a[13] is not zero
a[13] is not zero
sgn(a[13]) == 1

a[14] == ffff.0000000000000000000000000000
a[14] != NaN
a[14] != +oo
a[14] == -oo
a[14] is not an ordinary number
a[14] is not zero
a[14] is not zero
sgn(a[14]) == -1

a[15] == 0000.ffffffffffffffffffffffffffff
a[15] == NaN
a[15] != +oo
a[15] != -oo
a[15] is not an ordinary number
a[15] is not zero
a[15] is not zero
sgn(a[15]) == 0

a[16] == 0000.0000000000000000000000000001
a[16] == NaN
a[16] != +oo
a[16] != -oo
a[16] is not an ordinary number
a[16] is not zero
a[16] is not zero
sgn(a[16]) == 0

*/
