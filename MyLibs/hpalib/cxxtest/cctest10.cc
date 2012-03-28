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
#include "xcomplex.h"

using namespace std;
using namespace HPA;

const char* nl[4] = {
  "cxZERO", "cxONE", "cxI",
  NULL
};

int main (void)
{
//   char* buffer;
  xcomplex x, a[3];
  const xcomplex w = -cxONE;
  int i, n; 
//   int sc_not, sign, lim;

//   cout << "sc_not ? (0 = fixed, 1 = scientific) "; cin >> sc_not;
//   cout << "sign flag ? (0 = NO, 1 = YES) "; cin >> sign;
//   cout << "lim ? "; cin >> lim;
//   cout << "w = " << w << endl;
//   w.print (cout, sc_not, sign, lim);
//   cout << endl;
//   buffer = w.asprint (sc_not, sign, lim);
//   if ((buffer))
//     {
//       cout << buffer << endl;
//       free ((void*)buffer);
//     }
//   cout << "Test of xcomplex::getfrom(), xcomplex::print() and xcomplex::asprint()" << endl;
//   while ( x.getfrom (cin) > 0 )
//     {
//       x.print (cout, sc_not, sign, lim);
//       cout << endl;
//       buffer = x.asprint (sc_not, sign, lim);
//       if ((buffer))
// 	{
// 	  cout << buffer << endl;
// 	  free ((void*)buffer);
// 	}
//     }
  cout << "Test of xcomplex::root()" << endl;
  cout << "x ? (Must be a complex number, Ctrl+D on an empty line "
       << "to terminate)" << endl;
  while ( x.getfrom (cin) > 0 )
    {
      cout << "branch ? "; cin >> i;
      cout << "     n ? "; cin >> n;
      cout << "  root = " << root (x, i, n) << endl;
      cout << "x ? (Must be a complex number, Ctrl+D on an empty line "
	   << "to terminate)" << endl;
    }
  cout << "cxI = " << cxI << endl;
  cout << "w = " << w << endl;
  cout << "-cxI = " << -cxI << ", -w = " << -w << endl;
  if (!cxI)
    cout << "@@@ cxI is zero" << endl;
  else
    cout << "@@@ cxI is NOT zero" << endl;
  if (!w)
    cout << "@@@ w is zero" << endl;
  else
    cout << "@@@ w is NOT zero" << endl;
  a[0] = cxZERO;
  a[1] = cxONE;
  a[2] = cxI;
  for (i = 0; i < 3; i++)
    cout << nl[i] << " \t\t " << a[i] << endl; 
  return 0;
}
