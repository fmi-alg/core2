#pragma once
#include <vector>
#include "./geometry/Vector.h"
#include <cmath>

using namespace std;

class Rot3dSide {
 public:
  Vector* origin;
  double width;
  Vector* orientation;
  vector<Rot3dSide*> children;

  Rot3dSide(double x, double y, double z, double width) {
    assert(abs(x) == 1 || abs(y) == 1 || abs(z) == 1);
    origin = new Vector(x, y, z);
    this->width = width;
    if (abs(x) == 1) {
      orientation = new Vector(x, 0, 0);
    } else if (abs(y) == 1) {
      orientation = new Vector(0, y, 0);
    } else {
      orientation = new Vector(0, 0, z);
    }
  }

  static void rot2mat(double x, double y, double z, float mat[16]) {
    // 1. Normalize
    float norm = sqrt(x * x + y * y + z * z);
    x /= norm;
    y /= norm;
    z /= norm;

    // 2. Cross Product with North Pole
    float qx = z;
    float qy = 0;
    float qz = -x;
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
    mat[1] = 2 * (x * y + w * z);
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

  static int sideId(float initRot[3]) {
    for (int i = 0; i < 3; i++) {
      if (abs(initRot[i]) == 1) {
        return i;
      }
    }
    return -1;
  }

  static void interpolateRot(float initRot[3], float nextRot[3], int segId, int segCount, float intRot[3]) {
    int initSideId = sideId(initRot);
    int nextSideId = sideId(nextRot);
    double initSideValue = initRot[initSideId];
    double nextSideValue = nextRot[nextSideId];
    double interpRatio = segId * 1.0 / segCount;
    if (initSideId != nextSideId) {
      double initDist2Edge = abs(nextSideValue - initRot[nextSideId]);
      double nextDist2Edge = abs(initSideValue - nextRot[initSideId]);
      double dist = initDist2Edge + nextDist2Edge;
      if (interpRatio < initDist2Edge / dist) {
        intRot[0] = initRot[0];
        intRot[1] = initRot[1];
        intRot[2] = initRot[2];
        intRot[nextSideId] += nextSideValue * interpRatio * dist;
      } else {
        intRot[0] = nextRot[0];
        intRot[1] = nextRot[1];
        intRot[2] = nextRot[2];
        intRot[initSideId] += initSideValue * (1 - interpRatio) * dist;
      }
    } else {
      intRot[0] = initRot[0];
      intRot[1] = initRot[1];
      intRot[2] = initRot[2];
      intRot[initSideId] += interpRatio * (nextRot[initSideId] - initRot[nextSideId]);
    }
  }

  bool approxEqual(double x, double y) {
    return abs(x - y) < 0.001;
  }

  bool containsPoint(double xx, double yy, double zz) {
    Vector* v = new Vector(xx, yy, zz);
    if (!approxEqual((*orientation) * (*v), 1)) {
      return false;
    }
    double x = origin->x;
    double y = origin->y;
    double z = origin->z;
    return
      !(xx > x + width / 2 || xx < x - width / 2 ||
        yy > y + width / 2 || yy < y - width / 2 ||
        zz > z + width / 2 || zz < z - width / 2);
  }

  bool intervalContains(double x, double xx, double w, double ww) {
    w += 0.0001;
    w /= 2;
    ww += 0.0001;
    ww /= 2;
    return (x + w > xx && x - w < xx) || (xx + ww > x && xx - ww < x);
  }

  bool isAdjacent(Rot3dSide* other) {
    double ori = (*orientation) * (*(other->orientation));
    if (ori == -1) {
      return false;
    } else if (ori == 1) {
      double x = origin->x;
      double y = origin->y;
      double z = origin->z;
      double w = width;
      double xx = other->origin->x;
      double yy = other->origin->y;
      double zz = other->origin->z;
      double ww = other->width;
      if (abs(x) == 1) {
        return
          (approxEqual(abs(y - yy), width + other->width) && intervalContains(z, zz, w, ww)) ||
          (approxEqual(abs(z - zz), width + other->width) && intervalContains(y, yy, w, ww));
      } else if (abs(y) == 1) {
        return
          (approxEqual(abs(x - xx), width + other->width) && intervalContains(z, zz, w, ww)) ||
          (approxEqual(abs(z - zz), width + other->width) && intervalContains(x, xx, w, ww));
      } else {
        return
          (approxEqual(abs(x - xx), width + other->width) && intervalContains(y, yy, w, ww)) ||
          (approxEqual(abs(y - yy), width + other->width) && intervalContains(x, xx, w, ww));
      }
    } else {
      double ori_a[3] = {origin->x, origin->y, origin->z};
      double w_a = width;
      double ori_b[3] = {other->origin->x, other->origin->y, other->origin->z};
      double w_b = other->width;
      double int_a, int_b;
      for (int i = 0; i < 3; i++) {
        if (abs(ori_a[i]) == 1) {
          if (ori_b[i] + w_b != ori_a[i] && ori_b[i] - w_b != ori_a[i]) {
            return false;
          }
        } else if (ori_b[i] == 1) {
          if (ori_a[i] + w_a != ori_b[i] && ori_a[i] - w_a != ori_b[i]) {
            return false;
          }
        } else {
          int_a = ori_a[i];
          int_b = ori_b[i];
        }
      }
      return intervalContains(int_a, int_b, w_a, w_b);
    }
  }

  bool isIdentical(Rot3dSide* other) {
    return origin == other->origin && width == other->width;
  }

  bool split(double epsilon) {
    if (width < epsilon) {
      return false;
    }
    children.clear();
    double x = origin->x;
    double y = origin->y;
    double z = origin->z;
    if (abs(x) == 1) {
      for (int i = 0; i < 2; i++) {
        double ny = y - width / 4 + width / 2 * i;
        for (int j = 0; j < 2; j++) {
          double nz = z - width / 4 + width / 2 * j;
          children.push_back(new Rot3dSide(x, ny, nz, width / 2));
        }
      }
    } else if (abs(y) == 1) {
      for (int i = 0; i < 2; i++) {
        double nx = x - width / 4 + width / 2 * i;
        for (int j = 0; j < 2; j++) {
          double nz = z - width / 4 + width / 2 * j;
          children.push_back(new Rot3dSide(nx, y, nz, width / 2));
        }
      }
    } else {
      for (int i = 0; i < 2; i++) {
        double nx = x - width / 4 + width / 2 * i;
        for (int j = 0; j < 2; j++) {
          double ny = y - width / 4 + width / 2 * j;
          children.push_back(new Rot3dSide(nx, ny, z, width / 2));
        }
      }
    }
    return true;
  }
};
