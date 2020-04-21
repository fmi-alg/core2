/*
 * path.cpp
 *
 * Created on: July 1, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#include <unordered_set>

#include "path/path.h"

extern double EPS;

bool Path::isNeighbor(const ConfBox3d& a, const ConfBox3d& b) {
  double dx = fabs(a.center.X() - b.center.X());
  double dy = fabs(a.center.Y() - b.center.Y());
  double dz = fabs(a.center.Z() - b.center.Z());
  double wa = a.width / 2.0;
  double wb = b.width / 2.0;
  return (fabs(dx - (wa + wb)) < EPS && fabs(dy - fabs(wa - wb)) < EPS &&
          fabs(dz - fabs(wa - wb)) < EPS) ||
         (fabs(dx - (wa - wb)) < EPS && fabs(dy - fabs(wa + wb)) < EPS &&
          fabs(dz - fabs(wa - wb)) < EPS) ||
         (fabs(dx - (wa - wb)) < EPS && fabs(dy - fabs(wa - wb)) < EPS &&
          fabs(dz - fabs(wa + wb)) < EPS);
}

std::vector<ConfBox3d*> Path::bfsShortestPath(ConfBox3d* a, ConfBox3d* b) {
  std::unordered_set<ConfBox3d*> visited;
  std::vector<ConfBox3d*> fringe;
  fringe.push_back(a);
  visited.insert(a);
  unsigned begin = 0;
  while (begin < fringe.size()) {
    ConfBox3d* box = fringe[begin];
    ++begin;
    for (auto&& neighbor : box->neighbors) {
      if (neighbor->getStatus() == FREE &&
          visited.find(neighbor) == visited.end()) {
        neighbor->prev = box;
        fringe.push_back(neighbor);
        visited.insert(neighbor);
      }
    }
  }

  std::vector<ConfBox3d*> path;
  path.push_back(b);
  while (path.back()->prev) {
    path.push_back(path.back()->prev);
  }
  return path;
}

std::vector<ConfBox3d*> Path::dijkstraSafePath(ConfBox3d* a, ConfBox3d* b) {
  a->dist_Dijkstra = 0;
  std::vector<ConfBox3d*> bv;
  distHeap<VorCmp>::insert(bv, a);
  while (bv.size()) {
    ConfBox3d* current = distHeap<VorCmp>::extractMin(bv);
    if (current->visited) continue;
    current->visited = true;

    for (int i = 0; i < current->neighbors.size(); i++) {
      ConfBox3d* neighbor = current->neighbors[i];
      if (!neighbor->visited && neighbor->status == FREE) {
        double dist2pre = current->center.distance(neighbor->center);
        if (current->rotation_width != -2 && neighbor->rotation_width != -2) {
          dist2pre += rotationLength(current->rotation_center,
                                     neighbor->rotation_center);
        }
        double dist2src = dist2pre + current->dist_Dijkstra;

        if (neighbor->dist_Dijkstra == -1) {
          neighbor->prev = current;
          neighbor->dist_Dijkstra = dist2src;
          distHeap<VorCmp>::insert(bv, neighbor);
        } else {
          if (neighbor->dist_Dijkstra > dist2src) {
            neighbor->prev = current;
            distHeap<VorCmp>::decreaseKey(bv, neighbor, dist2src);
          }
        }
      }
    }
  }

  std::vector<ConfBox3d*> path;
  path.push_back(b);
  while (path.back()->prev) {
    path.push_back(path.back()->prev);
  }
  return path;
}

std::vector<ConfBox3d*> Path::dijkstraShortestPath(ConfBox3d* a, ConfBox3d* b) {
  a->dist_Dijkstra = 0;
  std::vector<ConfBox3d*> bv;
  distHeap<PathCmp>::insert(bv, a);
  while (bv.size()) {
    ConfBox3d* current = distHeap<PathCmp>::extractMin(bv);
    if (current->visited) continue;
    current->visited = true;

    for (unsigned i = 0; i < current->neighbors.size(); ++i) {
      ConfBox3d* neighbor = current->neighbors[i];
      if (!neighbor->visited && neighbor->status == FREE) {
        double dist2pre = current->center.distance(neighbor->center);
        //        if(current->rotation_width != -2 && neighbor->rotation_width != -2){
        //          dist2pre += rotationLength(current->rotation_center,
        //          neighbor->rotation_center);
        //        }
        double dist2src = dist2pre + current->dist_Dijkstra;

        if (neighbor->dist_Dijkstra == -1) {
          neighbor->prev = current;
          neighbor->dist_Dijkstra = dist2src;
          distHeap<PathCmp>::insert(bv, neighbor);
        } else {
          if (neighbor->dist_Dijkstra > dist2src) {
            neighbor->prev = current;
            distHeap<PathCmp>::decreaseKey(bv, neighbor, dist2src);
          }
        }
      }
    }
  }

  std::vector<ConfBox3d*> path;
  path.push_back(b);
  while (path.back()->prev) {
    path.push_back(path.back()->prev);
  }
  return path;
}

void Path::printPath(std::vector<ConfBox3d*> path) {
  std::cout << "Path:" << std::endl;
  std::cout << "Length: " << path.size() << std::endl;
  for (const auto& point : path) {
    std::cout << point->center.X() << "\t" << point->center.Y() << "\t"
              << point->center.Z() << "\t" << point->rotation_center.X() << "\t"
              << point->rotation_center.Y() << "\t"
              << point->rotation_center.Z() << std::endl;
  }
}
