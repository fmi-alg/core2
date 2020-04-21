/*
 * configuration_box3d_predicate.cpp
 *
 * Created on: July 1, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#include "predicate/configuration_box3d_predicate.h"

extern double EPS;
extern FILE* debug_ptr;
extern bool verbose;

// Find the nearest feature to the rod, and check the feature's convexity based
// on the closest point on the rod
Status ConfBox3dPredicate::classification(ConfBox3d* parent,
                                          Point3d& closest_point) {
  if (verbose) fprintf(debug_ptr, "classification\n");

  // find the closest point on the feature
  Feature* nearest_feature = nullptr;
  double min_dist = std::numeric_limits<float>::max();
  // find the closest feature to the rod
  for (const auto& corner : parent->corners) {
    if (verbose) fprintf(debug_ptr, "parent corner\n");
    if (verbose)
      fprintf(debug_ptr, "%f %f %f\n", corner->point().X(), corner->point().Y(),
              corner->point().Z());
    double dist = corner->point().distance(closest_point);
    if (dist < min_dist) {
      nearest_feature = corner;
      min_dist = dist;
    }
  }
  for (const auto& edge : parent->edges) {
    if (verbose) fprintf(debug_ptr, "parent edge\n");
    if (verbose)
      fprintf(debug_ptr, "%f %f %f\n", edge->a->point().X(),
              edge->a->point().Y(), edge->a->point().Z());
    if (verbose)
      fprintf(debug_ptr, "%f %f %f\n", edge->b->point().X(),
              edge->b->point().Y(), edge->b->point().Z());
    double dist = edge->segment().distance(closest_point);
    if (dist < min_dist) {
      nearest_feature = edge;
      min_dist = dist;
    }
  }
  for (const auto& wall : parent->walls) {
    if (verbose) fprintf(debug_ptr, "parent wall\n");
    if (verbose)
      fprintf(debug_ptr, "%f %f %f\n", wall->a->point().X(),
              wall->a->point().Y(), wall->a->point().Z());
    if (verbose)
      fprintf(debug_ptr, "%f %f %f\n", wall->b->point().X(),
              wall->b->point().Y(), wall->b->point().Z());
    if (verbose)
      fprintf(debug_ptr, "%f %f %f\n", wall->c->point().X(),
              wall->c->point().Y(), wall->c->point().Z());
    double dist = wall->triangle().distance(closest_point);
    if (dist < min_dist) {
      nearest_feature = wall;
      min_dist = dist;
    }
  }

  if (verbose)
    fprintf(debug_ptr, "closest point %f %f %f  feature %d %d %d\n",
            closest_point.X(), closest_point.Y(), closest_point.Z(),
            parent->corners.size(), parent->edges.size(), parent->walls.size());

  bool is_free = false;

  //// Box_t
  // if the nearest feature is a wall...
  // check the orientation of m(B) with respect to the wall
  if (nearest_feature != nullptr) {
    if (verbose) fprintf(debug_ptr, "find nearest\n");
    if (nearest_feature->type() == TYPE_WALL) {
      if (verbose) fprintf(debug_ptr, "nearest wall\n");
      if (verbose)
        fprintf(debug_ptr, "%f %f %f\n",
                static_cast<Wall*>(nearest_feature)->a->point().X(),
                static_cast<Wall*>(nearest_feature)->a->point().Y(),
                static_cast<Wall*>(nearest_feature)->a->point().Z());
      if (verbose)
        fprintf(debug_ptr, "%f %f %f\n",
                static_cast<Wall*>(nearest_feature)->b->point().X(),
                static_cast<Wall*>(nearest_feature)->b->point().Y(),
                static_cast<Wall*>(nearest_feature)->b->point().Z());
      if (verbose)
        fprintf(debug_ptr, "%f %f %f\n",
                static_cast<Wall*>(nearest_feature)->c->point().X(),
                static_cast<Wall*>(nearest_feature)->c->point().Y(),
                static_cast<Wall*>(nearest_feature)->c->point().Z());
      if (static_cast<Wall*>(nearest_feature)
              ->triangle()
              .toPlane()
              .apply(closest_point) >= 0) {
        is_free = true;
      }
    } else if (nearest_feature->type() == TYPE_EDGE) {
      if (verbose)
        fprintf(debug_ptr, "nearest edge %d\n",
                static_cast<Edge*>(nearest_feature)->convex());
      if (verbose)
        fprintf(debug_ptr, "%f %f %f\n",
                static_cast<Edge*>(nearest_feature)->a->point().X(),
                static_cast<Edge*>(nearest_feature)->a->point().Y(),
                static_cast<Edge*>(nearest_feature)->a->point().Z());
      if (verbose)
        fprintf(debug_ptr, "%f %f %f\n",
                static_cast<Edge*>(nearest_feature)->b->point().X(),
                static_cast<Edge*>(nearest_feature)->b->point().Y(),
                static_cast<Edge*>(nearest_feature)->b->point().Z());
      if (verbose)
        fprintf(
            debug_ptr, "right wall %f %f %f  %f %f %f  %f %f %f\n",
            static_cast<Edge*>(nearest_feature)->right_wall->a->point().X(),
            static_cast<Edge*>(nearest_feature)->right_wall->a->point().Y(),
            static_cast<Edge*>(nearest_feature)->right_wall->a->point().Z(),
            static_cast<Edge*>(nearest_feature)->right_wall->b->point().X(),
            static_cast<Edge*>(nearest_feature)->right_wall->b->point().Y(),
            static_cast<Edge*>(nearest_feature)->right_wall->b->point().Z(),
            static_cast<Edge*>(nearest_feature)->right_wall->c->point().X(),
            static_cast<Edge*>(nearest_feature)->right_wall->c->point().Y(),
            static_cast<Edge*>(nearest_feature)->right_wall->c->point().Z());
      if (verbose)
        fprintf(
            debug_ptr, "opposite wall %f %f %f  %f %f %f  %f %f %f\n",
            static_cast<Edge*>(nearest_feature)->opposite_wall->a->point().X(),
            static_cast<Edge*>(nearest_feature)->opposite_wall->a->point().Y(),
            static_cast<Edge*>(nearest_feature)->opposite_wall->a->point().Z(),
            static_cast<Edge*>(nearest_feature)->opposite_wall->b->point().X(),
            static_cast<Edge*>(nearest_feature)->opposite_wall->b->point().Y(),
            static_cast<Edge*>(nearest_feature)->opposite_wall->b->point().Z(),
            static_cast<Edge*>(nearest_feature)->opposite_wall->c->point().X(),
            static_cast<Edge*>(nearest_feature)->opposite_wall->c->point().Y(),
            static_cast<Edge*>(nearest_feature)->opposite_wall->c->point().Z());
      if (/*static_cast<Edge*>(nearest_feature)->convex()*/ static_cast<Edge*>(
              nearest_feature)
                  ->right_wall->triangle()
                  .toPlane()
                  .apply(closest_point) >= 0 ||
          static_cast<Edge*>(nearest_feature)
                  ->opposite_wall->triangle()
                  .toPlane()
                  .apply(closest_point) >= 0) {
        is_free = true;
      }
    } else {
      if (verbose) fprintf(debug_ptr, "nearest corner\n");
      if (verbose)
        fprintf(debug_ptr, "%f %f %f\n",
                static_cast<Corner*>(nearest_feature)->point().X(),
                static_cast<Corner*>(nearest_feature)->point().Y(),
                static_cast<Corner*>(nearest_feature)->point().Z());
      int convex_cnt = 0, concave_cnt = 0;
      for (const auto& pre_edge :
           static_cast<Corner*>(nearest_feature)->pre_edge) {
        if (pre_edge->convex())
          ++convex_cnt;
        else
          ++concave_cnt;
      }
      if (concave_cnt == 0) {  // convex corner
        is_free = true;
      } else if (convex_cnt > 0 && concave_cnt > 0) {  // mixed corner
        Wall* wall = nullptr;
        double dot_v = -2;
        Vector point = Vector(closest_point -
                              static_cast<Corner*>(nearest_feature)->point());
        point.normalize();
        for (const auto& incident_wall :
             static_cast<Corner*>(nearest_feature)->incident_wall) {
          // minimam angle implies that dot product has maximam value
          double v = point * incident_wall->triangle().normal();
          if (dot_v < v) {
            dot_v = v;
            wall = incident_wall;
          }
        }
        if (wall != nullptr) {
          if (wall->triangle().toPlane().apply(closest_point) >= 0) {
            is_free = true;
          }
        }
      }
      // else concave corner
      // is_free remains the same value
    }
  }
  // else, the nearest feature is either an edge or corner, and the box must
  // be free (since all obstacles are convex)
  else
    is_free = true;

  if (verbose) {
    if (!is_free)
      fprintf(debug_ptr, "STUCK\n");
    else
      fprintf(debug_ptr, "FREE\n");
  }

  return is_free ? FREE : STUCK;
}

// TODO: remove adding feature in split (insert)
void ConfBox3dPredicate::checkCollisionFeatureSet(ConfBox3d* box,
                                                  ConfBox3d* parent) {
  if (box->status != UNKNOWN) return;

  double outer_distance = ConfBox3d::rod_length + box->rB;

  // Translational and ratational box
  if (box->rotation_width != -2) {
    // CHECK: the creation of (intersection of ball and cone)
    // Do PI 1 intersection
    // 1) 4 half space by the box_rotation
    // 2) a pseudo base half space
    // 3) a ball with radius l0+rB: outer_distance

    Point3d rotation_point[5];
    double du[4] = {-1, 1, 1, -1};
    double dv[4] = {-1, -1, 1, 1};
    for (int i = 0; i < 4; ++i) {
      if (1 - fabs(box->rotation_center.X()) < EPS) {
        rotation_point[i].set(
            box->rotation_center.X(),
            box->rotation_center.Y() + du[i] * box->rotation_width / 2,
            box->rotation_center.Z() + dv[i] * box->rotation_width / 2);
      } else if (1 - fabs(box->rotation_center.Y()) < EPS) {
        rotation_point[i].set(
            box->rotation_center.X() + du[i] * box->rotation_width / 2,
            box->rotation_center.Y(),
            box->rotation_center.Z() + dv[i] * box->rotation_width / 2);
      } else {
        rotation_point[i].set(
            box->rotation_center.X() + du[i] * box->rotation_width / 2,
            box->rotation_center.Y() + dv[i] * box->rotation_width / 2,
            box->rotation_center.Z());
      }
      rotation_point[i] = Rot3d::cube2sphere(rotation_point[i]);
    }
    rotation_point[4] = rotation_point[0];

    // 1)
    std::vector<Plane3d> half_spaces;
    for (int i = 0; i < 4; ++i) {
      Triangle3d triangle, Triangle;
      if (box->rotation_center.X() == 1 || box->rotation_center.Y() == -1 ||
          box->rotation_center.Z() == 1) {
        // center at abstract box_translation
        triangle = Triangle3d(rotation_point[i], Point3d(0, 0, 0),
                              rotation_point[i + 1]);
        triangle.normalize();
        // transfer to physical space
        Triangle = Triangle3d(
            box->center + rotation_point[i] + triangle.normal() * box->rB,
            box->center + triangle.normal() * box->rB,
            box->center + rotation_point[i + 1] + triangle.normal() * box->rB);
      }
      if (box->rotation_center.X() == -1 || box->rotation_center.Y() == 1 ||
          box->rotation_center.Z() == -1) {
        // center at abstract box_translation
        triangle = Triangle3d(Point3d(0, 0, 0), rotation_point[i],
                              rotation_point[i + 1]);
        triangle.normalize();
        // transfer to physical space
        Triangle = Triangle3d(
            box->center + triangle.normal() * box->rB,
            box->center + rotation_point[i] + triangle.normal() * box->rB,
            box->center + rotation_point[i + 1] + triangle.normal() * box->rB);
      }
      half_spaces.emplace_back(Triangle.toPlane());
    }

    // 2) only 6 cases:
    // Dx=(+,0,0), -Dx=(-,0,0),
    // Dy=(0,+,0), -Dy=(0,-,0),
    // Dz=(0,0,+), -Dz=(0,0,-)
    Point3d rotation_center_on_sphere =
        Rot3d::cube2sphere(Point3d(std::floor(box->rotation_center.X()),
                                   std::floor(box->rotation_center.Y()),
                                   std::floor(box->rotation_center.Z())));
    Vector n(rotation_center_on_sphere.X(), rotation_center_on_sphere.Y(),
             rotation_center_on_sphere.Z());
    n.normalize();
    Point3d bottom_H(box->center.X() - n.X() * box->rB,
                     box->center.Y() - n.Y() * box->rB,
                     box->center.Z() - n.Z() * box->rB);
    half_spaces.emplace_back(bottom_H, -n);

    // test the corner is inside the ball(l0+rB) and the 5 half space
    for (const auto& corner : parent->corners) {
      if (corner->point().intersectHalfspaces(
              half_spaces, box->center, outer_distance)) {
        box->corners.push_back(corner);
        box->status = MIXED;
      }
    }

    // test the edge is inside the ball(l0+rB) and the 5 half space
    for (const auto& edge : parent->edges) {
      if (edge->segment().intersectHalfspaces(
              half_spaces, box->center,
              Rot3d::cube2sphere(box->rotation_center), outer_distance)) {
        box->edges.push_back(edge);
        box->status = MIXED;
      }
    }

    // test the wall is inside the ball(l0+rB) and the 5 half space
    for (const auto& wall : parent->walls) {
      if (wall->triangle().intersectHalfspaces(
              half_spaces, box->center,
              Rot3d::cube2sphere(box->rotation_center), outer_distance)) {
        box->walls.push_back(wall);
        box->status = MIXED;
      }
    }
  }
  // Translational box only
  else {
    for (const auto& corner : parent->corners) {
      double dist_corner = corner->point().distance(box->center);
      if (dist_corner <= outer_distance) {
        box->corners.push_back(corner);
        box->status = MIXED;
      }
    }
    for (const auto& edge : parent->edges) {
      double dist_edge = edge->segment().distance(box->center);
      if (dist_edge <= outer_distance) {
        box->edges.push_back(edge);
        box->status = MIXED;
      }
    }
    for (const auto& wall : parent->walls) {
      double dist_wall = wall->triangle().distance(box->center);
      if (dist_wall <= outer_distance) {
        box->walls.push_back(wall);
        box->status = MIXED;
      }
    }
  }

  if (verbose) {
    fprintf(debug_ptr, "size %d %d %d\n", box->corners.size(),
            box->edges.size(), box->walls.size());
    if (!box->corners.empty()) {
      for (const auto& corner : box->corners) {
        fprintf(debug_ptr, "corner\n");
        fprintf(debug_ptr, "%f %f %f\n", corner->point().X(),
                corner->point().Y(), corner->point().Z());
      }
    }
    if (!box->edges.empty()) {
      for (const auto& edge : box->edges) {
        fprintf(debug_ptr, "edge\n");
        fprintf(debug_ptr, "%f %f %f\n", edge->a->point().X(),
                edge->a->point().Y(), edge->a->point().Z());
        fprintf(debug_ptr, "%f %f %f\n", edge->b->point().X(),
                edge->b->point().Y(), edge->b->point().Z());
      }
    }
    if (!box->walls.empty()) {
      for (const auto& wall : box->walls) {
        fprintf(debug_ptr, "wall\n");
        fprintf(debug_ptr, "%f %f %f\n", wall->a->point().X(),
                wall->a->point().Y(), wall->a->point().Z());
        fprintf(debug_ptr, "%f %f %f\n", wall->b->point().X(),
                wall->b->point().Y(), wall->b->point().Z());
        fprintf(debug_ptr, "%f %f %f\n", wall->c->point().X(),
                wall->c->point().Y(), wall->c->point().Z());
      }
    }
    fprintf(debug_ptr, "box %f %f %f  %f\n", box->center.X(), box->center.Y(),
            box->center.Z(), box->width);
    if (box->rotation_width != -2) {
      fprintf(debug_ptr, "rot %f %f %f  %f\n", box->rotation_center.X(),
              box->rotation_center.Y(), box->rotation_center.Z(),
              box->rotation_width);
    }
  }
  if (box->corners.empty() && box->edges.empty() && box->walls.empty()) {
    box->status = classification(box->parent, box->center);
  }
}

// Sep(center, f) <= 2rB + clearance(center)
void ConfBox3dPredicate::checkVoronoiFeatureSet(ConfBox3d* box,
                                                ConfBox3d* parent) {
  double separation = 2 * box->rB + findCleanrance(box, box->rod);

  for (const auto& corner : parent->vor_corners) {
    if (corner->point().distance(box->center) <= separation) {
      box->vor_corners.push_back(corner);
    }
  }

  for (const auto& edge : parent->vor_edges) {
    if (edge->segment().distance(box->center) <= separation) {
      box->vor_edges.push_back(edge);
    }
  }

  for (const auto& wall : parent->walls) {
    if (wall->triangle().distance(box->center) <= separation) {
      box->vor_walls.push_back(wall);
    }
  }
}  // checkVoronoiFeatureSet

// find the nearest feature of rod, and return the distance
double ConfBox3dPredicate::findCleanrance(ConfBox3d* box, Segment3d& rod) {
  double clearance = std::numeric_limits<double>::max();
  for (const auto& wall : box->walls) {
    double dist = wall->triangle().separation(rod);
    if (dist < clearance) {
      clearance = dist;
    }
  }

  for (const auto& edge : box->edges) {
    double dist = edge->segment().separation(rod);
    if (dist < clearance) {
      clearance = dist;
    }
  }

  for (const auto& corner : box->corners) {
    double dist = corner->point().separation(rod);
    if (dist < clearance) {
      clearance = dist;
    }
  }

  return clearance;
}  // findCleanrance

// find the nearest voronoi feature of center and record the nearest feature
void ConfBox3dPredicate::findVorCleanrance(ConfBox3d* box, Segment3d& rod) {
  // use corners or vor_corners
  double mindistC = std::numeric_limits<double>::max();
  box->nearest_corner = nullptr;
  for (const auto& vor_corner : box->vor_corners) {
    double dist = vor_corner->point().separation(rod);
    if (dist < mindistC) {
      mindistC = dist;
      box->nearest_corner = vor_corner;
    }
  }

  double mindistE = std::numeric_limits<double>::max();
  box->nearest_edge = nullptr;
  for (const auto& vor_edge : box->vor_edges) {
    double dist = vor_edge->segment().separation(rod);
    if (dist < mindistE) {
      mindistE = dist;
      box->nearest_edge = vor_edge;
    }
  }

  double mindistW = std::numeric_limits<double>::max();
  box->nearest_wall = nullptr;
  for (const auto& vor_wall : box->vor_walls) {
    double dist = vor_wall->triangle().separation(rod);
    if (dist < mindistW) {
      mindistW = dist;
      box->nearest_wall = vor_wall;
    }
  }
}  // findVorCleanrance
