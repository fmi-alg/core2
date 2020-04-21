/****************************************************************************
 * Mpfr.sip -- Python Interface for Mpfr
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
 * $Id: Mpfr.sip,v 1.1.1.1 2006/02/09 09:18:13 exact Exp $
 ***************************************************************************/

class Mpfr {
%TypeHeaderCode
#include <CORE/Mpfr.hpp>
%End
public:
  Mpfr();
  Mpfr(int /Constrained/);
  Mpfr(double);
  Mpfr(const BigInt&);
  Mpfr(const BigRat&);
  Mpfr(const char*, int = 10);

  Mpfr(int /Constrained/, int);
  Mpfr(double, int);
  Mpfr(const BigInt&, int);
  Mpfr(const BigRat&, int);
  Mpfr(const char*, int, int);

  Mpfr(int, int, int);

public:
  int get_prec() const;
  void set_prec(int);

  bool is_exact() const;
  void set_exact(bool);

public:
  // raw version
  bool r_set(const Mpfr&);
  bool r_set(int /Constrained/);
  bool r_set(double);
  bool r_set(const BigInt&);
  bool r_set(const BigRat&);
  bool r_set(const char*, int = 10);
  bool r_set_2exp(int, int);

  // fix version
  bool set(const Mpfr&, int);
  bool set(int /Constrained/, int);
  bool set(double, int);
  bool set(const BigInt&, int);
  bool set(const BigRat&, int);
  bool set(const char*, int, int);
  bool set_2exp(int, int, int);
  
  // auto version
  bool set(const Mpfr&);
  bool set(int /Constrained/);
  bool set(double);
  bool set(const BigInt&);
  bool set(const BigRat&);
  bool set(const char*, int = 10);
  bool set_2exp(int, int);
  
  // raw version
  bool r_neg(const Mpfr&);
  bool r_neg(int /Constrained/);
  bool r_neg(double);
  bool r_neg(const BigInt&);
  bool r_neg(const BigRat&);

  bool r_sqrt(const Mpfr&);
  bool r_sqrt(int /Constrained/);
  bool r_sqrt(double);
  bool r_sqrt(const BigInt&);
  bool r_sqrt(const BigRat&);

  bool r_cbrt(const Mpfr&);

  bool r_root(const Mpfr&, int);

  bool r_addsub(const Mpfr&, const Mpfr&, bool);
  bool r_addsub(const Mpfr&, int /Constrained/, bool);
  bool r_addsub(int /Constrained/, const Mpfr&, bool);
  bool r_addsub(const Mpfr&, double, bool);
  bool r_addsub(double, const Mpfr&, bool);
  bool r_addsub(const Mpfr&, const BigInt&, bool);
  bool r_addsub(const BigInt&, const Mpfr&, bool);
  bool r_addsub(const Mpfr&, const BigRat&, bool);
  bool r_addsub(const BigRat&, const Mpfr&, bool);

  bool r_add(const Mpfr&, const Mpfr&);
  bool r_add(const Mpfr&, int /Constrained/);
  bool r_add(int /Constrained/, const Mpfr&);
  bool r_add(const Mpfr&, double);
  bool r_add(double, const Mpfr&);
  bool r_add(const Mpfr&, const BigInt&);
  bool r_add(const BigInt&, const Mpfr&);
  bool r_add(const Mpfr&, const BigRat&);
  bool r_add(const BigRat&, const Mpfr&);

  bool r_sub(const Mpfr&, const Mpfr&);
  bool r_sub(const Mpfr&, int /Constrained/);
  bool r_sub(int /Constrained/, const Mpfr&);
  bool r_sub(const Mpfr&, double);
  bool r_sub(double, const Mpfr&);
  bool r_sub(const Mpfr&, const BigInt&);
  bool r_sub(const BigInt&, const Mpfr&);
  bool r_sub(const Mpfr&, const BigRat&);
  bool r_sub(const BigRat&, const Mpfr&);

  bool r_mul(const Mpfr&, const Mpfr&);
  bool r_mul(const Mpfr&, int /Constrained/);
  bool r_mul(int /Constrained/, const Mpfr&);
  bool r_mul(const Mpfr&, double);
  bool r_mul(double, const Mpfr&);
  bool r_mul(const Mpfr&, const BigInt&);
  bool r_mul(const BigInt&, const Mpfr&);
  bool r_mul(const Mpfr&, const BigRat&);
  bool r_mul(const BigRat&, const Mpfr&);

  bool r_div(const Mpfr&, const Mpfr&);
  bool r_div(const Mpfr&, int /Constrained/);
  bool r_div(int /Constrained/, const Mpfr&);
  bool r_div(const Mpfr&, double);
  bool r_div(double, const Mpfr&);
  bool r_div(const Mpfr&, const BigInt&);
  bool r_div(const BigInt&, const Mpfr&);
  bool r_div(const Mpfr&, const BigRat&);
  bool r_div(const BigRat&, const Mpfr&);

  // fix version
  bool neg(const Mpfr&, int);
  bool neg(int /Constrained/, int);
  bool neg(double, int);
  bool neg(const BigInt&, int);
  bool neg(const BigRat&, int);

  bool sqrt(const Mpfr&, int = BF_DEF_SQRT_PREC);
  bool sqrt(int /Constrained/, int = BF_DEF_SQRT_PREC);
  bool sqrt(double, int = BF_DEF_SQRT_PREC);
  bool sqrt(const BigInt&, int = BF_DEF_SQRT_PREC);
  bool sqrt(const BigRat&, int = BF_DEF_SQRT_PREC);

  bool cbrt(const Mpfr&, int = BF_DEF_CBRT_PREC);

  bool root(const Mpfr&, int, int = BF_DEF_ROOT_PREC);

  bool addsub(const Mpfr&, const Mpfr&, bool, int);
  bool addsub(const Mpfr&, int /Constrained/, bool, int);
  bool addsub(int /Constrained/, const Mpfr&, bool, int);
  bool addsub(const Mpfr&, double, bool, int);
  bool addsub(double, const Mpfr&, bool, int);
  bool addsub(const Mpfr&, const BigInt&, bool, int);
  bool addsub(const BigInt&, const Mpfr&, bool, int);
  bool addsub(const Mpfr&, const BigRat&, bool, int);
  bool addsub(const BigRat&, const Mpfr&, bool, int);

  bool add(const Mpfr&, const Mpfr&, int);
  bool add(const Mpfr&, int /Constrained/, int);
  bool add(int /Constrained/, const Mpfr&, int);
  bool add(const Mpfr&, double, int);
  bool add(double, const Mpfr&, int);
  bool add(const Mpfr&, const BigInt&, int);
  bool add(const BigInt&, const Mpfr&, int);
  bool add(const Mpfr&, const BigRat&, int);
  bool add(const BigRat&, const Mpfr&, int);

  bool sub(const Mpfr&, const Mpfr&, int);
  bool sub(const Mpfr&, int /Constrained/, int);
  bool sub(int /Constrained/, const Mpfr&, int);
  bool sub(const Mpfr&, double, int);
  bool sub(double, const Mpfr&, int);
  bool sub(const Mpfr&, const BigInt&, int);
  bool sub(const BigInt&, const Mpfr&, int);
  bool sub(const Mpfr&, const BigRat&, int);
  bool sub(const BigRat&, const Mpfr&, int);

  bool mul(const Mpfr&, const Mpfr&, int);
  bool mul(const Mpfr&, int /Constrained/, int);
  bool mul(int /Constrained/, const Mpfr&, int);
  bool mul(const Mpfr&, double, int);
  bool mul(double, const Mpfr&, int);
  bool mul(const Mpfr&, const BigInt&, int);
  bool mul(const BigInt&, const Mpfr&, int);
  bool mul(const Mpfr&, const BigRat&, int);
  bool mul(const BigRat&, const Mpfr&, int);

  bool div(const Mpfr&, const Mpfr&, int = BF_DEF_DIV_PREC);
  bool div(const Mpfr&, int /Constrained/, int = BF_DEF_DIV_PREC);
  bool div(int /Constrained/, const Mpfr&, int = BF_DEF_DIV_PREC);
  bool div(const Mpfr&, double, int = BF_DEF_DIV_PREC);
  bool div(double, const Mpfr&, int = BF_DEF_DIV_PREC);
  bool div(const Mpfr&, const BigInt&, int = BF_DEF_DIV_PREC);
  bool div(const BigInt&, const Mpfr&, int = BF_DEF_DIV_PREC);
  bool div(const Mpfr&, const BigRat&, int = BF_DEF_DIV_PREC);
  bool div(const BigRat&, const Mpfr&, int = BF_DEF_DIV_PREC);

  // auto version
  bool neg(const Mpfr&);
  bool neg(int /Constrained/);
  bool neg(double);
  bool neg(const BigInt&);
  bool neg(const BigRat&);

  bool addsub(const Mpfr&, const Mpfr&, bool);
  bool addsub(const Mpfr&, int /Constrained/, bool);
  bool addsub(int /Constrained/, const Mpfr&, bool);
  bool addsub(const Mpfr&, double, bool);
  bool addsub(double, const Mpfr&, bool);
  bool addsub(const Mpfr&, const BigInt&, bool);
  bool addsub(const BigInt&, const Mpfr&, bool);
  bool addsub(const Mpfr&, const BigRat&, bool);
  bool addsub(const BigRat&, const Mpfr&, bool);

  bool add(const Mpfr&, const Mpfr&);
  bool add(const Mpfr&, int /Constrained/);
  bool add(int /Constrained/, const Mpfr&);
  bool add(const Mpfr&, double);
  bool add(double, const Mpfr&);
  bool add(const Mpfr&, const BigInt&);
  bool add(const BigInt&, const Mpfr&);
  bool add(const Mpfr&, const BigRat&);
  bool add(const BigRat&, const Mpfr&);

  bool sub(const Mpfr&, const Mpfr&);
  bool sub(const Mpfr&, int /Constrained/);
  bool sub(int /Constrained/, const Mpfr&);
  bool sub(const Mpfr&, double);
  bool sub(double, const Mpfr&);
  bool sub(const Mpfr&, const BigInt&);
  bool sub(const BigInt&, const Mpfr&);
  bool sub(const Mpfr&, const BigRat&);
  bool sub(const BigRat&, const Mpfr&);

  bool mul(const Mpfr&, const Mpfr&);
  bool mul(const Mpfr&, int /Constrained/);
  bool mul(int /Constrained/, const Mpfr&);
  bool mul(const Mpfr&, double);
  bool mul(double, const Mpfr&);
  bool mul(const Mpfr&, const BigInt&);
  bool mul(const BigInt&, const Mpfr&);
  bool mul(const Mpfr&, const BigRat&);
  bool mul(const BigRat&, const Mpfr&);

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

public:
  void swap(Mpfr&);
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

