# file: heron
#	To test heron's formula and compare results
#	with table published by Kahan
#	(see also ${CORE}/progs/heron)
# $Id: heron.m,v 1.1 2006/03/07 04:51:24 exact Exp $

# REMARKS: with Digits = 13,
#	Kahan's numbers agree with Maple for all but test 4.
#	Instead of  50002.50003, Maple gives a value of 50002.499999.
#	Moreover, Maple does not change this answer if higher precision
#	is used.


## Accuracy of evaluation to duplicate Kahan's numbers
Digits := 13;

######################################################################
# Heron's formula 
######################################################################
heron := proc(a,b,c)
	local s;
	s := (a+b+c)/2;
	if ((s<a) or (s<b) or (s<c)) then
		RETURN(-1);
	else
		RETURN(evalf(sqrt(s*(s-a)*(s-b)*(s-c))));
	fi;
	end;

######################################################################
# Test routine
######################################################################
test := proc(n,a,b,c,kahan)
	local s;
	printf("=========== Problem %d =========\n", n);
	printf(" (a,b,c) = (%f, %f, %f)\n", a, b, c);
	printf(" Kahan's area = ");
	if (kahan = -1) then
		printf("ERROR\n");
	else	
		printf("%f\n", kahan);
	fi;
	printf(" Maple's area = ");
	s := heron(a,b,c);
	if (s = -1) then
		printf("ERROR\n");
	else	
		printf("%f\n", s);
	fi;
	end;


######################################################################
# ACTUAL TESTS
######################################################################
  test1 := proc()
	test(1, 10,         10,           10,           43.3012702);
	end;
  test2 := proc()
	test(2, -3,         5,            2,            -1);
	end;
  test3 := proc()
	test(3, 100000,     99999.99979,  0.00029,      9.999999990);
	end;
  test4 := proc()
	test(4, 100000,     100000,       1.00005,      50002.50003);
	end;
  test5 := proc()
	test(5, 99999.99996, 99999.99994, 0.00003,      1.118033988);
	end;
  test6 := proc()
	test(6, 99999.99996,0.00003,      99999.99994,  1.118033988);
	end;
  test7 := proc()
	test(7, 10000,      5000.000001, 15000,        612.3724358);
	end;
  test8 := proc()
	test(8, 99999.99999, 99999.99999, 200000,       -1);
	end;
  test9 := proc()
	test(9, 5278.64055, 94721.35941,  99999.99996,  0);
	end;
  test10 := proc()
	test(10, 100002,    100002,       200004,       0);
	end;
  test11 := proc()
	test(11, 31622.77662, 0.000023,   31622.77661,  0.327490458);
	end;
  test12 := proc()
	test(12, 31622.77662, 0.0155555,  31622.77661,  245.9540000);
	end;

######################################################################
# To do all tests at Digits=d
######################################################################
  testall := proc(d)
	Digits := d;
	test1(); test2(); test3(); test4(); test5(); test6();
  	test7(); test8(); test9(); test10(); test11(); test12();
	end:

  testall(13);
