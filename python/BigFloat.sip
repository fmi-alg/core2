/****************************************************************************
 * BigFloat.sip -- Python Interface for BigFloat
 *
 * Core Library Version 2.0, March 2006
 * Copyright (c) 1995-2006 Exact Computation Project
 * All rights reserved.
 *
 * This file is part of Core Library (http://cs.nyu.edu/exact/core); you 
 * may redistribute it under the terms of the Q Public License version 1.0.
 * See the file LICENSE.QPL distributed with Core Library.
 *
 * Licensees holding a valid commercial license may use this file in
 * accordance with the commercial license agreement provided with the
 * software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * WWW URL: http://cs.nyu.edu/exact/core
 * Email: exact@cs.nyu.edu
 *
 * $Id: BigFloat.sip,v 1.2 2006/02/09 12:45:44 exact Exp $
 ***************************************************************************/

// TODO: add support for Mpfr after Mpfr is finalized

class BigFloat {
%TypeHeaderCode
#include <CORE/BigFloat.h>
%End
public:
  BigFloat();
  BigFloat(int /Constrained/);
  BigFloat(double);
  BigFloat(const BigInt&);
  BigFloat(const BigRat&);
  BigFloat(const char*, int = 10);

  BigFloat(int /Constrained/, int);
  BigFloat(double, int);
  BigFloat(const BigInt&, int);
  BigFloat(const BigRat&, int);
  BigFloat(const char*, int, int);

  BigFloat(int, int, int);

public:
  int get_prec() const;
  void set_prec(int);

  bool is_exact() const;
  void set_exact(bool);

public:
  // raw version
  bool r_set(const BigFloat&);
  bool r_set(int /Constrained/);
  bool r_set(double);
  bool r_set(const BigInt&);
  bool r_set(const BigRat&);
  bool r_set(const char*, int = 10);
  bool r_set_2exp(int, int);

  // fix version
  bool set(const BigFloat&, int);
  bool set(int /Constrained/, int);
  bool set(double, int);
  bool set(const BigInt&, int);
  bool set(const BigRat&, int);
  bool set(const char*, int, int);
  bool set_2exp(int, int, int);
  
  // auto version
  bool set(const BigFloat&);
  bool set(int /Constrained/);
  bool set(double);
  bool set(const BigInt&);
  bool set(const BigRat&);
  bool set(const char*, int = 10);
  bool set_2exp(int, int);
  
  // raw version
  bool r_neg(const BigFloat&);
  bool r_neg(int /Constrained/);
  bool r_neg(double);
  bool r_neg(const BigInt&);
  bool r_neg(const BigRat&);

  bool r_sqrt(const BigFloat&);
  bool r_sqrt(int /Constrained/);
  bool r_sqrt(double);
  bool r_sqrt(const BigInt&);
  bool r_sqrt(const BigRat&);

  bool r_cbrt(const BigFloat&);

  bool r_root(const BigFloat&, int);

  bool r_addsub(const BigFloat&, const BigFloat&, bool);
  bool r_addsub(const BigFloat&, int /Constrained/, bool);
  bool r_addsub(int /Constrained/, const BigFloat&, bool);
  bool r_addsub(const BigFloat&, double, bool);
  bool r_addsub(double, const BigFloat&, bool);
  bool r_addsub(const BigFloat&, const BigInt&, bool);
  bool r_addsub(const BigInt&, const BigFloat&, bool);
  bool r_addsub(const BigFloat&, const BigRat&, bool);
  bool r_addsub(const BigRat&, const BigFloat&, bool);

  bool r_add(const BigFloat&, const BigFloat&);
  bool r_add(const BigFloat&, int /Constrained/);
  bool r_add(int /Constrained/, const BigFloat&);
  bool r_add(const BigFloat&, double);
  bool r_add(double, const BigFloat&);
  bool r_add(const BigFloat&, const BigInt&);
  bool r_add(const BigInt&, const BigFloat&);
  bool r_add(const BigFloat&, const BigRat&);
  bool r_add(const BigRat&, const BigFloat&);

  bool r_sub(const BigFloat&, const BigFloat&);
  bool r_sub(const BigFloat&, int /Constrained/);
  bool r_sub(int /Constrained/, const BigFloat&);
  bool r_sub(const BigFloat&, double);
  bool r_sub(double, const BigFloat&);
  bool r_sub(const BigFloat&, const BigInt&);
  bool r_sub(const BigInt&, const BigFloat&);
  bool r_sub(const BigFloat&, const BigRat&);
  bool r_sub(const BigRat&, const BigFloat&);

  bool r_mul(const BigFloat&, const BigFloat&);
  bool r_mul(const BigFloat&, int /Constrained/);
  bool r_mul(int /Constrained/, const BigFloat&);
  bool r_mul(const BigFloat&, double);
  bool r_mul(double, const BigFloat&);
  bool r_mul(const BigFloat&, const BigInt&);
  bool r_mul(const BigInt&, const BigFloat&);
  bool r_mul(const BigFloat&, const BigRat&);
  bool r_mul(const BigRat&, const BigFloat&);

  bool r_div(const BigFloat&, const BigFloat&);
  bool r_div(const BigFloat&, int /Constrained/);
  bool r_div(int /Constrained/, const BigFloat&);
  bool r_div(const BigFloat&, double);
  bool r_div(double, const BigFloat&);
  bool r_div(const BigFloat&, const BigInt&);
  bool r_div(const BigInt&, const BigFloat&);
  bool r_div(const BigFloat&, const BigRat&);
  bool r_div(const BigRat&, const BigFloat&);

  // fix version
  bool neg(const BigFloat&, int);
  bool neg(int /Constrained/, int);
  bool neg(double, int);
  bool neg(const BigInt&, int);
  bool neg(const BigRat&, int);

  bool sqrt(const BigFloat&, int = BF_DEF_SQRT_PREC);
  bool sqrt(int /Constrained/, int = BF_DEF_SQRT_PREC);
  bool sqrt(double, int = BF_DEF_SQRT_PREC);
  bool sqrt(const BigInt&, int = BF_DEF_SQRT_PREC);
  bool sqrt(const BigRat&, int = BF_DEF_SQRT_PREC);

  bool cbrt(const BigFloat&, int = BF_DEF_CBRT_PREC);

  bool root(const BigFloat&, int, int = BF_DEF_ROOT_PREC);

  bool addsub(const BigFloat&, const BigFloat&, bool, int);
  bool addsub(const BigFloat&, int /Constrained/, bool, int);
  bool addsub(int /Constrained/, const BigFloat&, bool, int);
  bool addsub(const BigFloat&, double, bool, int);
  bool addsub(double, const BigFloat&, bool, int);
  bool addsub(const BigFloat&, const BigInt&, bool, int);
  bool addsub(const BigInt&, const BigFloat&, bool, int);
  bool addsub(const BigFloat&, const BigRat&, bool, int);
  bool addsub(const BigRat&, const BigFloat&, bool, int);

  bool add(const BigFloat&, const BigFloat&, int);
  bool add(const BigFloat&, int /Constrained/, int);
  bool add(int /Constrained/, const BigFloat&, int);
  bool add(const BigFloat&, double, int);
  bool add(double, const BigFloat&, int);
  bool add(const BigFloat&, const BigInt&, int);
  bool add(const BigInt&, const BigFloat&, int);
  bool add(const BigFloat&, const BigRat&, int);
  bool add(const BigRat&, const BigFloat&, int);

  bool sub(const BigFloat&, const BigFloat&, int);
  bool sub(const BigFloat&, int /Constrained/, int);
  bool sub(int /Constrained/, const BigFloat&, int);
  bool sub(const BigFloat&, double, int);
  bool sub(double, const BigFloat&, int);
  bool sub(const BigFloat&, const BigInt&, int);
  bool sub(const BigInt&, const BigFloat&, int);
  bool sub(const BigFloat&, const BigRat&, int);
  bool sub(const BigRat&, const BigFloat&, int);

  bool mul(const BigFloat&, const BigFloat&, int);
  bool mul(const BigFloat&, int /Constrained/, int);
  bool mul(int /Constrained/, const BigFloat&, int);
  bool mul(const BigFloat&, double, int);
  bool mul(double, const BigFloat&, int);
  bool mul(const BigFloat&, const BigInt&, int);
  bool mul(const BigInt&, const BigFloat&, int);
  bool mul(const BigFloat&, const BigRat&, int);
  bool mul(const BigRat&, const BigFloat&, int);

  bool div(const BigFloat&, const BigFloat&, int = BF_DEF_DIV_PREC);
  bool div(const BigFloat&, int /Constrained/, int = BF_DEF_DIV_PREC);
  bool div(int /Constrained/, const BigFloat&, int = BF_DEF_DIV_PREC);
  bool div(const BigFloat&, double, int = BF_DEF_DIV_PREC);
  bool div(double, const BigFloat&, int = BF_DEF_DIV_PREC);
  bool div(const BigFloat&, const BigInt&, int = BF_DEF_DIV_PREC);
  bool div(const BigInt&, const BigFloat&, int = BF_DEF_DIV_PREC);
  bool div(const BigFloat&, const BigRat&, int = BF_DEF_DIV_PREC);
  bool div(const BigRat&, const BigFloat&, int = BF_DEF_DIV_PREC);

  // auto version
  bool neg(const BigFloat&);
  bool neg(int /Constrained/);
  bool neg(double);
  bool neg(const BigInt&);
  bool neg(const BigRat&);

  bool addsub(const BigFloat&, const BigFloat&, bool);
  bool addsub(const BigFloat&, int /Constrained/, bool);
  bool addsub(int /Constrained/, const BigFloat&, bool);
  bool addsub(const BigFloat&, double, bool);
  bool addsub(double, const BigFloat&, bool);
  bool addsub(const BigFloat&, const BigInt&, bool);
  bool addsub(const BigInt&, const BigFloat&, bool);
  bool addsub(const BigFloat&, const BigRat&, bool);
  bool addsub(const BigRat&, const BigFloat&, bool);

  bool add(const BigFloat&, const BigFloat&);
  bool add(const BigFloat&, int /Constrained/);
  bool add(int /Constrained/, const BigFloat&);
  bool add(const BigFloat&, double);
  bool add(double, const BigFloat&);
  bool add(const BigFloat&, const BigInt&);
  bool add(const BigInt&, const BigFloat&);
  bool add(const BigFloat&, const BigRat&);
  bool add(const BigRat&, const BigFloat&);

  bool sub(const BigFloat&, const BigFloat&);
  bool sub(const BigFloat&, int /Constrained/);
  bool sub(int /Constrained/, const BigFloat&);
  bool sub(const BigFloat&, double);
  bool sub(double, const BigFloat&);
  bool sub(const BigFloat&, const BigInt&);
  bool sub(const BigInt&, const BigFloat&);
  bool sub(const BigFloat&, const BigRat&);
  bool sub(const BigRat&, const BigFloat&);

  bool mul(const BigFloat&, const BigFloat&);
  bool mul(const BigFloat&, int /Constrained/);
  bool mul(int /Constrained/, const BigFloat&);
  bool mul(const BigFloat&, double);
  bool mul(double, const BigFloat&);
  bool mul(const BigFloat&, const BigInt&);
  bool mul(const BigInt&, const BigFloat&);
  bool mul(const BigFloat&, const BigRat&);
  bool mul(const BigRat&, const BigFloat&);

public:
  std::string get_str(int = 0, int = 10) const;
  SIP_PYOBJECT __str__();
%MethodCode
    std::string str;
    Py_BEGIN_ALLOW_THREADS
    str = sipCpp->get_str();
    Py_END_ALLOW_THREADS
    sipRes = PyString_FromString(str.c_str());
%End
  BigInt get_z() const;
  BigRat get_q() const;

public:
  void swap(BigFloat&);
  bool has_zero() const;
  int sgn() const;
  int uMSB() const;
  int lMSB() const;

public:  
  void set_zero();
  void set_inf();
  void set_nan();
  void is_integer();
};

