/****************************************************************************
 * BigInt.sip -- Python Interface for BigInt
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
 * $Id: BigInt.sip,v 1.2 2006/02/09 12:45:44 exact Exp $
 ***************************************************************************/

class BigInt {
%TypeHeaderCode
#include <CORE/BigInt.h>
%End
public:
  BigInt();
  BigInt(int /Constrained/);
  BigInt(double);
  BigInt(const char*, int = 0);

public:
  void set(const BigInt&);
  void set(int /Constrained/);
  void set(double);
  int set(const char*, int =0);
  
  void add(const BigInt&, const BigInt&);
  void add(const BigInt&, int /Constrained/);
  void add(int /Constrained/, const BigInt&);
  void add(const BigInt&, double);
  void add(double, const BigInt&);

  void sub(const BigInt&, const BigInt&);
  void sub(const BigInt&, int /Constrained/);
  void sub(int /Constrained/, const BigInt&);
  void sub(const BigInt&, double);
  void sub(double, const BigInt&);
  
  void mul(const BigInt&, const BigInt&);
  void mul(const BigInt&, int /Constrained/);
  void mul(int /Constrained/, const BigInt&);
  void mul(const BigInt&, double);
  void mul(double, const BigInt&);

  void div(const BigInt&, const BigInt&);
  void div(const BigInt&, int /Constrained/);
  void div(const BigInt&, double);

  void mod(const BigInt&, const BigInt&);
  void mod(const BigInt&, int /Constrained/);

  void divexact(const BigInt&, const BigInt&);
  void divexact(const BigInt&, int /Constrained/);
  void divexact(const BigInt&, double);

  void divrem(BigInt&, const BigInt&, const BigInt&);
  void divrem(BigInt&, const BigInt&, int /Constrained/);
  void divrem(BigInt&, const BigInt&, double);

  void sqrt(const BigInt&);

  void pow(const BigInt&, int);
  void pow(int, int);

  void neg(const BigInt&);
  void abs(const BigInt&);
  
  void mul_2exp(const BigInt&, int);
  void div_2exp(const BigInt&, int);

  int cmp(const BigInt&) const;
  int cmp(int /Constrained/) const;
  int cmp(double) const;
  int cmpabs(const BigInt&) const;
  int cmpabs(int /Constrained/) const;
  int cmpabs(double) const;

  void logical_and(const BigInt&, const BigInt&);
  void logical_ior(const BigInt&, const BigInt&);
  void logical_xor(const BigInt&, const BigInt&);
  void logical_com(const BigInt&);

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
  double get_d_2exp(long*) const;
  int get_2exp() const;
  int get_k_exp(BigInt&, int) const;

public:
  void swap(BigInt&);
  void gcd(const BigInt&, const BigInt&);
  int sizeinbase(int = 2) const;
  int ceillg() const;
  int floorlg() const;
  int sgn() const;
  int uMSB() const;
  int lMSB() const;

public:  
  bool is_divisible(const BigInt&) const;
  bool is_odd() const;
  bool is_even() const;
  bool is_ulong() const;
  bool is_slong() const;
  bool is_uint() const;
  bool is_sint() const;
  bool is_ushort() const;
  bool is_sshort() const;
};

