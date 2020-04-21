/*
 *  interval.cpp
 *  simple
 *
 *  Created by Chris Wu on 2/19/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __INTERVAL_H__
#define __INTERVAL_H__
#endif

#ifndef CORE_LEVEL
#define CORE_LEVEL 4
#endif
#include "CORE/CORE.h"

typedef queue<BFInterval> QBFInterval;

template <typename T>
void pos_fplus(int degree, const Polynomial<T> &f, Polynomial<T> &result)
{
	assert( degree > 0 );
	result.expand(degree);
	BigFloat temp;
	for(int i = 0; i <= degree; i++) {
		temp = f.getCoeff(i);
		if ( temp > 0 )
			result.setCoeff(i, temp);
	}
}
template <typename T>
void pos_fminus(int degree, const Polynomial<T> &f, Polynomial<T> &result)
{
	assert( degree > 0 );
	result.expand(degree);
	BigFloat temp;
	for(int i = 0; i <= degree; i++) {
		temp = f.getCoeff(i);
		if ( temp < 0 )
			result.setCoeff(i, -1.0 * temp);
	}
}
template <typename T>
void neg_fplus(int degree, const Polynomial<T> &f, Polynomial<T> &result)
{
	assert( degree > 0 );
	result.expand(degree);
	BigFloat temp;
	BigFloat minusone = -1.0;
	for(int i = 0; i <= degree; i++) {
		temp = f.getCoeff(i);
		minusone *= -1.0;
		if ( minusone * temp > 0 )
			result.setCoeff(i, abs(temp) );
	}
}
template <typename T>
void neg_fminus(int degree, const Polynomial<T> &f, Polynomial<T> &result)
{
	assert( degree > 0 );
	result.expand(degree);
	BigFloat temp;
	BigFloat minusone = -1.0;
	for(int i = 0; i <= degree; i++) {
		temp = f.getCoeff(i);
		minusone *= -1.0;
		if ( minusone * temp < 0 )
			result.setCoeff(i, abs(temp) );
	}
}

template <typename T>
BFInterval box(BFInterval seg, Polynomial<T> &pos, Polynomial<T> &neg)
{
  //  cout << "Pos " ;pos.dump();cout << endl;
  //  cout << "Neg ";neg.dump();cout << endl;
  BFInterval newseg;
  // f^+(lower) - f^-(upper)
  newseg.first = pos.eval(abs(seg.first)) - neg.eval(abs(seg.second));
  // f^+(upper) - f^-(lower)
  newseg.second = pos.eval(abs(seg.second)) - neg.eval(abs(seg.first));
  
  return newseg;
}

// Return the value of adding two intervals
inline
BFInterval add(BFInterval I, BFInterval J){
  return BFInterval(I.first+J.first, I.second+J.second);
}

// Return the value of multiplyin two intervals
inline
BFInterval mul(BFInterval I, BFInterval J){
  if(I.first == J.first && I.second == J.second && I.first < 0)
    return BFInterval(0, I.second * I.second);
  
  BigFloat a = I.first * J.first, b = I.first * J.second, 
    c = I.second*J.first, d = I.second*J.second;
  BigFloat mi = core_min(a,core_min(b, core_min(c,d)));
  BigFloat ma = core_max(a,core_max(b, core_max(c,d)));
  return BFInterval(mi, ma);
}

// Interval arithmetic evaluation of P.
template <typename T>
BFInterval box2(BFInterval seg, Polynomial<T> &P)
{
  //  cout << "Pos " ;pos.dump();cout << endl;
  //  cout << "Neg ";neg.dump();cout << endl;
  BFInterval val(0,0);
  for (int i=P.getTrueDegree(); i>=0; i--) {
    val = mul(val, seg);
    val = add(val, BFInterval(BigFloat(P.coeff[i]), BigFloat(P.coeff[i])));	
  }

  return val;
}

template <typename T>
unsigned int isolateRoots2(Polynomial <T> &P, BFInterval I, BFVecInterval &v)
{

  //Break the search interval into positive and negative parts
  BFInterval Ipos, Ineg;
  if(I.first * I.second < 0){
    Ineg = BFInterval(I.first, 0);
    Ipos = BFInterval(0, I.second);
  }else{
    if(I.first < 0){
      Ineg = I; Ipos =BFInterval(0,0);
    }else
      Ipos = I; Ineg = BFInterval(0,0);
  }
  unsigned int deg = P.getTrueDegree();
  QBFInterval Q;
  BFInterval seg, segtemp;
  BigFloat  midpoint, SB_poly = P.sepBound();
  Polynomial <T> derivP(P);
  derivP.differentiate();

  unsigned int count=1;

  Q.push(Ipos);//Q.push(Ineg);
  while(!Q.empty())
    {
      seg = Q.front(); Q.pop();
      
      // First set up the box function for negative or positive range
      segtemp = box2(seg, P);
      //cout <<"segtemp is " << segtemp.first << " : " << segtemp.second << endl;
      /* a sign change in the range so:
	 - root isn't at endpoints
	 - return a single/BigFloat root
	 - subdivide
      */
      if(segtemp.first * segtemp.second > 0)
	{
	 std::cout << "Rejected Box " << seg.first
		   << " : " << seg.second << std::endl;
	}	  

      if(segtemp.first * segtemp.second < 0)
	{
	  std::cout << "Eval Box has sign change on " << seg.first
		    << " : " << seg.second << std::endl;
	  
	  segtemp = box2(seg, derivP);
	  
	  if(segtemp.first * segtemp.second > 0)
	    {
	      if(P.eval(seg.first) * P.eval(seg.second)<=0)
		v.push_back(seg);
	      if(v.size() == deg)
		return count;
	    }
	  else
	    {
	      // Check if the subdivision midpoint is a root...
	      midpoint = (seg.first + seg.second).div2() ;
	      if (P.eval( midpoint ) == 0) {
		//std::cout << midpoint << " is a root "<< std::endl;
		v.push_back(BFInterval(midpoint, midpoint));
	      // If so, return the root type...
	      //deriv.eval(midpoint) == 0;
	      // cout << "Multiple root at " << midpoint << ".\n":
	      //  cout << "Single root at " << midpoint << ".\n";
		
		// Then only enqueue those segments that are large enough w.r.t.
		// the root bound
		if (midpoint - SB_poly - seg.first > 0 ) {
		  count++;
		  Q.push(BFInterval(seg.first, midpoint - SB_poly));
		}
		if ( seg.second - midpoint - SB_poly > 0 ) {
		  count++;
		  Q.push(BFInterval(midpoint + SB_poly, seg.second));
		}
	      } else {
		count += 2;
		// If not, just add the BFIntervals
		Q.push(BFInterval(seg.first, midpoint));
		Q.push(BFInterval(midpoint ,seg.second));
	      }
	    }
	}
    }
  return count;
}



// Centered form evaluation of P. seq is the list of its derivatives and
// seq[start] to seq[end] are the derivatives of P, where seq[start] is P
// and seq[end] is the last derivative of P; this format is useful for 
// reusing the same sequence for the polynomial and its derivative.
template <typename T>
BFInterval box3(BFInterval seg, int start, int end, Polynomial<T> *seq)
{
  BigFloat midpoint = (seg.first + seg.second).div2();
  //Polynomial <T> temp(P);
  int deg = seq[start].getTrueDegree();
  //std::cout <<"Input interval " << seg.first <<": " << seg.second << std::endl;
  BigFloat val=0;
  BigFloat width = seg.second - seg.first;
  BigFloat degFact=1, t;
  for(int i=end, j=deg; i > start; i--,j--){
    val *= width;
    //std::cout << "Val before " << val <<std::endl;
    t = degFact * core_abs(seq[i].eval(midpoint));
    val += t;
    //std::cout << "Val after " << val <<std::endl;
    //if(j!= 0)
    degFact *= 2*j;
    //std::cout <<"degFact = " << degFact << " j = " << j << std::endl;
  }
  val*= width;
  //std::cout <<"degFact = " << degFact << " val = " << val << std::endl;
  BFInterval J= BFInterval(-val,val);
  //std::cout << "Range " << J.first << ":" << J.second <<std::endl;
  t = degFact*seq[start].eval(midpoint);
  //std::cout << "t = " << t << std::endl;
  J.first += t; J.second +=t;
  J.first /= degFact; J.first.makeFloorExact();
  J.second /= degFact; J.second.makeCeilExact();
  //std::cout << "Range returned " << J.first << ":" << J.second <<std::endl;
  return J;
}

//isolate the roots of P in the interval I where I.first < I.second
// Uses interval arithmetic to evaluate the box functions.
template <typename T>
unsigned int isolateRoots3(Polynomial <T> &P, BFInterval I, BFVecInterval &v)
{

  //Break the search interval into positive and negative parts
  /*BFInterval Ipos, Ineg;
  if(I.first * I.second < 0){
    Ineg = BFInterval(I.first, 0);
    Ipos = BFInterval(0, I.second);
  }else{
    if(I.first < 0){
      Ineg = I; Ipos =BFInterval(0,0);
    }else
      Ipos = I; Ineg = BFInterval(0,0);
      }*/
  unsigned int deg = P.getTrueDegree();
  QBFInterval Q;
  BFInterval seg, segtemp;
  BigFloat  midpoint, SB_poly = P.sepBound();
  Polynomial <T> temp(P), derivP;
  derivP = temp.differentiate();
  Polynomial <T> seq[deg+1];
  // Compute the sequence of derivatives of the polynomial
  seq[0] = P; seq[1] = derivP;
  for(unsigned int i=2; i <= deg; i++){// seq[i] stores the i'th derivative of P
    seq[i]= temp.differentiate();
    //seq[i].dump(); std::cout << std::endl;
  }
  Q.push(I);//Q.push(Ineg);//For now isolate the positive roots only
  unsigned int count = 1;// Counts the size of the tree
  
  while(!Q.empty())
    {
      seg = Q.front(); Q.pop();
      
      // First set up the box function for negative or positive range
      segtemp = box3(seg, 0, deg, seq);
      //cout <<"segtemp is " << segtemp.first << " : " 
      //<< segtemp.second << endl;

      /*      if(segtemp.first * segtemp.second > 0)
	{
	 std::cout << "Rejected Box " << seg.first
		   << " : " << seg.second << std::endl;
		   }*/

      if(segtemp.first * segtemp.second < 0)
	{
	  //	  std::cout << "Eval Box has sign change on " << seg.first
	  //    << " : " << seg.second << std::endl;
	  
	  segtemp = box3(seg, 1, deg, seq);
	  
	  if(segtemp.first * segtemp.second > 0)
	    {
	      if(P.eval(seg.first) * P.eval(seg.second)<=0)
		v.push_back(seg);
	      if(v.size() == deg)
		return count;
	    }
	  else
	    {
	      // Check if the subdivision midpoint is a root...
	      midpoint = (seg.first + seg.second).div2() ;
	      if (P.eval( midpoint ) == 0)
		//std::cout << midpoint << " is a root "<< std::endl;
		v.push_back(BFInterval(midpoint, midpoint));
		count += 2;
		// If not, just add the BFIntervals
		Q.push(BFInterval(seg.first, midpoint));
		Q.push(BFInterval(midpoint, seg.second));
		//}
	    }
	}
    }
    
  return count; 
}

// Function for evaluating all the Taylor coefficients of a polynomial 
// around a point x. That is, outputs P^(j)(X)/j! for any point X.
// Involves only linear number of multiplications and divisions; 
// the additions are still quadratic.
template <typename T>
void evalAllLin(Polynomial<T>& P, int deg, BigFloat x, BigFloat* seq)
{
  //std::cout << "X = " << x << std::endl;
  if(x != 0){
    BigFloat powx=1;
    for(int i=0; i <= deg; i++){
      seq[i] = P.coeff[i]*powx;
      powx *= x;
      /*      
      std::cout << "coeff[" << i <<"] = "<< P.coeff[i] << std::endl;
      std::cout << "powx = "<< powx << std::endl;
      std::cout << "seq[" << i <<"] = "<< seq[i] << std::endl;
      */
    }
    powx =1;
    for(int i=0; i < deg; i++){
      for(int j=deg-1; j >= i; j--){
	seq[j] += seq[j+1];
      }
      //      std::cout << "seq[" << i <<"] = "<< seq[i] << std::endl;
      seq[i] = div_exact(seq[i], powx);// Divide out the suitable power of x.
      powx *=x;
    }
    seq[deg] = P.coeff[deg];
  }else{
    for(int i=0; i <= deg; i++)
      seq[i] = P.coeff[i];
  }
}

/*
// Similar to above but takes quadratic number of multiplications. No divisions
template <typename T>
void evalAllQuad(Polynomial<T>& P, int deg, BigFloat x, BigFloat* seq)
{
  for(int i=0; i <= deg; i++){
    seq[i] = P.coeff[i];
    //std::cout << "seq[" << i <<"] = "<< seq[i] << std::endl;
  }
  if(x != 0){
    for(int i=0; i < deg; i++){
      for(int j=deg-1; j >= i; j--){
	seq[j] += x*seq[j+1];
      }
      //      std::cout << "seq[" << i <<"] = "<< seq[i] << std::endl;
    }
  }
}
*/
template <typename T>
BFInterval box4(BFInterval seg, Polynomial<T> &P, int deg)
{
  BigFloat midpoint = (seg.first + seg.second).div2();
  //Polynomial <T> temp(P);
  //std::cout <<"Input interval " << seg.first <<": " << seg.second << std::endl;
  BigFloat evals[deg+1];
  evalAllLin(P, deg, midpoint, evals);
  //std::cout <<"After evaluating all derivatives " << std::endl;
  BigFloat val=0;
  BigFloat halfwidth = (seg.second - seg.first).div2();
  BigFloat t;
  for(int i=deg; i > 0; i--){
    //std::cout << "Val before " << val <<std::endl;
    val *=halfwidth;
    val += core_abs(evals[i]);
    //std::cout << "Val after " << val <<std::endl;
  }
  val *= halfwidth;
  BFInterval J= BFInterval(-val,val);
  t = evals[0];
  J.first += t; J.second +=t;
  //std::cout << "Range returned " << J.first << ":" << J.second <<std::endl;
  return J;
}

template <typename T>
unsigned int isolateRoots4(Polynomial <T> &P, BFInterval I, BFVecInterval &v)
{

  //Break the search interval into positive and negative parts
  unsigned int deg = P.getTrueDegree();
  QBFInterval Q;
  BFInterval seg, segtemp;
  BigFloat  midpoint;
  Polynomial<T> R(P);
  R.differentiate();

  Q.push(I);//Q.push(Ineg);//For now isolate the positive roots only
  unsigned int count = 1;// Counts the size of the tree
  
  while(!Q.empty())
    {
      seg = Q.front(); Q.pop();
      
      // First set up the box function for negative or positive range
      segtemp = box4(seg, P, deg);
      / /cout <<"segtemp is " << segtemp.first << " : " 
      //<< segtemp.second << endl;

      /*      if(segtemp.first * segtemp.second > 0)
	{
	 std::cout << "Rejected Box " << seg.first
		   << " : " << seg.second << std::endl;
		   }*/

      if(segtemp.first * segtemp.second < 0)
	{
	  //	  std::cout << "Eval Box has sign change on " << seg.first
	  //    << " : " << seg.second << std::endl;
	  
	  segtemp = box4(seg, R, deg-1);
	  
	  if(segtemp.first * segtemp.second > 0)
	    {
	      if(P.eval(seg.first) * P.eval(seg.second)<=0)
		v.push_back(seg);
	      if(v.size() == deg)
		return count;
	    }
	  else
	    {
	      // Check if the subdivision midpoint is a root...
	      midpoint = (seg.first + seg.second).div2() ;
	      if (P.eval( midpoint ) == 0)
		v.push_back(BFInterval(midpoint, midpoint));

		count += 2;
		// If not, just add the BFIntervals
		Q.push(BFInterval(seg.first, midpoint));
		Q.push(BFInterval(midpoint, seg.second));
		//}
	    }
	}
    }
    
  return count; 
}





    
 
