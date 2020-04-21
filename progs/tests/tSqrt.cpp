/**
* Feb 2017:  
* Chee:  This is a cute example from
* http://math.stackexchange.com/questions/2134903/is-there-any-mathematical-reason-for-this-digit-repetition-show
* 
* Is there any mathematical reason for this "digit-repetition"?
* 
* The number sqrt(308642)
* has a crazy decimal representation :
*
* 555.5555777777773333333511111102222222719999970133335210666544640008?
* 
* Is there any mathematical reason for so many repetitions of the digits ?
* A long block containing only a single digit would be easier to understand.
* This could mean that there are extremely good rational approximations. But
* here we * have many long one-digit-blocks , some consecutive, some
* interrupted by a few digits. I did not calculate the probability of such a
* "digit-repitition-show", but I think it is extremely small.
* 
* Does anyone have an explanation ?
* 
* number-theory radicals decimal-expansion
* shareciteimprove this question
* asked Feb 8 at 13:02
* 
* 	Peter
* 	29.8k62979
* 	17	 	
* 	Hint: 308642=(50002+2)/92308642=(50002+2)/92.
* 	– Chen Wang Feb 8 at 13:09
* 	5	 	
* 	In interestingly the prime factorization of this number is
* 	2×1543212×154321 I wonder if the 54321 has anything to do with it?
* 	– Riemann-bitcoin. Feb 8 at 13:18 
* 	2	 	
* 	On a related note, see Schizophrenic number
* 	– PM 2Ring Feb 10 at 10:29
* 	  	 	
* 	Did this come up as an actual problem or just for fun?
* 	– Brian Risk Feb 10 at 20:52 
* 	1	 	
* 	@BrianRisk Just for fun!
* 	– Peter Feb 11 at 14:01
* 	add a comment
* 	2 Answers
* 	active oldest votes
* 	up vote
* 	110
* 	down vote
* 	accepted
* 	Repeated same numbers in a decimal representation can be converted to
* 	repeated zeros by multiplication with 99. (try it out)
* 	
* 	so if we multiply
* 	9308642------v=308642×81----------v=25000002--------v9308642=308642×81=25000002
* 	since this number is allmost 5000250002 it has a lot of zeros in its
* 	decimal expansion
* 	
* 	shareciteimprove this answer
* 	edited Feb 8 at 13:18
* 	
* 	Peter
* 	29.8k62979
* 	answered Feb 8 at 13:11
* 	
* 	the_architect
* 	8561311
* 	1	 	
* 	Superb answer! (+1)
* 	– Peter Feb 8 at 13:19
* 	18	 	
* 	And the underlying reason here is the series expansion
* 	a2+x-----v=a+12ax-1(2a)3x2+2(2a)5x3-5(2a)7x4+?
* 	a2+x=a+12ax-1(2a)3x2+2(2a)5x3-5(2a)7x4+?
* 	which can be derived from the generalized binomial theorem. When 2a2a
* 	is a large power of 1010, this gives a nice decimal representation of
* 	the square root.
* 	– Henning Makholm Feb 8 at 14:04 
* 	21	 	
* 	To check that this is the "right" explanation I'd find it good to have
* 	other, similar examples. And here is another one:
* 	1975308642---------v=44444.44444472222222222135416666667209201388884650336371564...1975308642=44444.44444472222222222135416666667209201388884650336371564...
* 	which can be explained by noting that
* 	1975308642=(4000002+2)/921975308642=(4000002+2)/92.
* 	– Michael Lugo Feb 8 at 14:37
* 	add a comment
* 	up vote
* 	92
* 	down vote
* 	The architect's answer, while explaining the absolutely crucial fact
* 	that 308642------v˜5000/9=555.555…308642˜5000/9=555.555…, didn't quite
* 	make it clear why we get several runs of repeating decimals. I try to
* 	shed additional light to that using a different tool.
* 	
* 	I want to emphasize the role of the binomial series. In particular the
* 	Taylor expansion
* 	1+x-----v=1+x2-x28+x316-5x4128+7x5256-21x61024+?
* 	1+x=1+x2-x28+x316-5x4128+7x5256-21x61024+?
* 	If we plug in x=2/(5000)2=8·10-8x=2/(5000)2=8·10-8, we get
* 	M:=1+8·10-8---------v=1+4·10-8-8·10-16+32·10-24-160·10-32+?.
* 	M:=1+8·10-8=1+4·10-8-8·10-16+32·10-24-160·10-32+?.
* 	Therefore
* 	308462------v=50009M=50009+200009·10-8-400009·10-16+1600009·10-24+?=59·103+29·10-4-49·10-12+169·10-20+?.
* 	308462=50009M=50009+200009·10-8-400009·10-16+1600009·10-24+?=59·103+29·10-4-49·10-12+169·10-20+?.
* 	This explains both the runs, their starting points, as well as the
* 	origin and location of those extra digits not part of any run. For
* 	example, the run of 5+2=75+2=7s begins when the first two terms of the
* 	above series are "active". When the third term joins in, we need to
* 	subtract a 44 and a run of 33s ensues et cetera.
* 	
* 	shareciteimprove this answer
* 	edited Feb 9 at 21:34
* 	answered Feb 8 at 14:00
* 	
* 	Jyrki Lahtonen?
* 	86.8k8124242
* 	  	 	
 * 	A very good answer! (+1) , but I do not unaccept because my accepted
 * 	answer is already very good.
 * 	– Peter Feb 8 at 14:06 
* 	10	 	
* 	@Peter It is quite common to unaccept an answer after a better answer
* 	appears. Doing so helps guide readers to the best answer, which is
* 	often not the highest voted one, due to many factors, e.g. earlier
* 	answers usually get more votes, and less technical answers usually get
* 	more votes from hot-list activity (as here). This is currently (by far)
    * 	the best explanation you have.
* 	– Bill Dubuque Feb 8 at 20:35 
* 	5	 	
* 	For the record: The reason I support Peter's decision to accept the
* 	architect's answer is that mine is building upon it. Without the
* 	observation that 5000/95000/9 is an extremely good approximation I most
* 	likely would not have bothered, and most certainly would not have come
* 	up with this refinement. IMHO Math.SE works at its best, when different
* 	users add different points of view refining earlier answers. The voters
* 	very clearly like both the answers. Sunshine and smiles to all!
* 	– Jyrki Lahtonen? Feb 10 at 7:36 
* 	2	 	
* 	@JyrkiLahtonen strong agreement. I love to see answers working in
* 	tandem, and the checkmark doesn't give all that many points. Best to
* 	have the answer that others build on be the one that people read first
* 	:)
* 	– hobbs Feb 10 at 8:13
* 	  	 	
* 	You may enjoy applying your skills to the Schizophrenic numbers. ;)
* 	– PM 2Ring Feb 10 at 10:35
**/ 	


#ifndef CORE_LEVEL
#define CORE_LEVEL 3
#endif

#include "CORE.h"

int main(int argc, char* argv[])
{
    	int k=175;
        if (argc>1)   k = atoi(argv[1]);
    
	std::cout.precision(k);
	setDefaultRelPrecision(k); //?
	
	std::cout << "cout.precision = " << k << std::endl;
	
	double E = 308642;
	std::cout << "sqrt(308642) = " << sqrt(E) << std::endl;

        return 0;
}
