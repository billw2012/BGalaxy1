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
#include </usr/include/complex.h>
#include "xcomplex.h"

using std::cout;
using std::cin;
using std::endl;
using namespace HPA;

typedef double complex dcomplex;

void println (dcomplex z)
{
  cout << "(" << creal(z) << ", " << cimag(z) << ")" << endl;
}

dcomplex ctrunc (dcomplex x)
{
  return trunc(creal(x)) + trunc(cimag(x)) * I;
}

#define cfix ctrunc

dcomplex cround (dcomplex x)
{
  return round(creal(x)) + round(cimag(x)) * I;
}

dcomplex cfrac (dcomplex z)
{
  return z - ctrunc (z);
}

dcomplex cceil (dcomplex x)
{
  return ceil(creal(x)) + ceil(cimag(x)) * I;
}

dcomplex cfloor (dcomplex x)
{
  return floor(creal(x)) + floor(cimag(x)) * I;
}

#define cconj conj

dcomplex cinv (dcomplex x)
{
  return 1.0/x;
}

dcomplex cswap (dcomplex x)
{
  return cimag(x) + creal(x) * I;
}

dcomplex cdrot (dcomplex x)
{
  return x*I;
}

dcomplex crrot (dcomplex x)
{
  return -1.0*x*I;
}

dcomplex csqr (dcomplex x)
{
  return x * x;
}

dcomplex cexp10 (dcomplex x)
{
  return cexp (log(10.0) * x);
}

dcomplex cexp2 (dcomplex x)
{
  return cexp (log(2.0) * x);
}

dcomplex clog2 (dcomplex z)
{
  return log2(cabs(z))+I*carg(z)/log(2);
}

dcomplex clog_sqrt (dcomplex x)
{
  return clog (csqrt(x));
}

xcomplex ABS (const xcomplex& z)
{
  return xcomplex(abs(z));
}

xcomplex ARG (const xcomplex& z)
{
  return xcomplex(arg(z));
}

dcomplex CABS (dcomplex z)
{
  return cabs(z) + 0.0*I;
}

dcomplex CARG (dcomplex z)
{
  return carg(z) + 0.0*I;
}

typedef dcomplex (*pm) (dcomplex x);

pm funlist2[] = {
  CABS,
  CARG,
  cfrac,
  ctrunc,
  cround,
  cceil,
  cfloor,
  cfix,
  cconj,
  cinv,
  cswap,
  cdrot,
  crrot,
  csqr,
  ctan,
  csin,
  ccos,
  catan,
  casin,
  cacos,
  csqrt,
  cexp,
  cexp2,
  cexp10,
  clog,
  clog2,
  clog10,
  clog_sqrt,
  ctanh,
  csinh,
  ccosh,
  catanh,
  casinh,
  cacosh,
  NULL
};

using namespace HPA;

typedef xcomplex (*pf) (const xcomplex& x);

const char* nlist[] = {
  "abs",
  "arg",
  "frac",
  "trunc",
  "round",
  "ceil",
  "floor",
  "fix",
  "conj",
  "inv",
  "swap",
  "drot",
  "rrot",
  "sqr",
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
  "log_sqrt",
  "tanh",
  "sinh",
  "cosh",
  "atanh",
  "asinh",
  "acosh",
  NULL
};

pf funlist1[] = {
  ABS,
  ARG,
  frac,
  trunc,
  round,
  ceil,
  floor,
  fix,
  conj,
  inv,
  swap,
  drot,
  rrot,
  sqr,
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
  log_sqrt,
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
  xcomplex w;
  dcomplex z;
  int i;

  xcomplex::set_fmt (XFMT_ALT);
  xcomplex::set_lim (5);
  cout << "Enter a complex number(Ctrl+D on an empty line to end up):" << endl;
  while (w.getfrom(cin) > 0)
    {
      for (i=0; nlist[i] != 0; i++)
	{
	  clear_xmatherr();
	  cout << nlist[i] << " (w) = " << (*funlist1[i])(w);
	  cout << " \t " << xmatherrcode() << endl;
	  z = w.dreal() + w.dimag() * I;
	  cout << nlist[i] << " (w) = ";
	  println((*funlist2[i])(z));
	}
      cout << "\n\t *** \t\n" << endl;
      cout << "Enter a complex number(Ctrl+D on an empty line to end up):" << endl;
    }
  return 0;
}
