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

#include<iostream>
#include"xreal.h"

using namespace std;
using namespace HPA;

int main (void)
{
  char* endptr;
  xreal X = xOne, Y = xTwo, Z(&xTen), W(Y);
  const xreal x = -0.5123;
  xreal y = 2.0;
  const xreal l = 0.2432f;
  xreal m = -7.64f;
  const xreal w = 2; 
  xreal z = -1234;
  const xreal p = 2U;
  xreal q = 1400500600U, r = 3440550660U;
  const xreal b = "0.94";
  xreal c = xreal("-210.7654dfe", &endptr);
  const xreal d = string("+98761.765");
  xreal e = string("-876.045");
  xreal a[10];
  int sc_not = 0;
  int sign = 1;
  int lim = 8;

  cout << "Test of the constructors" << endl;
  X.print (cout, sc_not, sign, lim);
  cout << endl;
  Y.print (cout, sc_not, sign, lim);
  cout << endl;
  Z.print (cout, sc_not, sign, lim);
  cout << endl;
  W.print (cout, sc_not, sign, lim);
  cout << endl;
  x.print (cout, sc_not, sign, lim);
  cout << endl;
  y.print (cout, sc_not, sign, lim);
  cout << endl;
  l.print (cout, sc_not, sign, lim);
  cout << endl;
  m.print (cout, sc_not, sign, lim);
  cout << endl;
  w.print (cout, sc_not, sign, lim);
  cout << endl;
  z.print (cout, sc_not, sign, lim);
  cout << endl;
  p.print (cout, sc_not, sign, lim);
  cout << endl;
  q.print (cout, sc_not, sign, lim);
  cout << endl;
  r.print (cout, sc_not, sign, lim);
  cout << endl;
  b.print (cout, sc_not, sign, lim);
  cout << endl;
  c.print (cout, sc_not, sign, lim);
  cout << "  " << "\"" << endptr << "\"" << endl;
  d.print (cout, sc_not, sign, lim);
  cout << endl;
  e.print (cout, sc_not, sign, lim);
  cout << endl;
  for (int i = 0; i < 10; i++)
    {
      a[i].print (cout, sc_not, sign, lim);
      cout << endl;
    }
  cout << "Test of the assignment operator" << endl;
  cout << "Now we have: " << endl;
  cout << "X = ";
  X.print (cout, sc_not, sign, lim);
  cout << endl;
  cout << "Y = ";
  Y.print (cout, sc_not, sign, lim);
  cout << endl;
  cout << "Z = ";
  Z.print (cout, sc_not, sign, lim);
  cout << endl;
  Z = Y = X;
  cout << "After Z = Y = X we have: " << endl;
  cout << "X = ";
  X.print (cout, sc_not, sign, lim);
  cout << endl;
  cout << "Y = ";
  Y.print (cout, sc_not, sign, lim);
  cout << endl;
  cout << "Z = ";
  Z.print (cout, sc_not, sign, lim);
  cout << endl;
  cout << "Test of the conversion's functions" << endl;
  x.print (cout, sc_not, sign, lim);
  cout << " \t " << x._2double() << endl;
  y.print (cout, sc_not, sign, lim);
  cout << " \t " << y._2double() << endl;
  l.print (cout, sc_not, sign, lim);
  cout << " \t " << l._2float() << endl;
  m.print (cout, sc_not, sign, lim);
  cout << " \t " << m._2float() << endl;
//   /* * */
//   w.print (cout, sc_not, sign, lim);
//   cout << " \t " << int(w) << " \t ";
//   xprint (stdout, struct xpr(w)); 
//   z.print (cout, sc_not, sign, lim);
//   cout << " \t " << long(z) << " \t ";
//   xprint (stdout, struct xpr(z));
//   /* * */
//   p.print (cout, sc_not, sign, lim);
//   cout << " \t " << (unsigned int)p << " \t ";
//   xprint (stdout, struct xpr(p));
//   q.print (cout, sc_not, sign, lim);
//   cout << " \t " << (unsigned long)q << endl;
//   r.print (cout, sc_not, sign, lim);
//   cout << " \t " << (unsigned long)r << endl;
  b.print (cout, sc_not, sign, lim);
  cout << " \t " << b._2string() << endl;
  c.print (cout, sc_not, sign, lim);
  cout << " \t " << c._2string() << endl;
  return 0;
}
