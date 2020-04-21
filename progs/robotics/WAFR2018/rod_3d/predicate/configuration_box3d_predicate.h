/*
 * configuration_box3d_predicate.h
 *
 * Created on: June 30, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 * Description:
 *  classification
 *  check collision
 *  find clearance (distance to the nearest feature)
 *
 *  Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#ifndef CONFIGURATION_BOX3D_PREDICATE_H
#define CONFIGURATION_BOX3D_PREDICATE_H

#include "subdivision/configuration_box3d.h"
#include "subdivision/status.h"

class ConfBox3d;

class ConfBox3dPredicate {
 public:
  Status classification(ConfBox3d* box, Point3d& closest_point);

  void checkCollisionFeatureSet(ConfBox3d* box, ConfBox3d* parent);
  void checkVoronoiFeatureSet(ConfBox3d* box, ConfBox3d* parent);

  double findCleanrance(ConfBox3d* box, Segment3d& rod);
  void findVorCleanrance(ConfBox3d* box, Segment3d& rod);
};

#endif  // CONFIGURATION_BOX3D_PREDICATE_H
