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

#ifndef BI_POLY_H
#define BI_POLY_H

#include "Interval.h"
#include <map>
#include <assert.h>
#include <iostream>
#include <math.h>
#include <string>
#include <boost/lexical_cast.hpp>

using namespace std;

typedef map<int, map<int, double>*> cmap;

class BiPoly {
public:
  // TODO: Fix rampant memory leaking.
  
  // TODO
  // BiPoly(string expr) {}

  BiPoly() : degree_(0) {
    add_monomial(0.0, 0, 0);
  }
  BiPoly(const BiPoly& p2) : degree_(0) { // Copy constructor.
    const cmap* poly2 = p2.get_poly();
    for (auto xit = poly2->begin(); xit != poly2->end(); ++xit) {
      int xpow = xit->first;
      map<int, double>* xmap = xit->second;
      for (auto yit = xmap->begin(); yit != xmap->end(); ++yit) {
	int ypow = yit->first;
	double c = yit->second;
	add_monomial(c, xpow, ypow);
      }
    }
  }
  ~BiPoly() {
    // for (auto xit = poly.begin(); xit != poly.end(); ++xit) {
    //   delete xit->second;
    // }
  }

  void add_monomial(double c, int x, int y) {
    assert(x >= 0 && y >= 0);

    // if (c == 0) {
    //   return;
    // }

    map<int, double>* x_entry;
    
    if (poly.find(x) == poly.end()) {
      poly.insert(pair<int, map<int, double>*>(x, new map<int, double>()));
    }

    x_entry = poly[x];
    if (x_entry->find(y) == x_entry->end()) {
      x_entry->insert(pair<int, double>(y, 0.0));
    }

    (*x_entry)[y] += c;

    if (x + y > degree_) {
      degree_ = x + y;
    }
  }

  double eval(double x, double y) {
    double acc = 0.0;
    for (auto xit = poly.begin(); xit != poly.end(); ++xit) {
      int xpow = xit->first;
      map<int, double>* xmap = xit->second;
      for (auto yit = xmap->begin(); yit != xmap->end(); ++yit) {
	int ypow = yit->first;
	double c = yit->second;
	acc += c * pow(x, xpow) * pow(y, ypow);
      }
    }
    return acc;
  }

  // Naive interval evaluation.
  Interval eval(Interval x, Interval y) {
    Interval acc(0.0, 0.0);
    for (auto xit = poly.begin(); xit != poly.end(); ++xit) {
      int xpow = xit->first;
      map<int, double>* xmap = xit->second;
      for (auto yit = xmap->begin(); yit != xmap->end(); ++yit) {
	int ypow = yit->first;
	double c = yit->second;
	acc += c * x.pow(xpow) * y.pow(ypow);
      }
    }
    return acc;
  }

  BiPoly partial_x() {
    BiPoly px;
    for (auto xit = poly.begin(); xit != poly.end(); ++xit) {
      int xpow = xit->first;
      map<int, double>* xmap = xit->second;      

      if (xpow == 0) {
	continue;
      }
      for (auto yit = xmap->begin(); yit != xmap->end(); ++yit) {
	int ypow = yit->first;
	double c = yit->second;

	px.add_monomial(c * xpow, xpow - 1, ypow);
      }
    }

    return px;    
  }

  BiPoly partial_y() {
    BiPoly py;    
    for (auto xit = poly.begin(); xit != poly.end(); ++xit) {
      int xpow = xit->first;
      map<int, double>* xmap = xit->second;
      for (auto yit = xmap->begin(); yit != xmap->end(); ++yit) {
	int ypow = yit->first;
	double c = yit->second;
	if (ypow > 0) {
	  py.add_monomial(c * ypow, xpow, ypow - 1);
	}
      }
    }

    return py;
  }

  pair<BiPoly, BiPoly> gradient() {
    return {partial_x(), partial_y()};
  }
  
  void print() {
    for (auto xit = poly.begin(); xit != poly.end(); ++xit) {
      int xpow = xit->first;
      map<int, double>* xmap = xit->second;
      for (auto yit = xmap->begin(); yit != xmap->end(); ++yit) {
  	int ypow = yit->first;
  	double c = yit->second;
  	if (xit != poly.begin() || yit != xmap->begin()) {
  	  cout << " + ";
  	}
  	cout << c;
  	if (xpow > 0) {
  	  cout << " x^" << xpow;
  	}
  	if (ypow > 0) {
  	  cout << " y^" << ypow;
  	}
      }
    }
    cout << "\n";
  }

  friend ostream& operator<<(ostream& os, const BiPoly& p) {
    for (auto xit = p.get_poly()->begin(); xit != p.get_poly()->end(); ++xit) {
      int xpow = xit->first;
      map<int, double>* xmap = xit->second;
      for (auto yit = xmap->begin(); yit != xmap->end(); ++yit) {
	int ypow = yit->first;
	double c = yit->second;
	if (xit != p.get_poly()->begin() || yit != xmap->begin()) {
	  os << " + ";
	}
	os << c;
	if (xpow > 0) {
	  os << " x^" << xpow;
	}
	if (ypow > 0) {
	  os << " y^" << ypow;
	}
      }
    }
  }

  string to_string() {
    string s = "";
    for (auto xit = poly.begin(); xit != poly.end(); ++xit) {
      int xpow = xit->first;
      map<int, double>* xmap = xit->second;
      for (auto yit = xmap->begin(); yit != xmap->end(); ++yit) {
	int ypow = yit->first;
	double c = yit->second;

	if (c == 0 && (xpow != 0 || ypow != 0)) {
	  continue;
	}
	
	if (xit != poly.begin() || yit != xmap->begin()) {
	  if (c > 0) {
	    s += " + ";
	  } else {
	    s += " - ";
	    c *= -1;
	  }
	}
	s += boost::lexical_cast<std::string>(c);
	if (xpow > 0) {
	  s += " x^" + boost::lexical_cast<std::string>(xpow);
	}
	if (ypow > 0) {
	  s += " y^" + boost::lexical_cast<std::string>(ypow);
	}
      }
    }

    return s;
  }

  bool has_term(double c, int x, int y) const {
    auto xit = poly.find(x);
    if (xit == poly.end()) {
      return false;
    }

    auto yit = xit->second->find(y);
    if (yit == xit->second->end()) {
      return false;
    }

    return yit->second == c;
  }

  bool operator==(const BiPoly& p2) const {
    map<int, double>* xmap;
    const cmap* poly2 = p2.get_poly();
    for (auto xit = poly.begin(); xit != poly.end(); ++xit) {
      int xpow = xit->first;
      xmap = xit->second;
      for (auto yit = xmap->begin(); yit != xmap->end(); ++yit) {
	int ypow = yit->first;
	double c = yit->second;
	if (!p2.has_term(c, xpow, ypow)) {
	  return false;
	}
      }
    }

    // Inefficient!
    for (auto xit = poly2->begin(); xit != poly2->end(); ++xit) {
      int xpow = xit->first;
      xmap = xit->second;
      for (auto yit = xmap->begin(); yit != xmap->end(); ++yit) {
	int ypow = yit->first;
	double c = yit->second;
	if (!has_term(c, xpow, ypow)) {
	  return false;
	}
      }
    }

    return true;
  }
  
  BiPoly operator*(const BiPoly& p2) {
    BiPoly prod;

    // This polynomial.
    for (auto xit = poly.begin(); xit != poly.end(); ++xit) {
      int xpow = xit->first;
      map<int, double>* xmap = xit->second;
      for (auto yit = xmap->begin(); yit != xmap->end(); ++yit) {
	int ypow = yit->first;
	double c = yit->second;
	for (auto xit2 = p2.get_poly()->begin(); xit2 != p2.get_poly()->end(); ++xit2) {
	  int xpow2 = xit2->first;
	  map<int, double>* xmap2 = xit2->second;
	  for (auto yit2 = xmap2->begin(); yit2 != xmap2->end(); ++yit2) {
	    int ypow2 = yit2->first;
	    double c2 = yit2->second;
	    prod.add_monomial(c * c2, xpow + xpow2, ypow + ypow2);
	  }
	}
      }
    }

    return prod;
  }

  const cmap* get_poly() const {
    return &poly;
  }

  int degree() const {
    return degree_;
  }
  
private:
  cmap poly;
  int degree_;
};

inline BiPoly operator*(double s, const BiPoly& p2) {
  BiPoly prod;
  const cmap* poly = p2.get_poly();
  for (auto xit = poly->begin(); xit != poly->end(); ++xit) {
    int xpow = xit->first;
    map<int, double>* xmap = xit->second;
    for (auto yit = xmap->begin(); yit != xmap->end(); ++yit) {
      int ypow = yit->first;
      double c = yit->second;
      prod.add_monomial(s * c, xpow, ypow);
    }
  }

  return prod;
}

inline BiPoly operator+(const BiPoly& p1, const BiPoly& p2) {
  BiPoly sum;

  // First polynomial.
  for (auto xit = p1.get_poly()->begin(); xit != p1.get_poly()->end(); ++xit) {
    int xpow = xit->first;
    map<int, double>* xmap = xit->second;
    for (auto yit = xmap->begin(); yit != xmap->end(); ++yit) {
      int ypow = yit->first;
      double c = yit->second;
      sum.add_monomial(c, xpow, ypow);
    }
  }

  // Second polynomial.
  for (auto xit = p2.get_poly()->begin(); xit != p2.get_poly()->end(); ++xit) {
    int xpow = xit->first;
    map<int, double>* xmap = xit->second;
    for (auto yit = xmap->begin(); yit != xmap->end(); ++yit) {
      int ypow = yit->first;
      double c = yit->second;
      sum.add_monomial(c, xpow, ypow);
    }
  }

  return sum;
}

inline BiPoly operator-(const BiPoly& p1, const BiPoly& p2) {
  return p1 + (-1.0 * p2);
}

#endif // BI_POLY_H
