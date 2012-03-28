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
#include <string>
#include "xcomplex.h"

using namespace std;
using namespace HPA;

int main (void)
{
  int i;
  string s;
  unsigned long startp, n;
  xcomplex x;
  char buffer[512];
  const char *oldptr, *newptr;

  cout << "Give me a list of complex numbers (empty line to terminate):" << endl;
  do
    {
      cin.getline (buffer, 512);
      s = buffer;
      for (startp = 0, i = 1; 
	   (n = sget (s, startp, x)) > 0;
	   startp += n, i++)
	cout << "[" << i << "]: " << x << endl;
      cout << endl;
    } while (s != "");
  cout << "A new list, please (empty line to terminate):" << endl;
  do
    {
      cin.getline (buffer, 512);
      for (oldptr = buffer, i = 1; 
	   (newptr = bget (oldptr, x)) != oldptr;
	   oldptr = newptr, i++)
	cout << "[" << i << "]: " << x << endl;
      cout << endl;
    } while (*buffer != '\0');
  return 0;
}

