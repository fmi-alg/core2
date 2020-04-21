#include <math.h>
#include "Corner.h"
#include "Wall.h"


class Line2d
{
public:
    double a, b, c;
    double srcx, srcy, dstx, dsty;  //original two points, before expand

    // ax+by+c=0
    Line2d(const double& x1, const double& y1, const double& x2, const double& y2):srcx(x1), srcy(y1), dstx(x2), dsty(y2)
    {
        a = y2 - y1;
        b = x1 - x2;
        c = x2*y1 - x1*y2;

        double n = sqrt(a*a + b*b);
        a = a / n;
        b = b / n;
        c = c / n;
    }
    Line2d(const Corner& c1, const Corner& c2): Line2d(c1.x, c1.y, c2.x, c2.y)
    {
    }


//	void expand(double d)
//	{
//		//assert(d >= 0);
//		if (c > 0)
//		{
//			c += d;
//		}
//		else
//		{
//			c -= d;
//		}
//	}

    // shift the line to its right by distance d
    bool shiftRight(double d) {
        c-=d;
        return true;
    }

    // shift the line to its left by distance d
    bool shiftLeft(double d) {
        c+=d;
        return true;
    }


    //expand L1 based on position to L2, L3
    //we want L1 to "go away" from L2 x L3
    bool expand(double d, Line2d& l2, Line2d& l3)
    {
        double x, y;
        l2.intersection(l3, x, y);
        //negative triangle, return
        if (this->isRight(x, y)) {
            return false;
        }

//        // ax+by+c1=0
//        double c1 = 2*c+sqrt(4*c*c-4*1*(c*c-(a*a+b*b)*d*d));
//        // ax+by+c2=0
//        double c2 = 2*c-sqrt(4*c*c-4*1*(c*c-(a*a+b*b)*d*d));

//        double d1 = fabs(a*x+b*y+c1)/sqrt(a*a+b*b);
//        double d2 = fabs(a*x+b*y+c2)/sqrt(a*a+b*b);

//        if(shrink) {
//            if(d1 < d2) c = c1;
//            else        c = c2;
//        } else {
//            if(d1 > d2) c = c1;
//            else        c = c2;
//        }

        //sc is the c for line parallel but go through intersection of L2 and L3
        double sc = -this->a * x - this->b * y;

        double n = sqrt(a*a + b*b);

        if (c > sc) c += d * n;
        else        c -= d * n;

        return true;
    }

    // we have two lines parallel to L1
    // pick the one which is farrer from the center(x, y)
    void expand2(double d) {
        // ax+by+c1=0
        double c1 = c+sqrt(c*c-(c*c-(a*a+b*b)*d*d));
        // ax+by+c2=0
        double c2 = c-sqrt(c*c-(c*c-(a*a+b*b)*d*d));

        double d1 = fabs(c1);
        double d2 = fabs(c2);

        if(d1 > d2) c = c1;
        else        c = c2;
    }

    bool oppositeSide(double x1, double y1, double x2, double y2) {
        double v1 = a*x1+b*y1+c;
        double v2 = a*x2+b*y2+c;
        return (v1*v2 < 0);
    }

    bool isNegative(Line2d& l2, Line2d& l3) {
        double x, y;
        l2.intersection(l3, x, y);
        if (this->isRight(x, y))
            return true;
        return false;
    }

    bool intersection(const Line2d& l, double& x, double& y) {
        double c1 = this->c;
        double c2 = l.c;
        double a1 = this->a;
        double a2 = l.a;
        double b1 = this->b;
        double b2 = l.b;

        if (a2*b1-a1*b2 == 0) {
        // the two line are parallel
          return false;
        }
        y = ( c2*a1 -c1*a2 ) / ( b1*a2 - b2*a1 );
        x = ( c2*b1 -c1*b2 ) / ( a1*b2 - a2*b1 );
        return true;
    }

    bool intersection(const Line2d& l, Corner& c) {
        return intersection(l, c.x, c.y);
    }

    bool isRight(double x, double y)
    {
        return isRight(-c/a, 0, dstx-srcx-c/a, dsty-srcy, x, y);
    }

    static bool isRight(double x1, double y1, double x2, double y2, double x, double y)
    {
        return (x2-x1)*(y-y1) - (y2-y1)*(x-x1) < 0;
    }

    static bool CCW(double x1, double y1, double x2, double y2, double x, double y)
    {
        return !isRight(x1, y1, x2, y2, x, y);
    }

    static bool lineSegIntsct(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
//        Line2d L1(x1, y1, x2, y2);
//        Line2d L2(x3, y3, x4, y4);
//        double x, y;
//        L1.intersection(L2, x, y);
//        if ( (x1 < x && x < x2) || (x2 < x && x < x1) )
//        {
//            return true;
//        }
//        return false;
        return ( CCW(x3, y3, x4, y4, x1, y1) != CCW(x3, y3, x4, y4, x2, y2) )
            && ( CCW(x1, y1, x2, y2, x3, y3) != CCW(x1, y1, x2, y2, x4, y4) );
//        Line2d L1(x1, y1, x2, y2);
//        Line2d L2(x3, y3, x4, y4);

//        double v1 = L2.a*x1+L2.b*y1+L2.c;
//        double v2 = L2.a*x2+L2.b*y2+L2.c;
//        double v3 = L1.a*x3+L1.b*y3+L1.c;
//        double v4 = L1.a*x4+L1.b*y4+L1.c;

//        return ((v1*v2 <= 0) && (v3*v4 <= 0));
    }

    static bool lineSegIntsct(Wall & A, Wall & B ) {
      return lineSegIntsct(A.src->x, A.src->y, A.dst->x, A.dst->y,
                           B.src->x, B.src->y, B.dst->x, B.dst->y );
    }
};

class Circle
{
   const double EPS = 1E-6;
public:
    double x, y;
    double r;
    Circle (double xx, double yy, double rr):x(xx), y(yy), r(rr) {}

    // Check if circle intersects a line
    // 	(and returns 2 intersection points if true)
    //
    bool intersectLine(Line2d & L, double &inter_x1, double &inter_y1,
                                   double &inter_x2, double &inter_y2) {
      double dist = L.a*x + L.b*y + L.c;
      if (abs(dist) > r + EPS) return false;
      if (abs(dist) > r-EPS && abs(dist) < r + EPS) {
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
};

class Arc : Circle
{
public:
    double theta0, theta1;

};
