#include "BasisShape.h"

bool BasisShape::isIntersect(Corner &c) {
    bool ans;
    if (this->degree == 2) {
        // 2-basis shape is union of a list of 1-basis shapes, use OR for all shapes in the list
        ans = false;
        for (BasisShape &x: this->list) {
            ans = ans || x.isIntersect(c);
        }
    }
    else if (this->degree == 1) {
        // 1-basis shape is union of a list of 0-basis shapes, use AND for all shapes in the list
        ans = true;
        for (BasisShape &x: this->list) {
            ans = ans && x.isIntersect(c);
        }
    }
    else {
        // 0-basis shape is either a halfspace, a disk or the complement of a disk
        if (this->circ.r == 0) {
            // half-space
            ans = !this->line.isRight(c);
        }
        else if (this->isComp) {
            // complement of a disk
            ans = this->circ.inDiskComplement(c);
        }
        else {
            // disk
            ans = this->circ.inDisk(c);
        }
    }
    return ans;
}

bool BasisShape::isIntersect(Wall &w) {
    bool ans;
    if (this->degree == 2) {
        // 2-basis shape is union of a list of 1-basis shapes, use OR for all shapes in the list
        ans = false;
        for (BasisShape &x: this->list) {
            ans = ans || x.isIntersect(w);
        }
    }
    else if (this->degree == 1) {
        // 1-basis shape is union of a list of 0-basis shapes
        // Need to modify the wall every time it intersects
        // Make sure the inner circle (complement of disk / concave shape) is the only one and last one

        Corner srcnew = *w.src;
        Corner dstnew = *w.dst;
        Wall w_new(&srcnew, &dstnew);
        Line2d lwall(srcnew, dstnew);
        ans = true;
        for (unsigned int i = 0; i < this->list.size(); i++) {
            ans = ans && this->list[i].isIntersect(w_new);
            if (i < list.size() - 1) {
                // chope the wall(line segment) by the shape, computed the intersected part
                this->list[i].chope(w_new, lwall);
            }
        }
    }
    else {
        // 0-basis shape is either a halfspace, a disk or the complement of a disk
        if (this->circ.r == 0) {
            // half-space
            ans = !(this->line.isRight(*w.src) && this->line.isRight(*w.dst));
        }
        else if (this->isComp) {
            // complement of a disk
            ans = w.distance(this->circ.x, this->circ.y) >= this->circ.r;
        }
        else {
            // disk
            ans = w.distance(this->circ.x, this->circ.y) <= this->circ.r;
        }
    }
    return ans;
}

void BasisShape::chope(Wall &w, Line2d &lwall) {
    // chope the wall by a half-space or a disk
    if (this->degree != 0) return;
    if (this->circ.r == 0) {
        // half-space
        if (!(this->line.isRight(*w.src)) && !(this->line.isRight(*w.dst))) {
            // fully interesected
            // do nothing
        } else {
            Corner inter;
            this->line.intersection(lwall, inter);
            if (this->line.isRight(*w.src)) {
                *w.src = inter;
            }
            else *w.dst = inter;
        }
    }
    else if (this->isComp) {
        // complement of a disk
        return;
    }
    else {
        // disk
        Corner inter_1;
        Corner inter_2;
        this->circ.intersectLine(lwall, inter_1, inter_2);
        if (w.isOnSegment(inter_1)) *w.dst = inter_1;
        if (w.isOnSegment(inter_2)) *w.src = inter_2;
    }
}
