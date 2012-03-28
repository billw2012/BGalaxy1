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
#include <cstdlib>
#include "xreal.h"

using namespace std;
using namespace HPA;

int main (void)
{
  char buffer[256];
  char *endptr, *tail;
  xreal x, y, q, r;
  int n, p;

  cout << "Enter, on a unique line, a couple of real numbers "
       << "and an integer value(Ctrl+D on an empty line to terminate):" << endl;
  while ((cin.getline(buffer, 256)))
    {
      x = xreal (buffer, &endptr);
      y = xreal (endptr, &tail);
      n = atoi (tail);
      if (!x)
	cout << "@@@ x is zero" << endl;
      else
	cout << "@@@ x is NOT zero" << endl;
      cout << "x = " << x << endl;
      cout << "y = " << y << endl;
      cout << "n = " << n << endl;
      cout << "compare (x, y) = " << compare (x, y) << endl;
      cout << "pow(x, y) = " << pow (x, y) << endl;
      cout << "x^n     = " << x(n) << endl;
      cout << "x * 2^n = " << x % n << endl;
      cout << "y^n = " << y(n) << endl;
      cout << "y * 2^n = " << y % n << endl;
      cout << "frexp (x, &p) = " << frexp (x, &p) << endl;
      cout << "p = " << p << endl;
      cout << "sfmod (x, &p) = " << sfmod (x, &p) << endl;
      cout << "p = " << p << endl;
      cout << " fmod (x, y)  = " << fmod (x, y) << endl;
      cout << " qfmod (x, y, q)  = " << qfmod (x, y, q) << endl;
      cout << "q = " << q << endl;
      q = xTen; x = xTwo; r = xZero;
      cout << "Now q = " << q << ", x = " << x << endl;
      cout << "After r += q += x we have r = " << (r += q += x) << endl;
      cout << "After r -= q -= x we have r = " << (r -= q -= x) << endl;
      cout << "After r *= q *= x we have r = " << (r *= q *= x) << endl;
      cout << "After r /= q /= x we have r = " << (r /= q /= x) << endl;
      cout << "After r /= q %= -2 we have r = " << (r /= q %= -2) << endl;
      cout << "After r /= q %= +2 we have r = " << (r /= q %= +2) << endl;
      cout << "x++  " << x++ << ", " << x << endl;
      cout << "++x  " << ++x << ", " << x << endl;
      cout << "x--  " << x-- << ", " << x << endl;
      cout << "--x  " << --x << ", " << x << endl;
      cout << "\n\t *** \t\n" << endl;
      cout << "Enter, on a unique line, a couple of real numbers "
	   << "and an integer value(Ctrl+D on an empty line to terminate):" << endl;
    }
  return 0;
}
