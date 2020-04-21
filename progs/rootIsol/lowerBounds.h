/***************************************************
 * file: lowerBounds.h
 *
 * Compute an upper bound on the roots of the polynomial that is supposedly
 * the closest to being the best bound amongst all bounds that depend 
 * only upon the absolute value of the coefficients. Confer van der Sluis;
 * it is the first amongst the root bounds given in Chee's book.
 * 2 max { (|a_{n-k}|/|a_n|)^{1/k}: k=1..n}, where the polynomial is
 * defined as a_nx^n + a_{n-1}x^{n-1} + ... + a_1 x + a_0.
 * An improved version of this bound for the case of positive real roots
 * was given by Kioustelidis. The improvement is obtained by considering only
 * the negative coefficients of the polynomial in the radicals above and this
 * is what we implement.
 *
 * Author: Jihun 2010
 *
 * REMARKS: bottleneck is that computing the root(.,.) is very slow.
 * 	Jan'19, Chee:
 * 	Note that root(x,k) gives the k-th root of x (see Expr.h).
 * 	I think speed is no longer an issue since we invoke MPFR's root.
 * ***************************************************/

#ifndef __LOWERBOUNDS_H__
#define __LOWERBOUNDS_H__

#define CORE_LEVEL 4
#include <CORE/CORE.h>

template<typename NT>
BigFloat upperbound1(const Polynomial<NT> & P)
	{
	  BigFloat2 max=0;
	  int n = P.getTrueDegree();
	  //  cout << endl <<" Inside upper bound " << endl;  
	
	  for(int i=1; i <=n; i++){
	    if(sign(P.coeff()[n-i]) < 0) {
	       BigFloat2 cc;
/* Jan'18, Chee:
 * 	The following expression causes the compile error
 * 		"invalid use of void expr"
 *
      max=core_max(max, 
		 root((core_abs(BigFloat2(P.coeff()[n-i]))/
      core_abs(BigFloat2(P.coeff()[n]))).makeCeilExact(), i).makeCeilExact());

 *    So we break this up into smaller parts:
 */
		cc = core_abs(BigFloat2(P.coeff()[n-i]))
			/ core_abs(BigFloat2(P.coeff()[n]));
		cc = cc.makeCeilExact();
		cc = root(cc, i).makeCeilExact();
		max = core_max(max, cc);
	    }//if
	    //cout <<"max after " << i << " iteration is "<< max << endl;
	    //cout <<"Taking radical of "
	    //     << core_abs(BigFloat(P.coeff()[n-i]))
	    // 		/ core_abs(BigFloat(P.coeff()[n]))
	    // 	   <<endl;
	  }//for
	  return 2*BigFloat(max.get_max());
	}
/***************************************************
 * A better method at computing the above bound suggested by Akritas;
 * see page 350-352 of his book. Roughly we find the smallest integer k
 * such that 2^{k} \ge (m|a_{n-i}|/|a_n|)^{1/i} where the polynomial
 * is a_nx^n + a_{n-1}x^{n-1} + ... + a_0 and m is the number of negative
 * coefficients. In the following procedure we extend the code
 * suggested by Akritas to handle the case when NT are rationals; even
 * in such a situation the return value is a BigFloat. Such an extension
 * is needed because in our root isolation algorithm we shift the
 * polynomials by BigFLoats and hence the coefficients can be non-integers.
 * Remark: It seems better to choose m = 1, because that gives a tighter bound.
 *
 * ***************************************************/
template<typename NT>
BigFloat upperbound2(const Polynomial<NT> &P)
	{
	  if(zeroP(P))
	    return 0;
	  Polynomial<NT> Q(P);
	  int n = P.getTrueDegree();
	  NT leadQ = Q.coeff()[n];
	  if(leadQ < 0) Q.negate();
	
	  int m=0;
	  //Count the number of negative coefficients.
	  for(int i=0; i <=n;i++)
	    if(Q.coeff()[i] < 0) m++;
	
	  if(m==0) return 0;//No sign variations implies there is no positive root.
	  
	  //cout << "Inside upper bound2: There are " << m << " negative coefficients " << endl;
	  int init=0,kpp=0,kp=0;
	  BigInt b,c;
	  BigRat w;
	  int ii, jj, p, q, r, k;
	  NT cip, leadQp;
	
	  for(int i=n-1; i>= 0; i--){
	    if(sign(Q.coeff()[i]) < 0){
	      k = n-i;
	      //cout << i <<"th coefficient of Q " << Q.coeff()[i] << endl;
	      cip = core_abs(Q.coeff()[i]);
	      // cout << "cip = " << cip << endl;
	      w = BigRat(cip)/BigRat(core_abs(leadQ));
	      //cout << "w = " << w << endl;
	      b = numerator(w);
	      c = denominator(w);
	      //cout << "b = " << b << " c = "<< c << endl;
	      ii = floorLg(b);
	      jj = floorLg(c);
	      //cout << "Floor of b " << ii << " Floor of c "<< jj << endl;
	      p = ii - jj - 1;
	      q = p/k;
	      r = p%k;
	      //cout << "quo = "<< q << " rem= "<< r << endl;
	
	      if(r < 0){// Can this ever occur?
		r += k; q-=1;
	      }
	      kp = q +2;
	      //cout << "kp = " << kp << endl;
	      /*if(r == k-1){
		leadQp = sign(leadQ)*floorLg(core_abs(leadQ)>>(kp*k));
		if(cip > leadQp) kp ++;
		} */
	      if(init == 0 || kp > kpp){//init is needed to signal the first assignment
		init = 1; kpp = kp;    // to kpp because kp may be negative the first time.
		//cout << "New kpp = " << kpp << endl;
	      }
	    }
	  }
	  //cout << "Computed the upper bound " << kpp << endl;
	  if(kpp < 0)
	    return BigFloat(1, kpp); //If lower bound in less than 1 then 
	  else
	    return BigFloat(1) << kpp;
	}

/***************************************************
  Returns a lower bound on the real roots of the polynomial P in the positive
  x-axis. Assumes that the polynomial is in Bernstein form.
  The value computed is a power of 2.
 ***************************************************/
template<typename NT>
BigFloat lowerboundPow(Polynomial<NT> &P, 
		       BigFloat (*Bound)(const Polynomial<NT> &) = upperbound2)
	{
	  Polynomial<NT> Q(P);
	  Q.reverse();
	  //cout << endl <<" Inside Lower bound: The polynomial is " << endl;  
	  /*Q.dump();
	  cout << endl;*/
	
	  //if(temp[deg] > 0)
	  //BigFloat B= ((BigFloat)1/(upperbound2(Q))).makeFloorExact();
	  return ((BigFloat)1/(Bound(Q))).makeFloorExact();
	  //cout <<"Computed a lower bound " << endl;
	}


/***************************************************
 * Returns a lower bound on the roots of the polynomial P in the unit interval.
 * Assumes that the polynomial is in Bernstein form. The lower bound is achieved
 * by computing the lower bound for the polynomial P(x/(1+x)). But the monomial
 * basis coefficients of this polynomial are a multiple of the coeffs of P(x),
 * and hence can be constructed in linear time. Once we have the lower bound B
 * for P(x/(1+x)), we return B/(1+B) as a lower bound for P(x).
 ***************************************************/
template < typename NT >
BigFloat lowerBoundB( const Polynomial<NT> & P, int deg)
	{
	  //cout<<"Input polynomial is "<< P << std::endl;
	  Polynomial<NT> Q(deg);
	  Q.coeff()[0] = P.coeff()[0]; Q.coeff()[deg] = P.coeff()[deg];
	  BigInt degfact = 1, ifact = 1;
	  for(int i=1; i < deg; i++){
	    degfact *= deg - i+1; ifact *= i;
	    Q.coeff()[i] = P.coeff()[i]*div_exact(degfact, ifact);
	  }
	  //cout<<"Transformed polynomial is "<< std::endl;
	  //Q.dump(); std::cout<<std::endl;
	  if(Q.coeff()[0] == 0)// Lower bound works only if the constant coeff is not zero
	    Q.mulXpower(-1);
	  BigFloat B = HongLowerBound(Q);// B is a power of two or zero.lowerbound2(Q);//
	  //  std::cout<<"Lower bound for transformed polynomial "<< B << std::endl;
	  BigFloat2 temp = BigFloat2(B)/BigFloat2(B+1);
	  return(temp.makeFloorExact());
	}

/***************************************************
  A lower bound based upon the upper bound given by Hong. This should be the
  best possible, since Hong's bound is better than Kioustelidis' bound.
  The problem is that it is an O(n^2) procedure. Assumes the polynomial
  P is in monomial form. Assumes the constant coefficient is not zero.
 ***************************************************/
template < typename NT >
BigInt HongLowerBound( const Polynomial<NT> & P)
	{
	  int deg = P.getDegree();
	  //  cout<<"Inside Hong's bound"<<endl;
	  long lB, gB;
	  bool localBoundSet = false, globalBoundSet = false;
	  long temp, q;
	  int s = sign(P.coeff()[0]);
	  if(s==0)
	    cout<<"Constant coefficient is zero."<<endl;
	  //  std::cout << "P[0]= " << P.coeff()[0] << std::endl;
	  for(int i= deg; i > 0; i--){
	    //    std::cout<< "P["<< i<< "]"<< " = "<< P.coeff()[i]<< std::endl;
	    if(sign(P.coeff()[i]) * s < 0){
	      for(int k=i-1; k >= 0; k--){
		if(sign(P.coeff()[k]) * s > 0){
		  //	  cout<<"Sign different"<<endl;
		  temp = floorLg( P.coeff()[i] )  - floorLg( P.coeff()[k] ) -1;
		  q = temp /(i-k);
		  // cout<<"temp = "<<temp<< " q= "<<q<< " (i-k)= "<< i-k<<endl;
		  if(!localBoundSet || lB > q + 2){
		    localBoundSet = true;
		    lB = q+2;
		  }
		}
	      }
	      //      cout <<"local bound is "<<lB<<endl;
	      localBoundSet = false;
	      if(!globalBoundSet || gB < lB){
		globalBoundSet = true;
		gB = lB;
	      }
	    }
	    //std::cout <<"Cout gb after " << i <<" loop = "<< gB << std::endl;
	  }
	  if ( gB+1 <= 0 ) return BigInt(1)<< core_abs(gB+1);
	  return 0;
	}

#endif //LOWERBOUNDS__H
