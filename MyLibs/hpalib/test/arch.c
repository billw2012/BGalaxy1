/*
   arch - version 1.0
   Copyright (C)  2005    Ivano Primi  <ivprimi@libero.it>    

   This is the source file of the "arch" program.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public
   License along with this program; if not, write to the Free
   Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
   02110-1301 USA.
*/

int main(void)
{
  /*
    This program returns 1 on a 64-bit architecture,
    0 on a 32- and 16-bit architecture.
  */
  return (sizeof (unsigned long) > 4 ? 1 : 0);
}
