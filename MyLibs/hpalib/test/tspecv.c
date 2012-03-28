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

#include<stdlib.h>
#include<float.h>
#include<cxpre.h>

#define MFWD  8
#define LIM  32

void print_string (const char* str)
{
  if ((str))
    {
      printf ("%s", str);
      free ((void*) str);
    }
  else
    {
      fprintf (stderr, "*** Out of memory\n");
      exit (EXIT_FAILURE);
    }
}


int main (void)
{
  const char* slist[] = {
    "347.86",
    "-52.001",
    "1.23e4950",
    "-2.34e4950",
    "dog",
    ":12",
    "_98.76",
    NULL
  };
  const char* slist2[] = {
    "+65-4.8i",
    "-23.006+78.54i",
    "+2.23e4932-1i",
    "-2.23e4932-1i",
    "+6.7+1.5e5000i",
    "+6.7-1.5e5000i",
    "+1.0e5000+1.2e4998i",
    "-1.0e5000+1.2e4998i",
    "+1.0e5000-1.2e4998i",
    "-1.0e5000-1.2e4998i",
    "_3+5i",
    "^3+5.0e5000i",
    "^3-5.0e5000i",
    "+3+5",
    "-3_5i",
    NULL
  };
  const char* factor[8][2] = {
    {"+8.0e4931", "+1.89"},
    {"+8.0e4931", "-1.89"},
    {"-8.0e4931", "+1.89"},
    {"-8.0e4931", "-1.89"},
    {"+1.2e2500", "+3.1e2501"},
    {"+1.2e2500", "-3.1e2501"},
    {"-1.2e2500", "+3.1e2501"},
    {"-1.2e2500", "-3.1e2501"}
  };
  struct xpr list[3] = {
    xPinf,
    xMinf,
    xNaN
  };
  struct cxpr clist[9] = {
    {xPinf, xPinf},
    {xPinf, xMinf},
    {xPinf, xNaN},
    {xMinf, xPinf},
    {xMinf, xMinf},
    {xMinf, xNaN},
    {xNaN, xPinf},
    {xNaN, xMinf},
    {xNaN, xNaN}
  };
  int i;
  struct xpr x, y;
  const char* str;
  char buffer[30], *endptr;
  struct xoutflags flag;
  
  flag.fmt = -1;
  flag.ldel = flag.rdel = -1;
  flag.padding = ' ';
  flag.notat = XOUT_SCIENTIFIC;
  flag.sf = 1;
  flag.mfwd = MFWD;
  flag.lim = LIM;
  printf ("\nCHECK USAGE OF THE SPECIAL VALUES\n");
  
  printf ("[1] atox and atocx\n");
  for (i=0; slist[i] != NULL; i++)
    {
      printf ("atox(%s) = ", slist[i]);
      xout (flag, atox (slist[i]));
      putchar ('\n');
    }
  for (i=0; slist2[i] != NULL; i++)
    {
      printf ("atocx(%s) = ", slist2[i]);
/*       cxout (flag, strtocx (slist2[i], NULL)); */
      cxout (flag, atocx (slist2[i]));
      putchar ('\n');
    }

  printf ("\n[2] xadd\n");
  x = atox ("7.5e4931");
  y = x;
  printf ("x = y = ");
  xout (flag, x);
  printf (" i.e. ");
  xprint (stdout, x);
  printf ("x + y = ");
  xout (flag, xadd (x, y, 0));
  x = xneg (x);
  y = xneg (y);
  printf ("\nNow\nx = y = ");
  xout (flag, x);
  printf (" i.e. ");
  xprint (stdout, x);
  printf ("x + y = ");
  xout (flag, xadd (x, y, 0));
  putchar ('\n');

  printf ("[3] xmul\n");
  for (i=0; i<8; i++)
    {
      x = atox (factor[i][0]);
      y = atox (factor[i][1]);
      printf ("x = ");
      xprint (stdout, x);
      printf ("i.e. ");
      xout (flag, x);
      printf ("\ny = ");
      xprint (stdout, y);
      printf ("i.e. ");
      xout (flag, y);
      printf ("\nx * y = ");
      xout (flag, xmul (x, y));
      printf ("\n\n");
    }

  printf ("[4] xpr2\n");
  x = atox ("+7e4931");
  printf ("x = ");
  xprint (stdout, x);
  printf ("(+7e4931)\n");
  x = xpr2 (x, 1);
  printf ("2 * x = ");
  xout (flag, x);
  x = atox ("-7e4931");
  printf ("\nx = ");
  xprint (stdout, x);
  printf ("(-7e4931)\n");
  x = xpr2 (x, 1);
  printf ("2 * x = ");
  xout (flag, x);
  putchar ('\n');

  printf ("[5] tan\n");
  printf ("tan(+Pi/2^{-}) = ");
  xout (flag, xtan (xPi2));
  printf ("\ntan(-Pi/2^{+}) = ");
  xout (flag, xtan (xneg (xPi2)));
  putchar ('\n');

  printf ("[6] atanh\n");
  printf ("atanh(+1) = ");
  xout (flag, xatanh(xOne));
  printf ("\natanh(+2) = ");
  xout (flag, xatanh(xTwo));
  printf ("\natanh(-1) = ");
  xout (flag, xatanh(xneg(xOne)));
  printf ("\natanh(-2) = ");
  xout (flag, xatanh(xneg(xTwo)));
  putchar ('\n');

  printf ("[7] exp\n");
  x = inttox (17000);
  printf ("x = ");
  xout (flag, x);
  printf ("\nexp(x) = ");
  xout (flag, xexp(x));
  printf ("\n 2^x = ");
  xout (flag, xexp2 (x));
  printf ("\n10^x = ");
  xout (flag, xexp10 (x));
  putchar ('\n');

  printf ("[8] logarithm\n");
  printf ("ln (0) = ");
  xout (flag, xlog (xZero));
  printf ("\nlog_2 (0) = ");
  xout (flag, xlog2 (xZero));
  printf ("\nlog_10 (0) = ");
  xout (flag, xlog10 (xZero));
  putchar ('\n');

  printf ("[9] xtodbl and xtoflt\n");
  x = atox ("1.0e42");
  printf ("x = ");
  xout (flag, x);
  printf ("\nFLT_MAX = %f, xtoflt(x) = %f\n", FLT_MAX, xtoflt(x));
  x = atox ("-1.0e42");
  printf ("x = ");
  xout (flag, x);
  printf ("\n-FLT_MAX = %f, xtoflt(x) = %f\n", -FLT_MAX, xtoflt(x));
  x = atox ("1.0e310");
  printf ("x = ");
  xout (flag, x);
  printf ("\nDBL_MAX = %e, xtodbl(x) = %e\n", DBL_MAX, xtodbl(x));
  x = atox ("-1.0e310");
  printf ("x = ");
  xout (flag, x);
  printf ("\n-DBL_MAX = %e, xtodbl(x) = %e\n", -DBL_MAX, xtodbl(x));

  printf ("[10] Output functions\n");
  flag.padding = '*';
  for (i=0; i<3; i++)
    {
      printf ("xprint: \t");
      xprint (stdout, list[i]);
      printf ("xpr_print: \t");
      xpr_print (stdout, list[i], 0, 1, LIM);
      printf ("\nxpr_asprint: \t");      
      str = xpr_asprint (list[i], 0, 1, LIM);
      print_string (str);
      printf ("\nxtoa: \t\t");
      str = xtoa (list[i], LIM);
      print_string (str);
      printf ("\nxprxpr: \t");
      xprxpr (list[i], LIM);
      printf ("\nxbprint: \t");      
      xbprint(stdout, list[i]);
      printf ("xfout: \t");
      xfout(stdout, flag, list[i]);
      printf ("\nxout: \t");      
      xout (flag, list[i]);
      printf ("\nxsout: \t");            
      xsout(buffer, 30, flag, list[i]); 
      printf ("%s\n", buffer);
      printf ("--- No mandatory sign for the next 3 calls ---\n");
      flag.notat = XOUT_FIXED;
      flag.lim = -1;
      flag.sf = 0;
      printf ("xfout: \t");
      xfout(stdout, flag, list[i]);
      printf ("\nxout: \t");      
      xout (flag, list[i]);
      printf ("\nxsout: \t");            
      xsout(buffer, 30, flag, list[i]); 
      printf ("%s\n\n", buffer);
      flag.notat = XOUT_SCIENTIFIC;
      flag.lim = LIM;
      flag.sf = 1;
    }
  for (i=0; i<9; i++)
    {
      printf ("cxprint: \t");
      cxprint (stdout, clist[i]);
      printf ("cxpr_print: \t");
      cxpr_print (stdout, clist[i], 0, 1, LIM);
      printf ("\ncxpr_asprint: \t");      
      str = cxpr_asprint (clist[i], 0, 1, LIM);
      print_string (str);
      printf ("\ncxtoa: \t\t");
      str = cxtoa (clist[i], LIM);
      print_string (str);
      printf ("\ncxprcxpr: \t");
      cxprcxpr (clist[i], LIM);
      printf ("\ncxfout: \t");
      cxfout(stdout, flag, clist[i]);
      printf ("\ncxout:  \t");      
      cxout (flag, clist[i]);
      printf ("\ncxsout: \t");            
      cxsout(buffer, 30, flag, clist[i]); 
      printf ("%s\n", buffer);
      printf ("--- No mandatory sign for the next 3 calls ");
      printf ("(alternative format) ---\n");
      flag.fmt = XFMT_ALT;
      flag.notat = XOUT_FIXED;
      flag.lim = -1;
      flag.sf = 0;
      printf ("\ncxfout: \t");
      cxfout(stdout, flag, clist[i]);
      printf ("\ncxout:  \t");      
      cxout (flag, clist[i]);
      printf ("\ncxsout: \t");            
      cxsout(buffer, 30, flag, clist[i]); 
      printf ("%s\n\n", buffer);
      flag.fmt = XFMT_STD;
      flag.notat = XOUT_SCIENTIFIC;
      flag.lim = LIM;
      flag.sf = 1;
    }

  return 0;
}

/*  Test output


CHECK USAGE OF THE SPECIAL VALUES
[1] atox and atocx
atox(347.86) = +3.47859999999999999999999999999999e+2
atox(-52.001) = -5.20009999999999999999999999999999e+1
atox(1.23e4950) =     +Inf
atox(-2.34e4950) =     -Inf
atox(dog) =     ?NaN
atox(:12) =     ?NaN
atox(_98.76) =     ?NaN
atocx(+65-4.8i) = +6.49999999999999999999999999999999e+1-4.80000000000000000000000000000000e+0i
atocx(-23.006+78.54i) = -2.30060000000000000000000000000000e+1+7.85399999999999999999999999999998e+1i
atocx(+2.23e4932-1i) =     +Inf-1.00000000000000000000000000000000e+0i
atocx(-2.23e4932-1i) =     -Inf-1.00000000000000000000000000000000e+0i
atocx(+6.7+1.5e5000i) = +6.70000000000000000000000000000000e+0    +Infi
atocx(+6.7-1.5e5000i) = +6.70000000000000000000000000000000e+0    -Infi
atocx(+1.0e5000+1.2e4998i) =     +Inf    +Infi
atocx(-1.0e5000+1.2e4998i) =     -Inf    +Infi
atocx(+1.0e5000-1.2e4998i) =     +Inf    -Infi
atocx(-1.0e5000-1.2e4998i) =     -Inf    -Infi
atocx(_3+5i) =     ?NaN    ?NaNi
atocx(^3+5.0e5000i) =     ?NaN    ?NaNi
atocx(^3-5.0e5000i) =     ?NaN    ?NaNi
atocx(+3+5) = +3.00000000000000000000000000000000e+0+0.00000000000000000000000000000000e+0i
atocx(-3_5i) = -3.00000000000000000000000000000000e+0+0.00000000000000000000000000000000e+0i

[2] xadd
x = y =     +Inf i.e. 7ffe.a16185ff055f3919ae14f56e1ab3
x + y =     +Inf
Now
x = y =     -Inf i.e. fffe.a16185ff055f3919ae14f56e1ab3
x + y =     -Inf
[3] xmul
x = 7ffe.ac23c22116cbf8a3ece327ece949
i.e.     +Inf
y = 3fff.f1eb851eb851eb851eb851eb851a
i.e. +1.89000000000000000000000000000000e+0
x * y =     +Inf

x = 7ffe.ac23c22116cbf8a3ece327ece949
i.e.     +Inf
y = bfff.f1eb851eb851eb851eb851eb851a
i.e. -1.89000000000000000000000000000000e+0
x * y =     -Inf

x = fffe.ac23c22116cbf8a3ece327ece949
i.e.     -Inf
y = 3fff.f1eb851eb851eb851eb851eb851a
i.e. +1.89000000000000000000000000000000e+0
x * y =     -Inf

x = fffe.ac23c22116cbf8a3ece327ece949
i.e.     -Inf
y = bfff.f1eb851eb851eb851eb851eb851a
i.e. -1.89000000000000000000000000000000e+0
x * y =     +Inf

x = 6070.879b007383f321eed35e72e76266
i.e. +1.19999999999999999999999999999645e+2500
y = 6074.daf243652d0b39744545d430418b
i.e. +3.09999999999999999999999999999083e+2501
x * y =     +Inf

x = 6070.879b007383f321eed35e72e76266
i.e. +1.19999999999999999999999999999645e+2500
y = e074.daf243652d0b39744545d430418b
i.e. -3.09999999999999999999999999999083e+2501
x * y =     -Inf

x = e070.879b007383f321eed35e72e76266
i.e. -1.19999999999999999999999999999645e+2500
y = 6074.daf243652d0b39744545d430418b
i.e. +3.09999999999999999999999999999083e+2501
x * y =     -Inf

x = e070.879b007383f321eed35e72e76266
i.e. -1.19999999999999999999999999999645e+2500
y = e074.daf243652d0b39744545d430418b
i.e. -3.09999999999999999999999999999083e+2501
x * y =     +Inf

[4] xpr2
x = 7ffe.969f49dcf3f2798f6f46c2ef4c1f
(+7e4931)
2 * x =     +Inf
x = fffe.969f49dcf3f2798f6f46c2ef4c1f
(-7e4931)
2 * x =     -Inf
[5] tan
tan(+Pi/2^{-}) =     +Inf
tan(-Pi/2^{+}) =     -Inf
[6] atanh
atanh(+1) =     +Inf
atanh(+2) =     +Inf
atanh(-1) =     -Inf
atanh(-2) =     -Inf
[7] exp
x = +1.69999999999999999999999999999999e+4
exp(x) =     +Inf
 2^x =     +Inf
10^x =     +Inf
[8] logarithm
ln (0) =     -Inf
log_2 (0) =     -Inf
log_10 (0) =     -Inf
[9] xtodbl and xtoflt
x = +9.99999999999999999999999999999952e+41
FLT_MAX = 340282346638528859811704183484516925440.000000, xtoflt(x) = 340282346638528859811704183484516925440.000000
x = -9.99999999999999999999999999999952e+41
-FLT_MAX = -340282346638528859811704183484516925440.000000, xtoflt(x) = -340282346638528859811704183484516925440.000000
x = +9.99999999999999999999999999999635e+309
DBL_MAX = 1.797693e+308, xtodbl(x) = 1.797693e+308
x = -9.99999999999999999999999999999635e+309
-DBL_MAX = -1.797693e+308, xtodbl(x) = -1.797693e+308
[10] Output functions
xprint: 	7fff.0000000000000000000000000000
xpr_print: 	+Inf
xpr_asprint: 	+Inf
xtoa: 		Inf
xprxpr: 	Inf
xbprint: 	0111111111111111.0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 
xfout: 	****+Inf
xout: 	****+Inf
xsout: 	****+Inf
--- No mandatory sign for the next 3 calls ---
xfout: 	*****Inf
xout: 	*****Inf
xsout: 	*****Inf

xprint: 	ffff.0000000000000000000000000000
xpr_print: 	-Inf
xpr_asprint: 	-Inf
xtoa: 		-Inf
xprxpr: 	-Inf
xbprint: 	1111111111111111.0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 0000000000000000 
xfout: 	****-Inf
xout: 	****-Inf
xsout: 	****-Inf
--- No mandatory sign for the next 3 calls ---
xfout: 	****-Inf
xout: 	****-Inf
xsout: 	****-Inf

xprint: 	0000.ffffffffffffffffffffffffffff
xpr_print: 	?NaN
xpr_asprint: 	?NaN
xtoa: 		NaN
xprxpr: 	NaN
xbprint: 	0000000000000000.1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 1111111111111111 
xfout: 	****?NaN
xout: 	****?NaN
xsout: 	****?NaN
--- No mandatory sign for the next 3 calls ---
xfout: 	*****NaN
xout: 	*****NaN
xsout: 	*****NaN

cxprint: 	(7fff.0000000000000000000000000000 , 7fff.0000000000000000000000000000)
cxpr_print: 	+Inf+Infi
cxpr_asprint: 	+Inf+Infi
cxtoa: 		Inf+Infi
cxprcxpr: 	Inf+Infi
cxfout: 	****+Inf****+Infi
cxout:  	****+Inf****+Infi
cxsout: 	****+Inf****+Infi
--- No mandatory sign for the next 3 calls (alternative format) ---

cxfout: 	(*****Inf, *****Inf)
cxout:  	(*****Inf, *****Inf)
cxsout: 	(*****Inf, *****Inf)

cxprint: 	(7fff.0000000000000000000000000000 , ffff.0000000000000000000000000000)
cxpr_print: 	+Inf-Infi
cxpr_asprint: 	+Inf-Infi
cxtoa: 		Inf-Infi
cxprcxpr: 	Inf-Infi
cxfout: 	****+Inf****-Infi
cxout:  	****+Inf****-Infi
cxsout: 	****+Inf****-Infi
--- No mandatory sign for the next 3 calls (alternative format) ---

cxfout: 	(*****Inf, ****-Inf)
cxout:  	(*****Inf, ****-Inf)
cxsout: 	(*****Inf, ****-Inf)

cxprint: 	(7fff.0000000000000000000000000000 , 0000.ffffffffffffffffffffffffffff)
cxpr_print: 	+Inf?NaNi
cxpr_asprint: 	+Inf?NaNi
cxtoa: 		Inf?NaNi
cxprcxpr: 	Inf?NaNi
cxfout: 	****+Inf****?NaNi
cxout:  	****+Inf****?NaNi
cxsout: 	****+Inf****?NaNi
--- No mandatory sign for the next 3 calls (alternative format) ---

cxfout: 	(*****Inf, *****NaN)
cxout:  	(*****Inf, *****NaN)
cxsout: 	(*****Inf, *****NaN)

cxprint: 	(ffff.0000000000000000000000000000 , 7fff.0000000000000000000000000000)
cxpr_print: 	-Inf+Infi
cxpr_asprint: 	-Inf+Infi
cxtoa: 		-Inf+Infi
cxprcxpr: 	-Inf+Infi
cxfout: 	****-Inf****+Infi
cxout:  	****-Inf****+Infi
cxsout: 	****-Inf****+Infi
--- No mandatory sign for the next 3 calls (alternative format) ---

cxfout: 	(****-Inf, *****Inf)
cxout:  	(****-Inf, *****Inf)
cxsout: 	(****-Inf, *****Inf)

cxprint: 	(ffff.0000000000000000000000000000 , ffff.0000000000000000000000000000)
cxpr_print: 	-Inf-Infi
cxpr_asprint: 	-Inf-Infi
cxtoa: 		-Inf-Infi
cxprcxpr: 	-Inf-Infi
cxfout: 	****-Inf****-Infi
cxout:  	****-Inf****-Infi
cxsout: 	****-Inf****-Infi
--- No mandatory sign for the next 3 calls (alternative format) ---

cxfout: 	(****-Inf, ****-Inf)
cxout:  	(****-Inf, ****-Inf)
cxsout: 	(****-Inf, ****-Inf)

cxprint: 	(ffff.0000000000000000000000000000 , 0000.ffffffffffffffffffffffffffff)
cxpr_print: 	-Inf?NaNi
cxpr_asprint: 	-Inf?NaNi
cxtoa: 		-Inf?NaNi
cxprcxpr: 	-Inf?NaNi
cxfout: 	****-Inf****?NaNi
cxout:  	****-Inf****?NaNi
cxsout: 	****-Inf****?NaNi
--- No mandatory sign for the next 3 calls (alternative format) ---

cxfout: 	(****-Inf, *****NaN)
cxout:  	(****-Inf, *****NaN)
cxsout: 	(****-Inf, *****NaN)

cxprint: 	(0000.ffffffffffffffffffffffffffff , 7fff.0000000000000000000000000000)
cxpr_print: 	?NaN+Infi
cxpr_asprint: 	?NaN+Infi
cxtoa: 		NaN+Infi
cxprcxpr: 	NaN+Infi
cxfout: 	****?NaN****+Infi
cxout:  	****?NaN****+Infi
cxsout: 	****?NaN****+Infi
--- No mandatory sign for the next 3 calls (alternative format) ---

cxfout: 	(*****NaN, *****Inf)
cxout:  	(*****NaN, *****Inf)
cxsout: 	(*****NaN, *****Inf)

cxprint: 	(0000.ffffffffffffffffffffffffffff , ffff.0000000000000000000000000000)
cxpr_print: 	?NaN-Infi
cxpr_asprint: 	?NaN-Infi
cxtoa: 		NaN-Infi
cxprcxpr: 	NaN-Infi
cxfout: 	****?NaN****-Infi
cxout:  	****?NaN****-Infi
cxsout: 	****?NaN****-Infi
--- No mandatory sign for the next 3 calls (alternative format) ---

cxfout: 	(*****NaN, ****-Inf)
cxout:  	(*****NaN, ****-Inf)
cxsout: 	(*****NaN, ****-Inf)

cxprint: 	(0000.ffffffffffffffffffffffffffff , 0000.ffffffffffffffffffffffffffff)
cxpr_print: 	?NaN?NaNi
cxpr_asprint: 	?NaN?NaNi
cxtoa: 		NaN?NaNi
cxprcxpr: 	NaN?NaNi
cxfout: 	****?NaN****?NaNi
cxout:  	****?NaN****?NaNi
cxsout: 	****?NaN****?NaNi
--- No mandatory sign for the next 3 calls (alternative format) ---

cxfout: 	(*****NaN, *****NaN)
cxout:  	(*****NaN, *****NaN)
cxsout: 	(*****NaN, *****NaN)

*/
