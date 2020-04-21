/****************************************************************************
 * BigRat.sip -- Python Interface for BigRat
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
 * $Id: BigRat.sip,v 1.2 2006/02/09 12:45:44 exact Exp $
 ***************************************************************************/

class BigRat {
%TypeHeaderCode
#include <CORE/BigRat.h>
%End
public:
  BigRat();
  BigRat(int /Constrained/);
  BigRat(double);
  BigRat(const char*);
  BigRat(const BigInt&);
  BigRat(const BigInt&, const BigInt&);

public:
  void set(const BigInt&);
  void set(int /Constrained/);
  void set(double);
  int set(const char*, int =0);

public:
  void add(const BigRat&, const BigRat&);
  void sub(const BigRat&, const BigRat&);
  void mul(const BigRat&, const BigRat&);
  void div(const BigRat&, const BigRat&);

  void neg(const BigRat&);
  void abs(const BigRat&);
  
  void mul_2exp(const BigRat&, int);
  void div_2exp(const BigRat&, int);

  int cmp(const BigRat&) const;
  int cmp(int /Constrained/) const;
  int cmp(double) const;
  int equal(const BigRat&) const;

public:
  double get_d() const;
  long get_si() const;
  unsigned long get_ui() const;
  std::string get_str(int = 10) const;
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
  void swap(BigRat&);
  void canonicalize();
  BigInt numerator() const;
  BigInt num() const;
  BigInt denominator() const;
  BigInt den() const;

  int sgn() const;
  long uMSB() const;
  long lMSB() const;
};

