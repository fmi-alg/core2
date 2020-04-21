/*
 * rot3d.h
 *
 * Created on: July 1, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#ifndef ROT3D_H
#define ROT3D_H

#include <cmath>
#include <vector>

#include "CORE/geom3d/point3d.h"
#include "subdivision/Box3d.h"

extern double EPS;

class Rot3d {
 public:
  Point3d* origin;
  double width;
  Vector* orientation;
  std::vector<Rot3d*> children;

  Rot3d(Point3d p, double width) {
    assert(1 - fabs(p.X()) < EPS || 1 - fabs(p.Y()) < EPS ||
           1 - fabs(p.Z()) < EPS);
    origin = new Point3d(p);
    this->width = width;
    if (1 - fabs(p.X()) < EPS) {
      orientation = new Vector(p.X(), 0, 0);
    } else if (1 - fabs(p.Y()) < EPS) {
      orientation = new Vector(0, p.Y(), 0);
    } else {
      orientation = new Vector(0, 0, p.Z());
    }
  }

  static void mat2angle(float mat[16], double& roll, double& pitch,
                        double& yaw) {
    if (mat[0] != 0)
      roll = atan(mat[4] / mat[0]);
    else
      roll = M_PI / 2;

    double tmp = hypot(mat[9], mat[10]);
    if (tmp > 0)
      pitch = atan(-mat[8] / tmp);
    else
      pitch = M_PI / 2;

    if (mat[10] != 0)
      yaw = atan(mat[9] / mat[10]);
    else
      yaw = M_PI / 2;
  }

  static void rot2mat(double x, double y, double z, float mat[16]) {
    // 1. Normalize
    cube2sphere(x, y, z);

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
    mat[6] = 2 * (y * z - w * x);
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

  static void cube2sphere(double& x, double& y, double& z) {
    double xx = x;
    double yy = y;
    double zz = z;
    x = xx *
        sqrt(1.0 - yy * yy * 0.5 - zz * zz * 0.5 + yy * yy * zz * zz / 3.0);
    y = yy *
        sqrt(1.0 - xx * xx * 0.5 - zz * zz * 0.5 + xx * xx * zz * zz / 3.0);
    z = zz *
        sqrt(1.0 - xx * xx * 0.5 - yy * yy * 0.5 + xx * xx * yy * yy / 3.0);
  }
  static Point3d cube2sphere(Point3d rotation_point) {
    double xx = rotation_point.X();
    double yy = rotation_point.Y();
    double zz = rotation_point.Z();
    return Point3d(xx * sqrt(1.0 - yy * yy * 0.5 - zz * zz * 0.5 +
                             yy * yy * zz * zz / 3.0),
                   yy * sqrt(1.0 - xx * xx * 0.5 - zz * zz * 0.5 +
                             xx * xx * zz * zz / 3.0),
                   zz * sqrt(1.0 - xx * xx * 0.5 - yy * yy * 0.5 +
                             xx * xx * yy * yy / 3.0));
  }

  static int side_id(float init_rot[3]) {
    for (int i = 0; i < 3; i++) {
      if (1 - fabs(init_rot[i]) < EPS) {
        return i;
      }
    }
    return -1;
  }

  static void interpolateRot(float cur_rot[3], float next_rot[3], int seg_id,
                             float seg_count, float res_rot[3]) {
    int cur_side_id = side_id(cur_rot);
    int next_side_id = side_id(next_rot);
    double cur_side_value = cur_rot[cur_side_id];
    double next_side_value = next_rot[next_side_id];
    float cnt = seg_id;
    double interp_ratio = cnt / seg_count;
    if (cur_side_id != next_side_id) {
      double cur_dist_edge = fabs(next_side_value - cur_rot[next_side_id]);
      double next_dist_edge = fabs(cur_side_value - next_rot[cur_side_id]);
      double dist = cur_dist_edge + next_dist_edge;
      if (interp_ratio < cur_dist_edge / dist) {
        res_rot[0] = cur_rot[0];
        res_rot[1] = cur_rot[1];
        res_rot[2] = cur_rot[2];
        res_rot[next_side_id] += next_side_value * interp_ratio * dist;
      } else {
        res_rot[0] = next_rot[0];
        res_rot[1] = next_rot[1];
        res_rot[2] = next_rot[2];
        res_rot[cur_side_id] += cur_side_value * (1.0f - interp_ratio) * dist;
      }
    } else {
      res_rot[0] = cur_rot[0];
      res_rot[1] = cur_rot[1];
      res_rot[2] = cur_rot[2];
      res_rot[cur_side_id] +=
          interp_ratio * (next_rot[cur_side_id] - cur_rot[next_side_id]);
    }
  }

  bool approxEqual(double a, double b) { return fabs(a - b) < EPS; }

  bool containsPoint(Point3d p) {
    Vector* v = new Vector(p.X(), p.Y(), p.Z());
    if (!approxEqual((*orientation) * (*v), 1)) {
      return false;
    }
    return !(p.X() > origin->X() + width / 2.0 ||
             p.X() < origin->X() - width / 2.0 ||
             p.Y() > origin->Y() + width / 2.0 ||
             p.Y() < origin->Y() - width / 2.0 ||
             p.Z() > origin->Z() + width / 2.0 ||
             p.Z() < origin->Z() - width / 2.0);
  }

  bool intervalOverlap(double x, double xx, double w, double ww) {
    w /= 2.0;
    ww += EPS;
    ww /= 2.0;
    return (xx + ww > x - w && xx - ww < x - w) ||
           (xx + ww > x + w && xx - ww < x + w);
  }

  bool intervalContains(double x, double xx, double w, double ww) {
    w /= 2.0;
    ww += EPS;
    ww /= 2.0;
    return (xx + ww >= x + w && xx - ww <= x - w);
  }

  double onBoundary(double x, double w) {
    return 1 - fabs(x - w / 2) < EPS || 1 - fabs(x + w / 2) < EPS;
  }

  bool isAdjacent(Rot3d* other) {
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

      if (1 - fabs(x) < EPS) {
        return (approxEqual(fabs(y - yy), width / 2.0 + other->width / 2.0) &&
                intervalOverlap(z, zz, w, ww)) ||
               (approxEqual(fabs(z - zz), width / 2.0 + other->width / 2.0) &&
                intervalOverlap(y, yy, w, ww)) ||
               (intervalOverlap(y, yy, w, ww) && intervalOverlap(z, zz, w, ww));
      } else if (1 - fabs(y) < EPS) {
        return (approxEqual(fabs(x - xx), width / 2.0 + other->width / 2.0) &&
                intervalOverlap(z, zz, w, ww)) ||
               (approxEqual(fabs(z - zz), width / 2.0 + other->width / 2.0) &&
                intervalOverlap(x, xx, w, ww)) ||
               (intervalOverlap(z, zz, w, ww) && intervalOverlap(x, xx, w, ww));
      } else {
        return (approxEqual(fabs(x - xx), width / 2.0 + other->width / 2.0) &&
                intervalOverlap(y, yy, w, ww)) ||
               (approxEqual(fabs(y - yy), width / 2.0 + other->width / 2.0) &&
                intervalOverlap(x, xx, w, ww)) ||
               (intervalOverlap(x, xx, w, ww) && intervalOverlap(y, yy, w, ww));
      }
    } else {
      double ori_a[3] = {origin->X(), origin->Y(), origin->Z()};
      double w_a = width;
      double ori_b[3] = {other->origin->X(), other->origin->Y(),
                         other->origin->Z()};
      double w_b = other->width;
      double int_a, int_b;

      for (int i = 0; i < 3; i++) {
        if (1 - fabs(ori_a[i]) < EPS) {
          if (ori_b[i] + w_b / 2.0 != ori_a[i] &&
              ori_b[i] - w_b / 2.0 != ori_a[i]) {
            return false;
          }
        } else if (1 - fabs(ori_b[i]) < EPS) {
          if (ori_a[i] + w_a / 2.0 != ori_b[i] &&
              ori_a[i] - w_a / 2.0 != ori_b[i]) {
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

  bool isIdentical(Rot3d* other) {
    return approxEqual(origin->X(), other->origin->X()) &&
           approxEqual(origin->Y(), other->origin->Y()) &&
           approxEqual(origin->Z(), other->origin->Z()) &&
           approxEqual(width, other->width);
  }

  bool isOverlap(Rot3d* other) {
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

      if (1 - fabs(x) < EPS) {
        return intervalContains(y, yy, w, ww) || intervalContains(z, zz, w, ww);
      } else if (1 - fabs(y) < EPS) {
        return intervalContains(z, zz, w, ww) || intervalContains(x, xx, w, ww);
      } else {
        return intervalContains(y, yy, w, ww) || intervalContains(x, xx, w, ww);
      }
    } else {
      return false;
    }
  }

  bool contains(Rot3d* other) {
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

      if (1 - fabs(x) < EPS) {
        return intervalContains(y, yy, w, ww) && intervalContains(z, zz, w, ww);
      } else if (1 - fabs(y) < EPS) {
        return intervalContains(z, zz, w, ww) && intervalContains(x, xx, w, ww);
      } else {
        return intervalContains(y, yy, w, ww) && intervalContains(x, xx, w, ww);
      }
    } else {
      return false;
    }
  }

  bool split(double epsilon) {
    if (width < epsilon) {
      return false;
    }
    children.clear();
    double x = origin->X();
    double y = origin->Y();
    double z = origin->Z();
    if (1 - fabs(x) < EPS) {
      for (double i = 0; i < 2; i++) {
        double ny = y - width / 4.0 + width / 2.0 * i;
        for (double j = 0; j < 2; j++) {
          double nz = z - width / 4.0 + width / 2.0 * j;
          children.push_back(new Rot3d(Point3d(x, ny, nz), width / 2.0));
        }
      }
    } else if (1 - fabs(y) < EPS) {
      for (double i = 0; i < 2; i++) {
        double nx = x - width / 4.0 + width / 2.0 * i;
        for (double j = 0; j < 2; j++) {
          double nz = z - width / 4.0 + width / 2.0 * j;
          children.push_back(new Rot3d(Point3d(nx, y, nz), width / 2.0));
        }
      }
    } else {
      for (double i = 0; i < 2; i++) {
        double nx = x - width / 4.0 + width / 2.0 * i;
        for (double j = 0; j < 2; j++) {
          double ny = y - width / 4.0 + width / 2.0 * j;
          children.push_back(new Rot3d(Point3d(nx, ny, z), width / 2.0));
        }
      }
    }
    return true;
  }
};

#endif  // ROT3D_H
