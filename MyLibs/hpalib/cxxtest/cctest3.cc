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

#include <iostream>
#include <cmath>
#include "xreal.h"

using namespace std;

double Frac (double x)
{
  return x - trunc (x);
}

#define Fix trunc

typedef double (*pm) (double x);

pm funlist2[] = {
  abs,
  Frac,
  trunc,
  round,
  ceil,
  floor,
  Fix,
  tan,
  sin,
  cos,
  atan,
  asin,
  acos,
  sqrt,
  exp,
  exp2,
  exp10,
  log,
  log2,
  log10,
  tanh,
  sinh,
  cosh,
  atanh,
  asinh,
  acosh,
  NULL
};

using namespace HPA;

typedef xreal (*pf) (const xreal& x);

const char* nlist[] = {
  "abs",
  "frac",
  "trunc",
  "round",
  "ceil",
  "floor",
  "fix",
  "tan",
  "sin",
  "cos",
  "atan",
  "asin",
  "acos",
  "sqrt",
  "exp",
  "exp2",
  "exp10",
  "log",
  "log2",
  "log10",
  "tanh",
  "sinh",
  "cosh",
  "atanh",
  "asinh",
  "acosh",
  NULL
};

pf funlist1[] = {
  abs,
  frac,
  trunc,
  round,
  ceil,
  floor,
  fix,
  tan,
  sin,
  cos,
  atan,
  asin,
  acos,
  sqrt,
  exp,
  exp2,
  exp10,
  log,
  log2,
  log10,
  tanh,
  sinh,
  cosh,
  atanh,
  asinh,
  acosh,
  NULL
};

int main (void)
{
  xreal x;
  double y;
  int i;

  cout << "Enter a real number "
       << "(Ctrl+D on an empty line to terminate):" << endl;
  while (x.getfrom(cin) > 0)
    {
      for (i=0; nlist[i] != 0; i++)
	{
	  clear_xmatherr();
	  cout << nlist[i] << " (x) = " << (*funlist1[i])(x);
	  cout << " \t " << xmatherrcode() << endl;
	  y = x._2double();
	  cout << nlist[i] << " (x) = " << (*funlist2[i])(y) << endl;
	}
      cout << "\n\t *** \t\n" << endl;
      cout << "Enter a real number "
	   << "(Ctrl+D on an empty line to terminate):" << endl;
    }
  return 0;
}
