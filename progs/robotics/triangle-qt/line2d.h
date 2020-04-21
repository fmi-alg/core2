#include <math.h>

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

    void intersection(const Line2d& l, double& x, double& y) {
        double c1 = this->c;
        double c2 = l.c;
        double a1 = this->a;
        double a2 = l.a;
        double b1 = this->b;
        double b2 = l.b;
//        y = ( c2*a1 -c1*a2 ) / ( b1*a2 - b2*a1 );
//        x = ( c2*b1 -c1*b2 ) / ( a1*b2 - a2*b1 );
        if((a2*b1-a1*b2) != 0) y = (a1*c2-a2*c1)/(a2*b1-a1*b2);
        else                   y = 0;
        if(a1 != 0) x = (-b1*y-c1)/a1;
        else        x = 0;
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
};
