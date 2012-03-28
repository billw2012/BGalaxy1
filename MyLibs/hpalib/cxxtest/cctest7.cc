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
#include"xcomplex.h"

using namespace std;
using namespace HPA;

int main (void)
{
  char *endptr, *tail;
  xcomplex X = cxOne, Y = cxZero, Z(&cxIU), W(Y);
  xcomplex A(xTwo, xTen);
  xreal _re = +0.24;
  xreal _im = -0.87;
  const xcomplex B = _re;
  const xcomplex BB(_re, _im);

  const xcomplex x = -0.5123;
  xcomplex y = 2.0;
  const xcomplex xx(-0.5123, 1.2);
  xcomplex yy(12.56, -4.338);

  const xcomplex l = 0.2432f;
  xcomplex m = -7.64f;
  const xcomplex ll(0.2432f, 1.12f);
  xcomplex mm(-7.64f, 5.43f);

  const xcomplex w = 2; 
  xcomplex z = -1234;
  const xcomplex ww(2, -56); 
  xcomplex zz(-1234, 4312);

  const xcomplex p = 2U;
  xcomplex q = 1400500600U, r = 3440550660U;
  const xcomplex pp= xcomplex(2U, 5U);
  xcomplex qq(1400500600U, 456U), rr = xcomplex(3440550660U, 4007560010U);

  const xcomplex b = "+0.94+8.76i";
  xcomplex c = xcomplex("-210.7654+5.4idfe", &endptr);
  const xcomplex d = string("+98761.765-78.5i");
  xcomplex e = string("-876.045");
  xcomplex a[10];
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
  A.print (cout, sc_not, sign, lim);
  cout << endl;
  B.print (cout, sc_not, sign, lim);
  cout << endl;
  BB.print (cout, sc_not, sign, lim);
  cout << endl;

  x.print (cout, sc_not, sign, lim);
  cout << endl;
  y.print (cout, sc_not, sign, lim);
  cout << endl;

  xx.print (cout, sc_not, sign, lim);
  cout << endl;
  yy.print (cout, sc_not, sign, lim);
  cout << endl;

  l.print (cout, sc_not, sign, lim);
  cout << endl;
  m.print (cout, sc_not, sign, lim);
  cout << endl;

  ll.print (cout, sc_not, sign, lim);
  cout << endl;
  mm.print (cout, sc_not, sign, lim);
  cout << endl;

  w.print (cout, sc_not, sign, lim);
  cout << endl;
  z.print (cout, sc_not, sign, lim);
  cout << endl;

  ww.print (cout, sc_not, sign, lim);
  cout << endl;
  zz.print (cout, sc_not, sign, lim);
  cout << endl;

  p.print (cout, sc_not, sign, lim);
  cout << endl;
  q.print (cout, sc_not, sign, lim);
  cout << endl;
  r.print (cout, sc_not, sign, lim);
  cout << endl;

  pp.print (cout, sc_not, sign, lim);
  cout << endl;
  qq.print (cout, sc_not, sign, lim);
  cout << endl;
  rr.print (cout, sc_not, sign, lim);
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

  double_complex _x_ = x._2dcomplex();
  x.print (cout, sc_not, sign, lim);
  cout << " \t (" << _x_.re << ", " << _x_.im << ")" << endl;

  double_complex _y_ = y._2dcomplex();
  y.print (cout, sc_not, sign, lim);
  cout << " \t (" << _y_.re << ", " << _y_.im << ")" << endl;

  float_complex _l_ = l._2fcomplex();
  l.print (cout, sc_not, sign, lim);
  cout << " \t (" << _l_.re << ", " << _l_.im << ")" << endl;

  float_complex _m_ = m._2fcomplex();
  m.print (cout, sc_not, sign, lim);
  cout << " \t (" << _m_.re << ", " << _m_.im << ")" << endl;

  b.print (cout, sc_not, sign, lim);
  cout << " \t " << b._2string() << endl;
  c.print (cout, sc_not, sign, lim);
  cout << " \t " << c._2string() << endl;
  
  cout << "Test of real() and imag()" <<endl;
  xx.print (cout, sc_not, sign, lim);
  cout << "\nre = " << xx.real() << endl;
  cout << "im = " << xx.imag() << endl;
  cout << "re = "; xprxpr (xx._real(), lim); cout << endl;
  cout << "im = "; xprxpr (xx._imag(), lim); cout << endl;
  cout << "re = " << xx.dreal() << endl;
  cout << "im = " << xx.dimag() << endl;
  cout << "re = " << xx.freal() << endl;
  cout << "im = " << xx.fimag() << endl;
  cout << "re = " << xx.sreal() << endl;
  cout << "im = " << xx.simag() << endl;
  yy.print (cout, sc_not, sign, lim);
  cout << "\nre = " << yy.real() << endl;
  cout << "im = " << yy.imag() << endl;
  cout << "re = "; xprxpr (yy._real(), lim); cout << endl;
  cout << "im = "; xprxpr (yy._imag(), lim); cout << endl;
  cout << "re = " << yy.dreal() << endl;
  cout << "im = " << yy.dimag() << endl;
  cout << "re = " << yy.freal() << endl;
  cout << "im = " << yy.fimag() << endl;
  cout << "re = " << yy.sreal() << endl;
  cout << "im = " << yy.simag() << endl;

  ll.print (cout, sc_not, sign, lim);
  cout << "\nre = " << ll.real() << endl;
  cout << "im = " << ll.imag() << endl;
  cout << "re = "; xprxpr (ll._real(), lim); cout << endl;
  cout << "im = "; xprxpr (ll._imag(), lim); cout << endl;
  cout << "re = " << ll.dreal() << endl;
  cout << "im = " << ll.dimag() << endl;
  cout << "re = " << ll.freal() << endl;
  cout << "im = " << ll.fimag() << endl;
  cout << "re = " << ll.sreal() << endl;
  cout << "im = " << ll.simag() << endl;
  mm.print (cout, sc_not, sign, lim);
  cout << "\nre = " << mm.real() << endl;
  cout << "im = " << mm.imag() << endl;
  cout << "re = "; xprxpr (mm._real(), lim); cout << endl;
  cout << "im = "; xprxpr (mm._imag(), lim); cout << endl;
  cout << "re = " << mm.dreal() << endl;
  cout << "im = " << mm.dimag() << endl;
  cout << "re = " << mm.freal() << endl;
  cout << "im = " << mm.fimag() << endl;
  cout << "re = " << mm.sreal() << endl;
  cout << "im = " << mm.simag() << endl;

  ww.print (cout, sc_not, sign, lim);
  cout << "\nre = " << ww.real() << endl;
  cout << "im = " << ww.imag() << endl;
  cout << "re = "; xprxpr (ww._real(), lim); cout << endl;
  cout << "im = "; xprxpr (ww._imag(), lim); cout << endl;
  cout << "re = " << ww.dreal() << endl;
  cout << "im = " << ww.dimag() << endl;
  cout << "re = " << ww.freal() << endl;
  cout << "im = " << ww.fimag() << endl;
  cout << "re = " << ww.sreal() << endl;
  cout << "im = " << ww.simag() << endl;
  zz.print (cout, sc_not, sign, lim);
  cout << "\nre = " << zz.real() << endl;
  cout << "im = " << zz.imag() << endl;
  cout << "re = "; xprxpr (zz._real(), lim); cout << endl;
  cout << "im = "; xprxpr (zz._imag(), lim); cout << endl;
  cout << "re = " << zz.dreal() << endl;
  cout << "im = " << zz.dimag() << endl;
  cout << "re = " << zz.freal() << endl;
  cout << "im = " << zz.fimag() << endl;
  cout << "re = " << zz.sreal() << endl;
  cout << "im = " << zz.simag() << endl;

  qq.print (cout, sc_not, sign, lim);
  cout << "\nre = " << qq.real() << endl;
  cout << "im = " << qq.imag() << endl;
  cout << "re = "; xprxpr (qq._real(), lim); cout << endl;
  cout << "im = "; xprxpr (qq._imag(), lim); cout << endl;
  cout << "re = " << qq.dreal() << endl;
  cout << "im = " << qq.dimag() << endl;
  cout << "re = " << qq.freal() << endl;
  cout << "im = " << qq.fimag() << endl;
  cout << "re = " << qq.sreal() << endl;
  cout << "im = " << qq.simag() << endl;
  rr.print (cout, sc_not, sign, lim);
  cout << "\nre = " << rr.real() << endl;
  cout << "im = " << rr.imag() << endl;
  cout << "re = "; xprxpr (rr._real(), lim); cout << endl;
  cout << "im = "; xprxpr (rr._imag(), lim); cout << endl;
  cout << "re = " << rr.dreal() << endl;
  cout << "im = " << rr.dimag() << endl;
  cout << "re = " << rr.freal() << endl;
  cout << "im = " << rr.fimag() << endl;
  cout << "re = " << rr.sreal() << endl;
  cout << "im = " << rr.simag() << endl;
  
  xcomplex::set_lim (14);
  cout << "Test of real(TYPE), imag(TYPE)" << endl;
  cout << "Now A = " << A << endl;
  cout << "_re   = " << _re << endl;
  cout << "_im   = " << _im << endl;
  A.real(_re), A.imag(_im);
  cout << "After A.real(_re) and A.imag(_im), A = " << A << endl;
  A.real(xOne), A.imag(xOne);
  cout << "After A.real(xOne) and A.imag(xOne), A = " << A << endl;
  A.real(&xPi), A.imag(&xPi);
  cout << "After A.real(&xPi) and A.imag(&xPi), A = " << A << endl;
  A.real(-42.3000000000002), A.imag(31.6900000000007);
  cout << "After A.real(-42.3000000000002) and A.imag(31.6900000000007), A = " << A << endl;
  A.real(-42.453f), A.imag(31.884f);
  cout << "After A.real(-42.453f) and A.imag(31.884f), A = " << A << endl;
  A.real(13), A.imag(-57);
  cout << "After A.real(13) and A.imag(-57), A = " << A << endl;
  A.real(1234567890L), A.imag(-1234567890L);
  cout << "After A.real(1234567890L) and A.imag(-1234567890L), A = " << A << endl;
  A.real(136U), A.imag(597U);
  cout << "After A.real(136U) and A.imag(597U), A = " << A << endl;
  A.real(4200291778U), A.imag(3900678002U);
  cout << "After A.real(4200291778) and A.imag(3900678002), A = " << A << endl;
  A.real("+32dog", &endptr), A.imag("-56.2e+bag", &tail);
  cout << "After A.real(\"+32dog\", &endptr) and A.imag(\"-56.2e+bag\", &tail), A = "
       << A << endl;
  cout << "endptr = \"" << endptr << "\"\n";
  cout << "tail   = \"" << tail << "\"\n";
  A.real(string("-68.43ff")), A.imag(string(".98201-dx"));
  cout << "After A.real(string(\"-68.43ff\")) and A.imag(string(\".98201-dx\")), A = "
       << A << endl;
//   B.real(_re), B.imag(_im);
//   B.real(xOne), B.imag(xOne);
//   B.real(&xPi), B.imag(&xPi);
//   B.real(-42.3000000000002), B.imag(31.6900000000007);
//   B.real(-42.453f), B.imag(31.884f);
//   B.real(13), B.imag(-57);
//   B.real(1234567890L), B.imag(-1234567890L);
//   B.real(136U), B.imag(597U);
//   B.real(4200291778U), B.imag(3900678002U);
//   B.real("+32dog", &endptr), B.imag("-56.2e+bag", &tail);
//   B.real(string("-68.43ff")), B.imag(string(".98201-dx"));
  return 0;
}
