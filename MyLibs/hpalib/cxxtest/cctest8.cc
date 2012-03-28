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
  /* Setting and Getting I/O flags */
  /* Friend operators just declared */
  short fmt, notat, sf, wd, lim;
  signed char ch, ldel, rdel;
  xcomplex X(xTwo, xEe), Y(xRndcorr, xTen), PI = xPi; 
  xcomplex z[6];
  xreal c;
  
  z[0] = xcomplex (0, 0);
  z[1] = xcomplex (1, 0);
  z[2] = xcomplex (3,-1);
  z[3] = xcomplex (2, 5);
  z[4] = xcomplex (-4.0, -0.5);
  z[5] = xcomplex (3, -1);
  cout << "[1] I/O flags manipulation\n";
  cout << "DEFAULT :\n";
  cout << "Format   =  " << xcomplex::get_fmt() << endl;
  cout << "Notation =  " << xcomplex::get_notation() << endl;
  cout << "Signflag =  " << xcomplex::get_signflag() << endl;
  cout << "Min_fwd  =  " << xcomplex::get_mfwd() << endl;
  cout << "Limit    =  " << xcomplex::get_lim() << endl;
  cout << "Padding  = \'" << xcomplex::get_padding() << "\'" << endl;
  cout << "Left del = \'" << xcomplex::get_ldelim() << "\'" << endl;
  cout << "Right del= \'" << xcomplex::get_rdelim() << "\'" << endl;
  cout << "Format   \?  "; cin >> fmt; xcomplex::set_fmt(fmt); 
  cout << "Notation \?  "; cin >> notat; xcomplex::set_notation (notat);
  cout << "Signflag \?  "; cin >> sf; xcomplex::set_signflag (sf);
  cout << "Min_fwd  \?  "; cin >> wd; xcomplex::set_mfwd (wd);
  cout << "Limit    \?  "; cin >> lim; xcomplex::set_lim (lim);
  cout << "Padding  \?  "; cin >> ch; xcomplex::set_padding(ch);
  cout << "Left del \?  "; cin >> ldel; xcomplex::set_ldelim(ldel); 
  cout << "Right del\?  "; cin >> rdel; xcomplex::set_rdelim(rdel); 
  cout << "NOW :\n";
  cout << "Format   =  " << xcomplex::get_fmt() << endl;
  cout << "Notation =  " << xcomplex::get_notation() << endl;
  cout << "Signflag =  " << xcomplex::get_signflag() << endl;
  cout << "Min_fwd  =  " << xcomplex::get_mfwd() << endl;
  cout << "Limit    =  " << xcomplex::get_lim() << endl;
  cout << "Padding  = \'" << xcomplex::get_padding() <<  "\'" << endl;
  cout << "Left del = \'" << xcomplex::get_ldelim() << "\'" << endl;
  cout << "Right del= \'" << xcomplex::get_rdelim() << "\'" << endl;
  cout << "[2] Binary operators\n";
  cout << "Output\n";
  cout << "X = " << X << ", Y = " << Y << ", PI = " << PI << endl;
  cout << "Input\n";
  cout << "Now enter a couple of double precision complex numbers, each of "
       << "them in the form of a couple of double precision numbers" << endl;
  cout << "X ? "; cin >> X;
  cout << "X = " << X << endl;
  cout << "Y ? "; cin >> Y;
  cout << "Y = " << Y << endl;
  cout << "Arithmetic\n";
  cout << " X   + Y   = " <<  X + Y << endl;
//   cout << " X   + 2   = " <<  X + 2 << endl;
//   cout << "-5   + Y   = " << -5 + Y << endl;
//   cout << " X   + 3   = " <<  X + 3U << endl;
//   cout << " 4   + Y   = " << 4U + Y << endl;
//   cout << " X   + 2   = " <<  X + 2L << endl;
//   cout << "-5   + Y   = " << -5L + Y << endl;
//   cout << " X   + 3   = " <<  X + 3UL << endl;
//   cout << " 4   + Y   = " << 4UL + Y << endl;
//   cout << " X   + 1.0 = " <<  X + 1.0f << endl;
//   cout << "-3.7 + Y   = " << -3.7f + Y << endl;
//   cout << " X   + 2.0 = " <<  X + 2.0 << endl;
//   cout << "-4.6 + Y   = " << -4.6 + Y << endl;
  cout << " X   - Y   = " <<  X - Y << endl;
//   cout << " X   - 2   = " <<  X - 2 << endl;
//   cout << "-5   - Y   = " << -5 - Y << endl;
//   cout << " X   - 3   = " <<  X - 3U << endl;
//   cout << " 4   - Y   = " << 4U - Y << endl;
//   cout << " X   - 2   = " <<  X - 2L << endl;
//   cout << "-5   - Y   = " << -5L - Y << endl;
//   cout << " X   - 3   = " <<  X - 3UL << endl;
//   cout << " 4   - Y   = " << 4UL - Y << endl;
//   cout << " X   - 1.0 = " <<  X - 1.0f << endl;
//   cout << "-3.7 - Y   = " << -3.7f - Y << endl;
//   cout << " X   - 2.0 = " <<  X - 2.0 << endl;
//   cout << "-4.6 - Y   = " << -4.6 - Y << endl;
  cout << " X   * Y   = " <<  X * Y << endl;
//   cout << " X   * 2   = " <<  X * 2 << endl;
//   cout << "-5   * Y   = " << -5 * Y << endl;
//   cout << " X   * 3   = " <<  X * 3U << endl;
//   cout << " 4   * Y   = " << 4U * Y << endl;
//   cout << " X   * 2   = " <<  X * 2L << endl;
//   cout << "-5   * Y   = " << -5L * Y << endl;
//   cout << " X   * 3   = " <<  X * 3UL << endl;
//   cout << " 4   * Y   = " << 4UL * Y << endl;
//   cout << " X   * 1.0 = " <<  X * 1.0f << endl;
//   cout << "-3.7 * Y   = " << -3.7f * Y << endl;
//   cout << " X   * 2.0 = " <<  X * 2.0 << endl;
//   cout << "-4.6 * Y   = " << -4.6 * Y << endl;
  cout << " X   / Y   = " <<  X / Y << endl;
//   cout << " X   / 2   = " <<  X / 2 << endl;
//   cout << "-5   / Y   = " << -5 / Y << endl;
//   cout << " X   / 3   = " <<  X / 3U << endl;
//   cout << " 4   / Y   = " << 4U / Y << endl;
//   cout << " X   / 2   = " <<  X / 2L << endl;
//   cout << "-5   / Y   = " << -5L / Y << endl;
//   cout << " X   / 3   = " <<  X / 3UL << endl;
//   cout << " 4   / Y   = " << 4UL / Y << endl;
//   cout << " X   / 1.0 = " <<  X / 1.0f << endl;
//   cout << "-3.7 / Y   = " << -3.7f / Y << endl;
//   cout << " X   / 2.0 = " <<  X / 2.0 << endl;
//   cout << "-4.6 / Y   = " << -4.6 / Y << endl;
  cout << "gdiv (X, Y) = " << gdiv(X, Y) << endl;
  cout << "gmod (X, Y) = " << gmod(X, Y) << endl;
  cout << "idiv (X, Y) = " << idiv(X, Y) << endl;
  cout << " mod (X, Y) = " << mod(X, Y) << endl;
  cout << "c (real number) ? ";
  cin >> c;
  cout << "rmul (c, X) = " << rmul (c, X) << endl;
  cout << "rmul (c, Y) = " << rmul (c, Y) << endl;
  cout << "Comparison\n";
  for (int i = 0; i < 6; i++)
    {
      for (int j = 0; j < 6; j++)
	{
	  cout << "z[" << i << "] == z[" << j << "] : " << (z[i] == z[j]) << endl;
	  cout << "z[" << i << "] != z[" << j << "] : " << (z[i] != z[j]) << endl;
          cout << "z[" << i << "] <= z[" << j << "] : " << (z[i] <= z[j]) << endl;
          cout << "z[" << i << "] >= z[" << j << "] : " << (z[i] >= z[j]) << endl;
          cout << "z[" << i << "] <  z[" << j << "] : " << (z[i] < z[j]) << endl;
          cout << "z[" << i << "] >  z[" << j << "] : " << (z[i] > z[j]) << endl;
	}
//       cout << "z[i] == 5 : " << (z[i] == 5) << endl;
//       cout << "z[i] != 5 : " << (z[i] != 5) << endl;
//       cout << "z[i] <= 5 : " << (z[i] <= 5) << endl;
//       cout << "z[i] >= 5 : " << (z[i] >= 5) << endl;
//       cout << "z[i] <  5 : " << (z[i] < 5) << endl;
//       cout << "z[i] >  5 : " << (z[i] > 5) << endl;
//       cout << "-7 == z[i] : " << (-7 == z[i]) << endl;
//       cout << "-7 != z[i] : " << (-7 != z[i]) << endl;
//       cout << "-7 <= z[i] : " << (-7 <= z[i]) << endl;
//       cout << "-7 >= z[i] : " << (-7 >= z[i]) << endl;
//       cout << "-7 <  z[i] : " << (-7 < z[i]) << endl;
//       cout << "-7 >  z[i] : " << (-7 > z[i]) << endl;
//       cout << "z[i] == 5 : " << (z[i] == 5U) << endl;
//       cout << "z[i] != 5 : " << (z[i] != 5U) << endl;
//       cout << "z[i] <= 5 : " << (z[i] <= 5U) << endl;
//       cout << "z[i] >= 5 : " << (z[i] >= 5U) << endl;
//       cout << "z[i] <  5 : " << (z[i] < 5U) << endl;
//       cout << "z[i] >  5 : " << (z[i] > 5U) << endl;
//       cout << "10 ==  z[i] : " << (10U == z[i]) << endl;
//       cout << "10 !=  z[i] : " << (10U != z[i]) << endl;
//       cout << "10 <=  z[i] : " << (10U <= z[i]) << endl;
//       cout << "10 >=  z[i] : " << (10U >= z[i]) << endl;
//       cout << "10 <   z[i] : " << (10U < z[i]) << endl;
//       cout << "10 >   z[i] : " << (10U > z[i]) << endl;
//       cout << "z[i] == -20 : " << (z[i] == -20L) << endl;
//       cout << "z[i] != -20 : " << (z[i] != -20L) << endl;
//       cout << "z[i] <= -20 : " << (z[i] <= -20L) << endl;
//       cout << "z[i] >= -20 : " << (z[i] >= -20L) << endl;
//       cout << "z[i] <  -20 : " << (z[i] < -20L) << endl;
//       cout << "z[i] >  -20 : " << (z[i] > -20L) << endl;
//       cout << "10 ==  z[i] : " << (10L == z[i]) << endl;
//       cout << "10 !=  z[i] : " << (10L != z[i]) << endl;
//       cout << "10 <=  z[i] : " << (10L <= z[i]) << endl;
//       cout << "10 >=  z[i] : " << (10L >= z[i]) << endl;
//       cout << "10 <   z[i] : " << (10L < z[i]) << endl;
//       cout << "10 >   z[i] : " << (10L > z[i]) << endl;
//       cout << "z[i] == 5 : " << (z[i] == 5UL) << endl;
//       cout << "z[i] != 5 : " << (z[i] != 5UL) << endl;
//       cout << "z[i] <= 5 : " << (z[i] <= 5UL) << endl;
//       cout << "z[i] >= 5 : " << (z[i] >= 5UL) << endl;
//       cout << "z[i] <  5 : " << (z[i] < 5UL) << endl;
//       cout << "z[i] >  5 : " << (z[i] > 5UL) << endl;
//       cout << "10 ==  z[i] : " << (10UL == z[i]) << endl;
//       cout << "10 !=  z[i] : " << (10UL != z[i]) << endl;
//       cout << "10 <=  z[i] : " << (10UL <= z[i]) << endl;
//       cout << "10 >=  z[i] : " << (10UL >= z[i]) << endl;
//       cout << "10 <   z[i] : " << (10UL < z[i]) << endl;
//       cout << "10 >   z[i] : " << (10UL > z[i]) << endl;
//       cout << "z[i] == 6.2 : " << (z[i] == 6.2f) << endl;
//       cout << "z[i] != 6.2 : " << (z[i] != 6.2f) << endl;
//       cout << "z[i] <= 6.2 : " << (z[i] <= 6.2f) << endl;
//       cout << "z[i] >= 6.2 : " << (z[i] >= 6.2f) << endl;
//       cout << "z[i] <  6.2 : " << (z[i] < 6.2f) << endl;
//       cout << "z[i] >  6.2 : " << (z[i] > 6.2f) << endl;
//       cout << "-11.6 == z[i] : " << (-11.6f == z[i]) << endl;
//       cout << "-11.6 != z[i] : " << (-11.6f != z[i]) << endl;
//       cout << "-11.6 <= z[i] : " << (-11.6f <= z[i]) << endl;
//       cout << "-11.6 >= z[i] : " << (-11.6f >= z[i]) << endl;
//       cout << "-11.6 <  z[i] : " << (-11.6f < z[i]) << endl;
//       cout << "-11.6 >  z[i] : " << (-11.6f > z[i]) << endl;
//       cout << "z[i] == 6.2 : " << (z[i] == 6.2) << endl;
//       cout << "z[i] != 6.2 : " << (z[i] != 6.2) << endl;
//       cout << "z[i] <= 6.2 : " << (z[i] <= 6.2) << endl;
//       cout << "z[i] >= 6.2 : " << (z[i] >= 6.2) << endl;
//       cout << "z[i] <  6.2 : " << (z[i] < 6.2) << endl;
//       cout << "z[i] >  6.2 : " << (z[i] > 6.2) << endl;
//       cout << "-11.6 == z[i] : " << (-11.6 == z[i]) << endl;
//       cout << "-11.6 != z[i] : " << (-11.6 != z[i]) << endl;
//       cout << "-11.6 <= z[i] : " << (-11.6 <= z[i]) << endl;
//       cout << "-11.6 >= z[i] : " << (-11.6 >= z[i]) << endl;
//       cout << "-11.6 <  z[i] : " << (-11.6 < z[i]) << endl;
//       cout << "-11.6 >  z[i] : " << (-11.6 > z[i]) << endl;
      cout << " * * * " << endl;
    }
  return 0;
}
