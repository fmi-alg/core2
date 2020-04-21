#pragma once

#include <math.h>
#include "Corner.h"
#include "Wall.h"


class Line2d
{
public:
    double a = 0, b = 0, c = 0;
    double srcx = 0, srcy = 0, dstx = 0, dsty = 0;  //original two points, before expand

    // ax+by+c=0
    // vector (a,b) is the normal vector, on the right side of the line
    Line2d() {}
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

    // shift the line to its right by distance d
    bool shiftRight(double d) {
        c -= d;
        srcx += a*d;
        srcy += b*d;
        dstx += a*d;
        dsty += b*d;
        return true;
    }

    // shift the line to its left by distance d
    bool shiftLeft(double d) {
        c += d;
        srcx -= a*d;
        srcy -= b*d;
        dstx -= a*d;
        dsty -= b*d;
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
        // check the intersection with line l, output the intersection to (x, y) if they intersect
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

    bool isRight(const Corner &c)
    {
        return isRight(c.x, c.y);
    }

    bool isRight(double x, double y)
    {
        return a*x + b*y + c > 0;
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
        return ( CCW(x3, y3, x4, y4, x1, y1) != CCW(x3, y3, x4, y4, x2, y2) )
            && ( CCW(x1, y1, x2, y2, x3, y3) != CCW(x1, y1, x2, y2, x4, y4) );
    }

    static bool lineSegIntsct(Wall & A, Wall & B ) {
      return lineSegIntsct(A.src->x, A.src->y, A.dst->x, A.dst->y,
                           B.src->x, B.src->y, B.dst->x, B.dst->y );
    }
};

