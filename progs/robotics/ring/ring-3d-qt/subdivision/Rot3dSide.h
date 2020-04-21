#pragma once

#include <vector>
#include <cmath>

#include "CORE/geom3d/point3d.h"
#include "subdivision/Box3d.h"

using namespace std;

extern FILE* g_fptr;

class Rot3dSide {
 public:
  Point3d* origin;
  double width;
  Vector* orientation;
  vector<Rot3dSide*> children;

  Rot3dSide(Point3d p, double width) {
    assert(abs(p.X()) == 1 || abs(p.Y()) == 1 || abs(p.Z()) == 1);
    origin = new Point3d(p);
    this->width = width;
    if (abs(p.X()) == 1) {
      orientation = new Vector(p.X(), 0, 0);
    } else if (abs(p.Y()) == 1) {
      orientation = new Vector(0, p.Y(), 0);
    } else {
      orientation = new Vector(0, 0, p.Z());
    }
  }

  static void mat2angle(float mat[16], double &roll, double &pitch, double &yaw){
    if(mat[0] != 0)
      roll = atan(mat[4]/mat[0]);
    else
      roll = M_PI/2;

    double tmp = sqrt(mat[9]*mat[9]+mat[10]*mat[10]);
    if(tmp > 0)
      pitch = atan(-mat[8]/tmp);
    else
      pitch = M_PI/2;

    if(mat[10] != 0)
      yaw = atan(mat[9]/mat[10]);
    else
      yaw = M_PI/2;
  }

  static void rot2mat(double x, double y, double z, float mat[16]) {
    // 1. Normalize
    float norm = sqrt(x * x + y * y + z * z);
    x /= norm;
    y /= norm;
    z /= norm;

    // 2. Cross Product with North Pole
    float qx = -z;
    float qy = 0;
    float qz = x;
    float qnorm = sqrt(qx * qx + qy * qy + qz * qz);
    if (qx == 0 && qy == 0 && qz == 0) {
      qx = 1;
      qz = 0;
      qnorm = 1;
    }
    qx /= qnorm;
    qy /= qnorm;
    qz /= qnorm;

    // 3. Angle with North Pole
    float theta = acos(y);

    // 4. Get Quaternion
    float c = cos(theta / 2);
    float s = sin(theta / 2);
    float w = c;
    x = s * qx;
    y = s * qy;
    z = s * qz;

    // 5. Get Rotational Matrix
    mat[0] = 1 - 2 * (y * y + z * z);
    mat[1] = 2 * (x * y - w * z);
    mat[2] = 2 * (x * z + w * y);
    mat[3] = 0;

    mat[4] = 2 * (x * y + w * z);
    mat[5] = 1 - 2 * (x * x + z * z);
    mat[6] = 2 * (y * z- w * x);
    mat[7] = 0;

    mat[8] = 2 * (x * z - w * y);
    mat[9] = 2 * (y * z + w * x);
    mat[10] = 1 - 2 * (x * x + y * y);
    mat[11] = 0;

    mat[12] = 0;
    mat[13] = 0;
    mat[14] = 0;
    mat[15] = 1;
  }

  static void cube2sphere(double &x_, double &y_, double &z_) {
    double xx = x_;
    double yy = y_;
    double zz = z_;
    x_ = xx*sqrt(1-yy*yy*0.5f-zz*zz*0.5f+yy*yy*zz*zz/3);
    y_ = yy*sqrt(1-xx*xx*0.5f-zz*zz*0.5f+xx*xx*zz*zz/3);
    z_ = zz*sqrt(1-xx*xx*0.5f-yy*yy*0.5f+xx*xx*yy*yy/3);
  }
  static Point3d cube2sphere(Point3d rot_p) {
    double xx = rot_p.X();
    double yy = rot_p.Y();
    double zz = rot_p.Z();
    return Point3d(xx*sqrt(1-yy*yy*0.5f-zz*zz*0.5f+yy*yy*zz*zz/3),
                   yy*sqrt(1-xx*xx*0.5f-zz*zz*0.5f+xx*xx*zz*zz/3),
                   zz*sqrt(1-xx*xx*0.5f-yy*yy*0.5f+xx*xx*yy*yy/3));
  }


  static int sideId(float initRot[3]) {
    for (int i = 0; i < 3; i++) {
      if (abs(initRot[i]) == 1) {
        return i;
      }
    }
    return -1;
  }

  static void interpolateRot(float curRot[3], float nextRot[3], int segId, float segCount, float resRot[3]) {
    int curSideId = sideId(curRot);
    int nextSideId = sideId(nextRot);
    double curSideValue = curRot[curSideId];
    double nextSideValue = nextRot[nextSideId];
    float cnt = segId;
    double interpRatio = cnt/segCount;
    if (curSideId != nextSideId) {
      double curDist2Edge = abs(nextSideValue - curRot[nextSideId]);
      double nextDist2Edge = abs(curSideValue - nextRot[curSideId]);
      double dist = curDist2Edge + nextDist2Edge;
      if (interpRatio < curDist2Edge / dist) {
        resRot[0] = curRot[0];
        resRot[1] = curRot[1];
        resRot[2] = curRot[2];
        resRot[nextSideId] += nextSideValue * interpRatio * dist;
      } else {
        resRot[0] = nextRot[0];
        resRot[1] = nextRot[1];
        resRot[2] = nextRot[2];
        resRot[curSideId] += curSideValue * (1.0f - interpRatio) * dist;
      }
    } else {
      resRot[0] = curRot[0];
      resRot[1] = curRot[1];
      resRot[2] = curRot[2];
      resRot[curSideId] += interpRatio * (nextRot[curSideId] - curRot[nextSideId]);
    }
  }

  bool approxEqual(double a, double b) {
    return abs(a - b) < 1e-8;
  }

  bool containsPoint(Point3d p) {
    Vector* v = new Vector(p.X(), p.Y(), p.Z());
    if (!approxEqual((*orientation) * (*v), 1)) {
      return false;
    }
    return
      !(p.X() > origin->X() + width / 2 || p.X() < origin->X() - width / 2 ||
        p.Y() > origin->Y() + width / 2 || p.Y() < origin->Y() - width / 2 ||
        p.Z() > origin->Z() + width / 2 || p.Z() < origin->Z() - width / 2);
  }

  bool intervalOverlap(double x, double xx, double w, double ww) {
    w  += 1e-8;
    w  /= 2;
    ww += 1e-8;
    ww /= 2;
    return (x + w > xx && x - w < xx) || (xx + ww > x && xx - ww < x);
  }

  bool intervalContains(double x, double xx, double w, double ww){
    w  /= 2;
    ww += 1e-8;
    ww /= 2;
    return (xx + ww > x+w && xx - ww < x-w);
  }

  double onBoundary(double x, double w){
    return abs(x-w/2) == 1 || abs(x+w/2) == 1;
  }

  bool isAdjacent(Rot3dSide* other) {
    double ori = (*orientation) * (*(other->orientation));
    if (ori == -1) {
      return false;
    } else if (ori == 1) {
      double x = origin->X();
      double y = origin->Y();
      double z = origin->Z();
      double w = width;
      double xx = other->origin->X();
      double yy = other->origin->Y();
      double zz = other->origin->Z();
      double ww = other->width;

      if (abs(x) == 1) {
        return
          (approxEqual(abs(y - yy), width/2 + other->width/2) && intervalOverlap(z, zz, w, ww)) ||
          (approxEqual(abs(z - zz), width/2 + other->width/2) && intervalOverlap(y, yy, w, ww));
      } else if (abs(y) == 1) {
        return
          (approxEqual(abs(x - xx), width/2 + other->width/2) && intervalOverlap(z, zz, w, ww)) ||
          (approxEqual(abs(z - zz), width/2 + other->width/2) && intervalOverlap(x, xx, w, ww));
      } else {
        return
          (approxEqual(abs(x - xx), width/2 + other->width/2) && intervalOverlap(y, yy, w, ww)) ||
          (approxEqual(abs(y - yy), width/2 + other->width/2) && intervalOverlap(x, xx, w, ww));
      }
    } else {
      double ori_a[3] = {origin->X(), origin->Y(), origin->Z()};
      double w_a = width;
      double ori_b[3] = {other->origin->X(), other->origin->Y(), other->origin->Z()};
      double w_b = other->width;
      double int_a, int_b;

      for (int i = 0; i < 3; i++) {
        if (abs(ori_a[i]) == 1) {
          if (ori_b[i] + w_b/2 != ori_a[i] && ori_b[i] - w_b/2 != ori_a[i]) {
            return false;
          }
        } else if (abs(ori_b[i]) == 1) {
          if (ori_a[i] + w_a/2 != ori_b[i] && ori_a[i] - w_a/2 != ori_b[i]) {
            return false;
          }
        } else {
          int_a = ori_a[i];
          int_b = ori_b[i];
        }
      }
      return intervalOverlap(int_a, int_b, w_a, w_b);
    }
  }

  bool isIdentical(Rot3dSide* other) {
    return  (abs(origin->X()-other->origin->X()) < 1e-8) &&
            (abs(origin->Y()-other->origin->Y()) < 1e-8) &&
            (abs(origin->Z()-other->origin->Z()) < 1e-8) &&
            (abs(width-other->width) < 1e-8);
  }

  bool isOverlap(Rot3dSide* other) {
    double ori = (*orientation) * (*(other->orientation));
    // must be same orientation
    if (ori == 1) {
      double x = origin->X();
      double y = origin->Y();
      double z = origin->Z();
      double w = width;
      double xx = other->origin->X();
      double yy = other->origin->Y();
      double zz = other->origin->Z();
      double ww = other->width;

      if (abs(x) == 1) {
        return intervalContains(z, zz, w, ww) || intervalContains(y, yy, w, ww);
      } else if (abs(y) == 1) {
        return intervalContains(z, zz, w, ww) || intervalContains(x, xx, w, ww);
      } else {
        return intervalContains(y, yy, w, ww) || intervalContains(x, xx, w, ww);
      }
    } else {
      return false;
    }
  }

  bool contains(Rot3dSide* other) {
    double ori = (*orientation) * (*(other->orientation));
    // must be same orientation
    if (ori == 1) {
      double x = origin->X();
      double y = origin->Y();
      double z = origin->Z();
      double w = width;
      double xx = other->origin->X();
      double yy = other->origin->Y();
      double zz = other->origin->Z();
      double ww = other->width;

      if (abs(x) == 1) {
        return intervalContains(z, zz, w, ww) && intervalContains(y, yy, w, ww);
      } else if (abs(y) == 1) {
        return intervalContains(z, zz, w, ww) && intervalContains(x, xx, w, ww);
      } else {
        return intervalContains(y, yy, w, ww) && intervalContains(x, xx, w, ww);
      }
    } else {
      return false;
    }
  }

  bool split(double epsilon) {
    //fprintf(g_fptr, "split Br\n");
    if (width < epsilon) {
      //fprintf(g_fptr, "GGG\n");
      return false;
    }
    children.clear();
    double x = origin->X();
    double y = origin->Y();
    double z = origin->Z();
    if (abs(x) == 1) {
      for (int i = 0; i < 2; i++) {
        double ny = y - width / 4 + width / 2 * i;
        for (int j = 0; j < 2; j++) {
          double nz = z - width / 4 + width / 2 * j;
          children.push_back(new Rot3dSide(Point3d(x, ny, nz), width / 2));
        }
      }
    } else if (abs(y) == 1) {
      for (int i = 0; i < 2; i++) {
        double nx = x - width / 4 + width / 2 * i;
        for (int j = 0; j < 2; j++) {
          double nz = z - width / 4 + width / 2 * j;
          children.push_back(new Rot3dSide(Point3d(nx, y, nz), width / 2));
        }
      }
    } else {
      for (int i = 0; i < 2; i++) {
        double nx = x - width / 4 + width / 2 * i;
        for (int j = 0; j < 2; j++) {
          double ny = y - width / 4 + width / 2 * j;
          children.push_back(new Rot3dSide(Point3d(nx, ny, z), width / 2));
        }
      }
    }
    return true;
  }
};
