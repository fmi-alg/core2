/*
SubVor, a program for computing Voronoi diagrams via subdivision.
Copyright (C) 2016 Huck Bennett, Evanthia Papadopoulou, and Chee Yap.
For comments or questions, please contact Huck Bennett at hbennett@cs.nyu.edu.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//------------------------------------------------------------------------------
//  Copyright 2010-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#pragma once
#if !defined(_MATHTOOL_Point_H_)
#define _MATHTOOL_Point_H_

#include <math.h>
#include "Vector.h"

//define k-D point
template<class T, int D=3>
class Point {
public:

///////////////////////////////////////////////////////////////////////////////////
//
//  Constructors
//
///////////////////////////////////////////////////////////////////////////////////

  Point(const T& x=0,const T& y=0,const T& z=0, const T& w=0)
    { 
      set(x,y,z,w); 
    }

  Point(const T V[D])
    {
      set(V);
    }

  Point( const Point & other ) { *this = other; }

  Point( const Vector<T,D>& vec ){ 
    set(vec.get());
  }

///////////////////////////////////////////////////////////////////////////////////
//
//  Operators
//
///////////////////////////////////////////////////////////////////////////////////
  const Point & operator=( const Point & other ) {
    //memcpy(v,other.v,sizeof(T)*D);
    for(int i=0;i<D;i++) v[i]=other.v[i];
    return *this;
  }

  bool operator==( const Point & other ) const {
    for( int i=0;i<D;i++ )
      if( other.v[i]!=v[i] ) return false;
    return true;
  }

  bool almost_equ( const Point & other ) const {
    for( int i=0;i<D;i++ )
      if( fabs(other.v[i]-v[i])>1e-10 ) return false;
    return true;
  }

  bool operator!=( const Point & other ) const {
    return !(*this==other);
  }

  Vector<T,D> operator*(double r) const {
    Vector<T, D> vec;
    for (int i = 0; i < D; i++) {
      vec[i] = r * v[i];
    }
    return vec;
  }

  Vector<T,D> operator+(const Point& other) const {
    Vector<T, D> vec;
    for (int i = 0; i < D; i++) {
      vec[i] = v[i] + other.v[i];
    }
    return vec;
  }

  Vector<T,D> operator-(const Point& other) const {
    Vector<T, D> vec;
    for (int i = 0; i < D; i++) {
      vec[i] = v[i] - other.v[i];
    }
    return vec;
  }

///////////////////////////////////////////////////////////////////////////////////
//
//  Access
//
///////////////////////////////////////////////////////////////////////////////////
	
  void set(const T& x=0,const T& y=0,const T& z=0, const T& w=0) {
    if( D>0 ) v[0]=x; if( D>1 ) v[1]=y;
    if( D>2 ) v[2]=z; if( D>3 ) v[3]=w;
    for(int i=4;i<D;i++) v[i]=0;
  }

  void set(const T other[D]) { for(int i=0;i<D;i++) v[i]=other[i]; } //memcpy(v,other,sizeof(T)*D); }

  void get(T other[D]) const { for(int i=0;i<D;i++) other[i]=v[i]; } //memcpy(other,v,sizeof(T)*D); }
  const T * get() const { return v; }
  T * get() { return v; }
	
  T& operator[]( int i ){ return v[i]; }
  const T& operator[]( int i ) const{ return v[i]; }

private:
  T v[D];
};

template<class T, int D>
ostream & operator<<(ostream & out, const Point<T,D> & point) {
  for( int d=0;d<D;d++ ) out<<point[d]<<" ";
  return out;
}

template<class T, int D>
istream & operator>>(istream & in, Point<T,D> & point) {
  for( int d=0;d<D;d++ ) 
    in>>point[d];

  return in;
}

// Typedef common used vector type
typedef Point<double,2> Point2d;
typedef Point<double,3> Point3d;
typedef Point<double,4> Point4d;


#endif // _MATHTOOL_Point_H_
