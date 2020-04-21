#pragma once

#include "Corner.h"
#include "Wall.h"
#include "Line2d.h"

const double EPS = 1E-6;

class Circle
{
public:
    double x = 0, y = 0;
    double r = 0;
    Circle() {}
    Circle (double xx, double yy, double rr):x(xx), y(yy), r(rr) {}

    // Check if circle intersects a line
    // 	(and returns 2 intersection points if true)
    //
    bool intersectLine(Line2d & L, double &inter_x1, double &inter_y1,
                                   double &inter_x2, double &inter_y2) {
      double dist = L.a*x + L.b*y + L.c;
      if (abs(dist) > r + EPS) return false;
      if (abs(dist) > r - EPS && abs(dist) < r + EPS) {
          if (dist > 0){
            inter_x1 = inter_x2 = x - r * L.a;
            inter_y1 = inter_y2 = y - r * L.b;
          }
          else {
            inter_x1 = inter_x2 = x + r * L.a;
            inter_y1 = inter_y2 = y + r * L.b;
          }
      }
      else {
          if (L.a < EPS && L.a > -EPS) {
            inter_y1 = inter_y2 = -L.c/L.b;
            inter_x1 = x + sqrt(r*r - (inter_y1-y)*(inter_y1-y));
            inter_x2 = x - sqrt(r*r - (inter_y1-y)*(inter_y1-y));
          }
          else {
            // subsitute x, get function of y, A*y^2 + B*y + C = 0
            double A = 1 + (L.b/L.a)*(L.b/L.a); // for y^2
            double B = 2*((L.b/L.a)*(L.c/L.a + x) - y); // for y
            double C = y*y + (L.c/L.a +x)*(L.c/L.a +x) - r*r; //for 1

            inter_y1 = (-B + sqrt(B*B-4*A*C))/2/A;
            inter_y2 = (-B - sqrt(B*B-4*A*C))/2/A;
            inter_x1 = - (L.b*inter_y1 + L.c)/L.a;
            inter_x2 = - (L.b*inter_y2 + L.c)/L.a;
          }
      }

      auto swap = [](double &p, double &q){
        double tmp = p;
        p = q;
        q = tmp;
      };
      // make sure the vector (inter_2 -> inter_1) is in the same direction as the input line L
      if ( -L.b * (inter_x1-inter_x2) + L.a * (inter_y1-inter_y2) < 0 ) {
        swap(inter_x1, inter_x2);
        swap(inter_y1, inter_y2);
      }
      return true;
    }//intersectLine

    bool intersectLine(Line2d& L, Corner& c1, Corner& c2){
      return intersectLine(L, c1.x, c1.y, c2.x, c2.y);
    }

    double getTheta(double xx, double yy) {
      double ans = atan2(yy-y, xx-x);
      double PI = atan2(0,-1);
      if (ans < 0) ans += 2*PI;
      return ans;
    }

    double getTheta(const Corner &c) {
      return getTheta(c.x,c.y);
    }

    bool inDisk(const Corner &c) {
      return (c.x - x)*(c.x - x) + (c.y - y)*(c.y - y) <= r*r;
    }

    bool inDiskComplement(const Corner &c) {
      return (c.x - x)*(c.x - x) + (c.y - y)*(c.y - y) >= r*r;
    }

    Corner projectionOf(const Corner &c) {
        double vx = c.x - x;
        double vy = c.y - y;
        double len = sqrt(vx*vx + vy*vy);
        if (len==0) return Corner(x, y);
        double px = x + vx * r/len;
        double py = y + vy * r/len;
        return Corner(px, py);
    }
};

class Arc : Circle
{
public:
    double theta0, theta1;

};
