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
  /* Setting and Getting I/O flags */
  /* Friend operators just declared */
  short notat, sf, wd, lim;
  signed char ch;
  xreal X = xTwo, Y = xRndcorr, PI = xPi; 
  xreal a[5];
  
  for (int i = 0; i < 5; i++)
    a[i] = inttox (10*(i-2));
  cout << "[1] I/O flags manipulation\n";
  cout << "DEFAULT :\n";
  cout << "Notation =  " << xreal::get_notation() << endl;
  cout << "Signflag =  " << xreal::get_signflag() << endl;
  cout << "Min_fwd  =  " << xreal::get_mfwd() << endl;
  cout << "Limit    =  " << xreal::get_lim() << endl;
  cout << "Padding  = \'" << xreal::get_padding() << "\'" << endl;
  cout << "Notation \?  "; cin >> notat; xreal::set_notation (notat);
  cout << "Signflag \?  "; cin >> sf; xreal::set_signflag (sf);
  cout << "Min_fwd  \?  "; cin >> wd; xreal::set_mfwd (wd);
  cout << "Limit    \?  "; cin >> lim; xreal::set_lim (lim);
  cout << "Padding  \?  "; cin >> ch; xreal::set_padding(ch);
  cout << "NOW :\n";
  cout << "Notation =  " << xreal::get_notation() << endl;
  cout << "Signflag =  " << xreal::get_signflag() << endl;
  cout << "Min_fwd  =  " << xreal::get_mfwd() << endl;
  cout << "Limit    =  " << xreal::get_lim() << endl;
  cout << "Padding  = \'" << xreal::get_padding() <<  "\'" << endl;

  cout << "[2] Binary operators\n";
  cout << "Output\n";
  cout << "X = " << X << ", Y = " << Y << ", PI = " << PI << endl;
  cout << "Input\n";
  cout << "Now enter a couple of double precision numbers" << endl;
  cout << "X ? "; cin >> X;
  cout << "X = " << X << endl;
  cout << "Y ? "; cin >> Y;
  cout << "Y = " << Y << endl;
  cout << "Arithmetic\n";
  cout << " X   + Y   = " <<  X + Y << endl;
  cout << " X   + 2   = " <<  X + 2 << endl;
  cout << "-5   + Y   = " << -5 + Y << endl;
  cout << " X   + 3   = " <<  X + 3U << endl;
  cout << " 4   + Y   = " << 4U + Y << endl;
  cout << " X   + 2   = " <<  X + 2L << endl;
  cout << "-5   + Y   = " << -5L + Y << endl;
  cout << " X   + 3   = " <<  X + 3UL << endl;
  cout << " 4   + Y   = " << 4UL + Y << endl;
  cout << " X   + 1.0 = " <<  X + 1.0f << endl;
  cout << "-3.7 + Y   = " << -3.7f + Y << endl;
  cout << " X   + 2.0 = " <<  X + 2.0 << endl;
  cout << "-4.6 + Y   = " << -4.6 + Y << endl;
  cout << " X   - Y   = " <<  X - Y << endl;
  cout << " X   - 2   = " <<  X - 2 << endl;
  cout << "-5   - Y   = " << -5 - Y << endl;
  cout << " X   - 3   = " <<  X - 3U << endl;
  cout << " 4   - Y   = " << 4U - Y << endl;
  cout << " X   - 2   = " <<  X - 2L << endl;
  cout << "-5   - Y   = " << -5L - Y << endl;
  cout << " X   - 3   = " <<  X - 3UL << endl;
  cout << " 4   - Y   = " << 4UL - Y << endl;
  cout << " X   - 1.0 = " <<  X - 1.0f << endl;
  cout << "-3.7 - Y   = " << -3.7f - Y << endl;
  cout << " X   - 2.0 = " <<  X - 2.0 << endl;
  cout << "-4.6 - Y   = " << -4.6 - Y << endl;
  cout << " X   * Y   = " <<  X * Y << endl;
  cout << " X   * 2   = " <<  X * 2 << endl;
  cout << "-5   * Y   = " << -5 * Y << endl;
  cout << " X   * 3   = " <<  X * 3U << endl;
  cout << " 4   * Y   = " << 4U * Y << endl;
  cout << " X   * 2   = " <<  X * 2L << endl;
  cout << "-5   * Y   = " << -5L * Y << endl;
  cout << " X   * 3   = " <<  X * 3UL << endl;
  cout << " 4   * Y   = " << 4UL * Y << endl;
  cout << " X   * 1.0 = " <<  X * 1.0f << endl;
  cout << "-3.7 * Y   = " << -3.7f * Y << endl;
  cout << " X   * 2.0 = " <<  X * 2.0 << endl;
  cout << "-4.6 * Y   = " << -4.6 * Y << endl;
  cout << " X   / Y   = " <<  X / Y << endl;
  cout << " X   / 2   = " <<  X / 2 << endl;
  cout << "-5   / Y   = " << -5 / Y << endl;
  cout << " X   / 3   = " <<  X / 3U << endl;
  cout << " 4   / Y   = " << 4U / Y << endl;
  cout << " X   / 2   = " <<  X / 2L << endl;
  cout << "-5   / Y   = " << -5L / Y << endl;
  cout << " X   / 3   = " <<  X / 3UL << endl;
  cout << " 4   / Y   = " << 4UL / Y << endl;
  cout << " X   / 1.0 = " <<  X / 1.0f << endl;
  cout << "-3.7 / Y   = " << -3.7f / Y << endl;
  cout << " X   / 2.0 = " <<  X / 2.0 << endl;
  cout << "-4.6 / Y   = " << -4.6 / Y << endl;
  cout << "Comparison\n";
  for (int i = 0; i < 5; i++)
    {
      for (int j = 0; j < 5; j++)
	{
	  cout << "a[i] == a[j] : " << (a[i] == a[j]) << endl;
	  cout << "a[i] != a[j] : " << (a[i] != a[j]) << endl;
          cout << "a[i] <= a[j] : " << (a[i] <= a[j]) << endl;
          cout << "a[i] >= a[j] : " << (a[i] >= a[j]) << endl;
          cout << "a[i] <  a[j] : " << (a[i] < a[j]) << endl;
          cout << "a[i] >  a[j] : " << (a[i] > a[j]) << endl;
	}
      cout << "a[i] == 5 : " << (a[i] == 5) << endl;
      cout << "a[i] != 5 : " << (a[i] != 5) << endl;
      cout << "a[i] <= 5 : " << (a[i] <= 5) << endl;
      cout << "a[i] >= 5 : " << (a[i] >= 5) << endl;
      cout << "a[i] <  5 : " << (a[i] < 5) << endl;
      cout << "a[i] >  5 : " << (a[i] > 5) << endl;
      cout << "-7 == a[i] : " << (-7 == a[i]) << endl;
      cout << "-7 != a[i] : " << (-7 != a[i]) << endl;
      cout << "-7 <= a[i] : " << (-7 <= a[i]) << endl;
      cout << "-7 >= a[i] : " << (-7 >= a[i]) << endl;
      cout << "-7 <  a[i] : " << (-7 < a[i]) << endl;
      cout << "-7 >  a[i] : " << (-7 > a[i]) << endl;
      cout << "a[i] == 5 : " << (a[i] == 5U) << endl;
      cout << "a[i] != 5 : " << (a[i] != 5U) << endl;
      cout << "a[i] <= 5 : " << (a[i] <= 5U) << endl;
      cout << "a[i] >= 5 : " << (a[i] >= 5U) << endl;
      cout << "a[i] <  5 : " << (a[i] < 5U) << endl;
      cout << "a[i] >  5 : " << (a[i] > 5U) << endl;
      cout << "10 ==  a[i] : " << (10U == a[i]) << endl;
      cout << "10 !=  a[i] : " << (10U != a[i]) << endl;
      cout << "10 <=  a[i] : " << (10U <= a[i]) << endl;
      cout << "10 >=  a[i] : " << (10U >= a[i]) << endl;
      cout << "10 <   a[i] : " << (10U < a[i]) << endl;
      cout << "10 >   a[i] : " << (10U > a[i]) << endl;
      cout << "a[i] == -20 : " << (a[i] == -20L) << endl;
      cout << "a[i] != -20 : " << (a[i] != -20L) << endl;
      cout << "a[i] <= -20 : " << (a[i] <= -20L) << endl;
      cout << "a[i] >= -20 : " << (a[i] >= -20L) << endl;
      cout << "a[i] <  -20 : " << (a[i] < -20L) << endl;
      cout << "a[i] >  -20 : " << (a[i] > -20L) << endl;
      cout << "10 ==  a[i] : " << (10L == a[i]) << endl;
      cout << "10 !=  a[i] : " << (10L != a[i]) << endl;
      cout << "10 <=  a[i] : " << (10L <= a[i]) << endl;
      cout << "10 >=  a[i] : " << (10L >= a[i]) << endl;
      cout << "10 <   a[i] : " << (10L < a[i]) << endl;
      cout << "10 >   a[i] : " << (10L > a[i]) << endl;
      cout << "a[i] == 5 : " << (a[i] == 5UL) << endl;
      cout << "a[i] != 5 : " << (a[i] != 5UL) << endl;
      cout << "a[i] <= 5 : " << (a[i] <= 5UL) << endl;
      cout << "a[i] >= 5 : " << (a[i] >= 5UL) << endl;
      cout << "a[i] <  5 : " << (a[i] < 5UL) << endl;
      cout << "a[i] >  5 : " << (a[i] > 5UL) << endl;
      cout << "10 ==  a[i] : " << (10UL == a[i]) << endl;
      cout << "10 !=  a[i] : " << (10UL != a[i]) << endl;
      cout << "10 <=  a[i] : " << (10UL <= a[i]) << endl;
      cout << "10 >=  a[i] : " << (10UL >= a[i]) << endl;
      cout << "10 <   a[i] : " << (10UL < a[i]) << endl;
      cout << "10 >   a[i] : " << (10UL > a[i]) << endl;
      cout << "a[i] == 6.2 : " << (a[i] == 6.2f) << endl;
      cout << "a[i] != 6.2 : " << (a[i] != 6.2f) << endl;
      cout << "a[i] <= 6.2 : " << (a[i] <= 6.2f) << endl;
      cout << "a[i] >= 6.2 : " << (a[i] >= 6.2f) << endl;
      cout << "a[i] <  6.2 : " << (a[i] < 6.2f) << endl;
      cout << "a[i] >  6.2 : " << (a[i] > 6.2f) << endl;
      cout << "-11.6 == a[i] : " << (-11.6f == a[i]) << endl;
      cout << "-11.6 != a[i] : " << (-11.6f != a[i]) << endl;
      cout << "-11.6 <= a[i] : " << (-11.6f <= a[i]) << endl;
      cout << "-11.6 >= a[i] : " << (-11.6f >= a[i]) << endl;
      cout << "-11.6 <  a[i] : " << (-11.6f < a[i]) << endl;
      cout << "-11.6 >  a[i] : " << (-11.6f > a[i]) << endl;
      cout << "a[i] == 6.2 : " << (a[i] == 6.2) << endl;
      cout << "a[i] != 6.2 : " << (a[i] != 6.2) << endl;
      cout << "a[i] <= 6.2 : " << (a[i] <= 6.2) << endl;
      cout << "a[i] >= 6.2 : " << (a[i] >= 6.2) << endl;
      cout << "a[i] <  6.2 : " << (a[i] < 6.2) << endl;
      cout << "a[i] >  6.2 : " << (a[i] > 6.2) << endl;
      cout << "-11.6 == a[i] : " << (-11.6 == a[i]) << endl;
      cout << "-11.6 != a[i] : " << (-11.6 != a[i]) << endl;
      cout << "-11.6 <= a[i] : " << (-11.6 <= a[i]) << endl;
      cout << "-11.6 >= a[i] : " << (-11.6 >= a[i]) << endl;
      cout << "-11.6 <  a[i] : " << (-11.6 < a[i]) << endl;
      cout << "-11.6 >  a[i] : " << (-11.6 > a[i]) << endl;
      cout << " * * * " << endl;
    }
  return 0;
}
