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
#include <sstream>
#include <cstdlib>
#include "xreal.h"

using namespace std;
using namespace HPA;

const char* nl[18] = {
  "xZERO", "xONE", "xTWO", "xTEN",
  "xINF", "x_INF", "xNAN",
  "xPI", "xPI2", "xPI4", "xEE", "xSQRT2",
  "xLN2", "xLN10", "xLOG2_E", "xLOG2_10", "xLOG10_E",
  NULL
};

int main (void)
{
  char buffer[256];
  stringstream sstr;
  xreal x, y, q, r, a[17];
  const xreal w = -xONE;
  int i;

  cout << "Enter a couple of real numbers on the same line "
       << "(Ctrl+D on an empty line to end up):" << endl;
  while ((cin.getline(buffer, 256)))
    {
      sstr.str(buffer);
      sstr >> x >> y;
      cout << "x = " << x << endl;
      cout << "y = " << y << endl;
      cout << "x.isneg() " << x.isneg() << endl;
      cout << "y.isneg() " << y.isneg() << endl;
      cout << "x.exp() " << x.exp() << endl;
      cout << "y.exp() " << y.exp() << endl;
      cout << "-x = " << -x << ", -y = " << -y << endl;
      if (!x)
	cout << "@@@ x is zero" << endl;
      else
	cout << "@@@ x is NOT zero" << endl;
      if (!y)
	cout << "@@@ y is zero" << endl;
      else
	cout << "@@@ y is NOT zero" << endl;
      cout << "\n\t *** \t\n" << endl;
      cout << "Enter a couple of real numbers on the same line "
	   << "(Ctrl+D on an empty line to end up):" << endl;
    }
  cout << "xPI = " << xPI << endl;
  cout << "w = " << w << endl;
  cout << "xPI.isneg() " << xPI.isneg() << endl;
  cout << "w.isneg() " << w.isneg() << endl;
  cout << "xPI.exp() " << xPI.exp() << endl;
  cout << "w.exp() " << w.exp() << endl;
  cout << "-xPI = " << -xPI << ", -w = " << -w << endl;
  if (!xPI)
    cout << "@@@ xPI is zero" << endl;
  else
    cout << "@@@ xPI is NOT zero" << endl;
  if (!w)
    cout << "@@@ w is zero" << endl;
  else
    cout << "@@@ w is NOT zero" << endl;
  a[0] = xZERO;
  a[1] = xONE;
  a[2] = xTWO;
  a[3] = xTEN;
  a[4] = xINF;
  a[5] = x_INF;
  a[6] = xNAN;
  a[7] = xPI; 
  a[8] = xPI2; 
  a[9] = xPI4; 
  a[10] = xEE; 
  a[11] = xSQRT2;
  a[12] = xLN2; 
  a[13] = xLN10; 
  a[14] = xLOG2_E; 
  a[15] = xLOG2_10; 
  a[16] = xLOG10_E;
  for (i = 0; i < 17; i++)
    cout << nl[i] << " \t\t " << a[i] << endl; 
  return 0;
}
