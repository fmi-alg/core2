#ifndef TRIANGLE
#define TRIANGLE
#include <deque>
#include <vector>
#include "Pose.h"
#include "Wall.h"
#include "triangulate.h"
using std::deque;
using std::vector;

const double area_EPS = 1E-6;

class Triangle {
public:
    // vertices a, b, c in CCW order
    Pose a, b, c;

    Triangle(Pose aa, Pose bb, Pose cc):a(aa), b(bb), c(cc){}
    Triangle(Corner &aa, Corner &bb, Corner &cc):
        a(aa.x, aa.y), b(bb.x, bb.y), c(cc.x, cc.y){}

    double area() {
        return (a.x*(b.y-c.y) - b.x*(a.y-c.y) + c.x*(a.y-b.y))/2.0;
    }

    vector<Triangle> niceDecompose() {
        deque<Corner> v;
        v.emplace_back(a.x, a.y);
        v.emplace_back(b.x, b.y);
        v.emplace_back(c.x, c.y);
        deque<Wall> e;
        e.emplace_back(v[0],v[1]);
        e.emplace_back(v[1],v[2]);
        e.emplace_back(v[2],v[0]);
        deque<pt2line> h(3);
        for (int i = 0; i<3; i++) h[i] = e[i].distance2(0,0);

        // reorder the vertices, find the closes edge/vertex
        if (h[0].dist < h[1].dist && h[0].dist <= h[2].dist) {
            // e[0] or v[0] is closest
        } else if (h[1].dist < h[2].dist && h[1].dist <= h[0].dist) {
            // e[1] or v[1] is closest
            v.push_back(v.front());
            v.pop_front();
            e.push_back(e.front());
            e.pop_front();
            h.push_back(h.front());
            h.pop_front();
        } else {
            // e[2] or v[2] is closest
            v.push_front(v.back());
            v.pop_back();
            e.push_front(e.back());
            e.pop_back();
            h.push_front(h.back());
            h.pop_back();
        }

        deque<Corner> d(3); // closest points to O(0,0) on the edges
        for (int i=0; i<3; i++) {
            double u = h[i].u;
            d[i] = Corner(u*v[(i+1)%3].x + (1-u)*v[i].x, u*v[(i+1)%3].y + (1-u)*v[i].y);
        }

        vector<Triangle> ans;

        Corner O(0,0);
        if (!Triangulate::InsideTriangle(a.x,a.y,b.x,b.y,c.x,c.y,0,0)) {
            // O not in triangle: decomposed into at most 4 triangles
            // 0
            ans.emplace_back(d[0], v[1], d[1]);
            if (ans.back().area() < area_EPS) ans.pop_back();
            // 1
            ans.emplace_back(d[0], d[1], v[2]);
            if (ans.back().area() < area_EPS) ans.pop_back();
            // 2
            ans.emplace_back(d[0], v[2], d[2]);
            if (ans.back().area() < area_EPS) ans.pop_back();
            // 3
            ans.emplace_back(d[0], d[2], v[0]);
            if (ans.back().area() < area_EPS) ans.pop_back();
        }
        else {
            // O in triangle: decomposed into at most 6 triangles
            for (int i=0; i<3; i++) {
                ans.emplace_back(O, v[i], d[i]);
                if (ans.back().area() < area_EPS) ans.pop_back();
                ans.emplace_back(O, d[i], v[(i+1)%3]);
                if (ans.back().area() < area_EPS) ans.pop_back();
            }
        }
        return ans;
    }
};

#endif // TRIANGLE

