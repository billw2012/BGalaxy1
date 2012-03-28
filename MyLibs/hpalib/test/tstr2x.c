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
    Test:  strtox  xprxpr  xprint

    Input file:  data/str2x.dat
*/
#include<stdio.h>
#include<stdlib.h>
#include"xpre.h"

int decd = 30;

int
isNaN (const struct xpr* u)
{
  int i;

  for (i=0; i <= XDIM && u->nmm[i] == xNaN.nmm[i]; i++);
  return (i > XDIM ? 1 : 0);
}

int
get_line (char *buffer, int size, FILE * fp)
{
  if ((fgets (buffer, size, fp)))
    {
      int i;

      for (i = 0; buffer[i] != '\0'; i++);
      if (i > 0 && buffer[i - 1] == '\n')
	buffer[--i] = '\0';
      return i;
    }
  else
    return -1;
}

int
main (int na, char **av)
{
  struct xpr f;
  double x;
  FILE *fp;
  char num[256];
  char *endptr, *endptr2;

  if (na != 2)
    {
      printf ("para: input_file\n");
      exit (-1);
    }
  fp = fopen (*++av, "r");
  while (get_line (num, 256, fp) != -1)
    {
      printf ("*string in : \"%s\"\n", num);

      /* convert ascii string to extended precision */
      f = strtox (num, &endptr);
      printf (" endptr                : \"%s\"\n", endptr);
      x = strtod (num, &endptr2);
      printf (" endptr (from strtod()): \"%s\"\n", endptr2);
      if ((isNaN(&f)))
	printf (" NAN\n");
      else if (xprcmp (&f, &xPinf) == 0)
	printf (" OVERFLOW (pos.)\n");
      else if (xprcmp (&f, &xMinf) == 0)
	printf (" OVERFLOW (neg.)\n");
      else
	{
	  printf (" f =       ");
	  xprxpr (f, decd);
	  printf ("\n hex =       ");
	  xprint (stdout, f);
	}
    }
  return 0;
}

/*  Test output

*string in : "123.45"
 endptr                : ""
 endptr (from strtod()): ""
 f =       1.234500000000000000000000000000e+2
 hex =       4005.f6e6666666666666666666666666
*string in : ".45"
 endptr                : ""
 endptr (from strtod()): ""
 f =       4.500000000000000000000000000000e-1
 hex =       3ffd.e66666666666666666666666665e
*string in : "123"
 endptr                : ""
 endptr (from strtod()): ""
 f =       1.230000000000000000000000000000e+2
 hex =       4005.f600000000000000000000000000
*string in : "123."
 endptr                : ""
 endptr (from strtod()): ""
 f =       1.230000000000000000000000000000e+2
 hex =       4005.f600000000000000000000000000
*string in : "+123.45"
 endptr                : ""
 endptr (from strtod()): ""
 f =       1.234500000000000000000000000000e+2
 hex =       4005.f6e6666666666666666666666666
*string in : "+.45"
 endptr                : ""
 endptr (from strtod()): ""
 f =       4.500000000000000000000000000000e-1
 hex =       3ffd.e66666666666666666666666665e
*string in : "+123"
 endptr                : ""
 endptr (from strtod()): ""
 f =       1.230000000000000000000000000000e+2
 hex =       4005.f600000000000000000000000000
*string in : "+123."
 endptr                : ""
 endptr (from strtod()): ""
 f =       1.230000000000000000000000000000e+2
 hex =       4005.f600000000000000000000000000
*string in : "-123.45"
 endptr                : ""
 endptr (from strtod()): ""
 f =       -1.234500000000000000000000000000e+2
 hex =       c005.f6e6666666666666666666666666
*string in : "-.45"
 endptr                : ""
 endptr (from strtod()): ""
 f =       -4.500000000000000000000000000000e-1
 hex =       bffd.e66666666666666666666666665e
*string in : "-123"
 endptr                : ""
 endptr (from strtod()): ""
 f =       -1.230000000000000000000000000000e+2
 hex =       c005.f600000000000000000000000000
*string in : "-123."
 endptr                : ""
 endptr (from strtod()): ""
 f =       -1.230000000000000000000000000000e+2
 hex =       c005.f600000000000000000000000000
*string in : "45.3e3"
 endptr                : ""
 endptr (from strtod()): ""
 f =       4.530000000000000000000000000000e+4
 hex =       400e.b0f4000000000000000000000000
*string in : "45.3e+3"
 endptr                : ""
 endptr (from strtod()): ""
 f =       4.530000000000000000000000000000e+4
 hex =       400e.b0f4000000000000000000000000
*string in : "+45.3e3"
 endptr                : ""
 endptr (from strtod()): ""
 f =       4.530000000000000000000000000000e+4
 hex =       400e.b0f4000000000000000000000000
*string in : "+45.3e+3"
 endptr                : ""
 endptr (from strtod()): ""
 f =       4.530000000000000000000000000000e+4
 hex =       400e.b0f4000000000000000000000000
*string in : "-45.3e3"
 endptr                : ""
 endptr (from strtod()): ""
 f =       -4.530000000000000000000000000000e+4
 hex =       c00e.b0f4000000000000000000000000
*string in : "-45.3e+3"
 endptr                : ""
 endptr (from strtod()): ""
 f =       -4.530000000000000000000000000000e+4
 hex =       c00e.b0f4000000000000000000000000
*string in : "-45.3e-3"
 endptr                : ""
 endptr (from strtod()): ""
 f =       -4.530000000000000000000000000000e-2
 hex =       bffa.b98c7e28240b780346dc5d63885b
*string in : "-45.3e3"
 endptr                : ""
 endptr (from strtod()): ""
 f =       -4.530000000000000000000000000000e+4
 hex =       c00e.b0f4000000000000000000000000
*string in : "-45.3e+3"
 endptr                : ""
 endptr (from strtod()): ""
 f =       -4.530000000000000000000000000000e+4
 hex =       c00e.b0f4000000000000000000000000
*string in : "-45.3e-3"
 endptr                : ""
 endptr (from strtod()): ""
 f =       -4.530000000000000000000000000000e-2
 hex =       bffa.b98c7e28240b780346dc5d63885b
*string in : "1276.983e45"
 endptr                : ""
 endptr (from strtod()): ""
 f =       1.276983000000000000000000000000e+48
 hex =       409e.dfade5ec7d808f64f61c15f8aa3f
*string in : "1276.983e++45"
 endptr                : "e++45"
 endptr (from strtod()): "e++45"
 f =       1.276983000000000000000000000000e+3
 hex =       4009.9f9f74bc6a7ef9db22d0e5604189
*string in : "1276.983e-+45"
 endptr                : "e-+45"
 endptr (from strtod()): "e-+45"
 f =       1.276983000000000000000000000000e+3
 hex =       4009.9f9f74bc6a7ef9db22d0e5604189
*string in : ""
 endptr                : ""
 endptr (from strtod()): ""
 NAN
*string in : "."
 endptr                : "."
 endptr (from strtod()): "."
 NAN
*string in : ".e"
 endptr                : ".e"
 endptr (from strtod()): ".e"
 NAN
*string in : ".E"
 endptr                : ".E"
 endptr (from strtod()): ".E"
 NAN
*string in : ".e+"
 endptr                : ".e+"
 endptr (from strtod()): ".e+"
 NAN
*string in : ".E+"
 endptr                : ".E+"
 endptr (from strtod()): ".E+"
 NAN
*string in : ".e-"
 endptr                : ".e-"
 endptr (from strtod()): ".e-"
 NAN
*string in : ".E-"
 endptr                : ".E-"
 endptr (from strtod()): ".E-"
 NAN
*string in : "123,"
 endptr                : ","
 endptr (from strtod()): ","
 f =       1.230000000000000000000000000000e+2
 hex =       4005.f600000000000000000000000000
*string in : "65.67, 3456"
 endptr                : ", 3456"
 endptr (from strtod()): ", 3456"
 f =       6.567000000000000000000000000000e+1
 hex =       4005.83570a3d70a3d70a3d70a3d70a3d
*string in : "321dog"
 endptr                : "dog"
 endptr (from strtod()): "dog"
 f =       3.210000000000000000000000000000e+2
 hex =       4007.a080000000000000000000000000
*string in : "67.elm 34"
 endptr                : "elm 34"
 endptr (from strtod()): "elm 34"
 f =       6.700000000000000000000000000000e+1
 hex =       4005.8600000000000000000000000000
*string in : "67.dlm 34"
 endptr                : "dlm 34"
 endptr (from strtod()): "dlm 34"
 f =       6.700000000000000000000000000000e+1
 hex =       4005.8600000000000000000000000000
*string in : "65.988jump"
 endptr                : "jump"
 endptr (from strtod()): "jump"
 f =       6.598800000000000000000000000000e+1
 hex =       4005.83f9db22d0e5604189374bc6a7ef
*string in : "+321dog"
 endptr                : "dog"
 endptr (from strtod()): "dog"
 f =       3.210000000000000000000000000000e+2
 hex =       4007.a080000000000000000000000000
*string in : "+67.elm 34"
 endptr                : "elm 34"
 endptr (from strtod()): "elm 34"
 f =       6.700000000000000000000000000000e+1
 hex =       4005.8600000000000000000000000000
*string in : "-65.988jump"
 endptr                : "jump"
 endptr (from strtod()): "jump"
 f =       -6.598800000000000000000000000000e+1
 hex =       c005.83f9db22d0e5604189374bc6a7ef
*string in : "+dog12"
 endptr                : "+dog12"
 endptr (from strtod()): "+dog12"
 NAN
*string in : "-4df"
 endptr                : "df"
 endptr (from strtod()): "df"
 f =       -4.000000000000000000000000000000e+0
 hex =       c001.8000000000000000000000000000
*string in : "-cat"
 endptr                : "-cat"
 endptr (from strtod()): "-cat"
 NAN
*string in : "21.32e"
 endptr                : "e"
 endptr (from strtod()): "e"
 f =       2.132000000000000000000000000000e+1
 hex =       4003.aa8f5c28f5c28f5c28f5c28f5c28
*string in : "-21.32E"
 endptr                : "E"
 endptr (from strtod()): "E"
 f =       -2.132000000000000000000000000000e+1
 hex =       c003.aa8f5c28f5c28f5c28f5c28f5c28
*string in : "-21.32e+"
 endptr                : "e+"
 endptr (from strtod()): "e+"
 f =       -2.132000000000000000000000000000e+1
 hex =       c003.aa8f5c28f5c28f5c28f5c28f5c28
*string in : "+21.32E-"
 endptr                : "E-"
 endptr (from strtod()): "E-"
 f =       2.132000000000000000000000000000e+1
 hex =       4003.aa8f5c28f5c28f5c28f5c28f5c28
*string in : "34.56789overflows"
 endptr                : "overflows"
 endptr (from strtod()): "overflows"
 f =       3.456789000000000000000000000000e+1
 hex =       4004.8a4584f4c6e6d9be4cd74927913e
*string in : "123.45e+5000"
 endptr                : ""
 endptr (from strtod()): ""
 OVERFLOW (pos.)
*string in : "123.45e-5000"
 endptr                : ""
 endptr (from strtod()): ""
 f =       0.000000000000000000000000000000e+0
 hex =       0006.0000000000000000000000000000
*string in : "123.45e+10000"
 endptr                : ""
 endptr (from strtod()): ""
 OVERFLOW (pos.)
*string in : "123.45e-10000"
 endptr                : ""
 endptr (from strtod()): ""
 f =       0.000000000000000000000000000000e+0
 hex =       0006.0000000000000000000000000000
*string in : "-123.45E+10000"
 endptr                : ""
 endptr (from strtod()): ""
 OVERFLOW (neg.)
*string in : "-123.45E-10000"
 endptr                : ""
 endptr (from strtod()): ""
 f =       -0.000000000000000000000000000000e+0
 hex =       8006.0000000000000000000000000000
*string in : "+1.95e+4931"
 endptr                : ""
 endptr (from strtod()): ""
 f =       1.949999999999999999999999999989e+4931
 hex =       7ffc.a7d61079dca078d306f713ba3041
*string in : "-1.95e4931"
 endptr                : ""
 endptr (from strtod()): ""
 f =       -1.949999999999999999999999999989e+4931
 hex =       fffc.a7d61079dca078d306f713ba3041
*string in : "-1.03e-4931"
 endptr                : ""
 endptr (from strtod()): ""
 f =       -9.999999999999999999999999999942e-4932
 hex =       8002.be5b66ecbce0b7b0e32e439301f8
*string in : "+1.03e-4931"
 endptr                : ""
 endptr (from strtod()): ""
 f =       9.999999999999999999999999999942e-4932
 hex =       0002.be5b66ecbce0b7b0e32e439301f8
*string in : "+1.95e+4932"
 endptr                : ""
 endptr (from strtod()): ""
 OVERFLOW (pos.)
*string in : "-1.95e4932"
 endptr                : ""
 endptr (from strtod()): ""
 OVERFLOW (neg.)
*string in : "-1.03e-4932"
 endptr                : ""
 endptr (from strtod()): ""
 f =       0.000000000000000000000000000000e+0
 hex =       0000.0000000000000000000000000000
*string in : "+1.03e-4932"
 endptr                : ""
 endptr (from strtod()): ""
 f =       0.000000000000000000000000000000e+0
 hex =       0001.0000000000000000000000000000
*string in : "    12.456"
 endptr                : ""
 endptr (from strtod()): ""
 f =       1.245600000000000000000000000000e+1
 hex =       4002.c74bc6a7ef9db22d0e5604189374
*string in : "1.73ee"
 endptr                : "ee"
 endptr (from strtod()): "ee"
 f =       1.730000000000000000000000000000e+0
 hex =       3fff.dd70a3d70a3d70a3d70a3d70a3d3
*string in : "411.6.8"
 endptr                : ".8"
 endptr (from strtod()): ".8"
 f =       4.116000000000000000000000000000e+2
 hex =       4007.cdcccccccccccccccccccccccccc
*string in : "459021..0"
 endptr                : ".0"
 endptr (from strtod()): ".0"
 f =       4.590210000000000000000000000000e+5
 hex =       4011.e021a00000000000000000000000
*string in : "234e14+i"
 endptr                : "+i"
 endptr (from strtod()): "+i"
 f =       2.340000000000000000000000000000e+16
 hex =       4035.a6445a1f7d000000000000000000
*string in : "6530.876e+q34"
 endptr                : "e+q34"
 endptr (from strtod()): "e+q34"
 f =       6.530876000000000000000000000000e+3
 hex =       400b.cc17020c49ba5e353f7ced916872
*string in : "  	  +22.998e-4"
 endptr                : ""
 endptr (from strtod()): ""
 f =       2.299800000000000000000000000000e-3
 hex =       3ff6.96b83dc98f2aaace754378fb9453
*string in : "  	+e21"
 endptr                : "  	+e21"
 endptr (from strtod()): "  	+e21"
 NAN
*string in : "		-E-23"
 endptr                : "		-E-23"
 endptr (from strtod()): "		-E-23"
 NAN
*string in : "	  +.E15"
 endptr                : "	  +.E15"
 endptr (from strtod()): "	  +.E15"
 NAN
*string in : "   -.e-21"
 endptr                : "   -.e-21"
 endptr (from strtod()): "   -.e-21"
 NAN
*string in : "   .E34"
 endptr                : "   .E34"
 endptr (from strtod()): "   .E34"
 NAN
*string in : "        0e23"
 endptr                : ""
 endptr (from strtod()): ""
 f =       0.000000000000000000000000000000e+0
 hex =       004c.0000000000000000000000000000
*string in : "0E-23"
 endptr                : ""
 endptr (from strtod()): ""
 f =       0.000000000000000000000000000000e+0
 hex =       0000.0000000000000000000000000000
*string in : " .0e+23"
 endptr                : ""
 endptr (from strtod()): ""
 f =       0.000000000000000000000000000000e+0
 hex =       004c.0000000000000000000000000000
*string in : "     0e-34"
 endptr                : ""
 endptr (from strtod()): ""
 f =       0.000000000000000000000000000000e+0
 hex =       0000.0000000000000000000000000000
*string in : ".0E+15"
 endptr                : ""
 endptr (from strtod()): ""
 f =       0.000000000000000000000000000000e+0
 hex =       0031.0000000000000000000000000000
*string in : "    	d:34"
 endptr                : "    	d:34"
 endptr (from strtod()): "    	d:34"
 NAN
*string in : "   $+21"
 endptr                : "   $+21"
 endptr (from strtod()): "   $+21"
 NAN
*string in : "$+21"
 endptr                : "$+21"
 endptr (from strtod()): "$+21"
 NAN
*string in : "        -4$,"
 endptr                : "$,"
 endptr (from strtod()): "$,"
 f =       -4.000000000000000000000000000000e+0
 hex =       c001.8000000000000000000000000000
*string in : "   +12.807   :"
 endptr                : "   :"
 endptr (from strtod()): "   :"
 f =       1.280700000000000000000000000000e+1
 hex =       4002.cce978d4fdf3b645a1cac083126d
*string in : "      34.56e++"
 endptr                : "e++"
 endptr (from strtod()): "e++"
 f =       3.456000000000000000000000000000e+1
 hex =       4004.8a3d70a3d70a3d70a3d70a3d70a3
*string in : "     -45,67"
 endptr                : ",67"
 endptr (from strtod()): ",67"
 f =       -4.500000000000000000000000000000e+1
 hex =       c004.b400000000000000000000000000
*/
