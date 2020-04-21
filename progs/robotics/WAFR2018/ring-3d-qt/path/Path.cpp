#include "path/Path.h"

bool Path::isNeighbor(ConfBox3d* a, ConfBox3d* b) {
  double dx = abs(a->mB.X() - b->mB.X());
  double dy = abs(a->mB.Y() - b->mB.Y());
  double dz = abs(a->mB.Z() - b->mB.Z());
  double wa = a->width / 2;
  double wb = b->width / 2;
  return
    (abs(dx - (wa + wb)) < 0.001 && abs(dy - abs(wa - wb)) < 0.001 && abs(dz - abs(wa - wb)) < 0.001) ||
    (abs(dx - (wa - wb)) < 0.001 && abs(dy - abs(wa + wb)) < 0.001 && abs(dz - abs(wa - wb)) < 0.001) ||
    (abs(dx - (wa - wb)) < 0.001 && abs(dy - abs(wa - wb)) < 0.001 && abs(dz - abs(wa + wb)) < 0.001);
}

vector<ConfBox3d*> Path::bfsShortestPath(ConfBox3d* a, ConfBox3d* b) {
  unordered_set<ConfBox3d*> visited;
  vector<ConfBox3d*> fringe;
  fringe.push_back(a);
  visited.insert(a);
  int begin = 0;
  while (begin < fringe.size()) {
    ConfBox3d* c = fringe[begin];
    begin++;
    for (int i = 0; i < c->neighbors.size(); i++) {
      ConfBox3d* n = c->neighbors[i];
      if (n->getStatus() == FREE && visited.find(n) == visited.end()) {
        n->prev = c;
        fringe.push_back(n);
        visited.insert(n);
      }
    }
  }

  vector<ConfBox3d*> path;
  path.push_back(b);
  while (path.back()->prev) {
    path.push_back(path.back()->prev);
  }
  return path;
}

vector<ConfBox3d*> Path::dijkstraSafePath(ConfBox3d* a, ConfBox3d* b) {
  a->distDij = 0;
  vector<ConfBox3d*> bv;
  distHeap<VorCmp>::insert(bv, a);
  while (bv.size()) {
    ConfBox3d* current = distHeap<VorCmp>::extractMin(bv);
    if(current->visited) continue;
    current->visited = true;

    for (int i = 0; i < current->neighbors.size(); i++) {
      ConfBox3d* neighbor = current->neighbors[i];
      if (!neighbor->visited && neighbor->status == FREE) {
        double dist2pre = current->mB.distance(neighbor->mB);
        if(current->rot_width != -2 && neighbor->rot_width != -2)
          dist2pre += current->rot_mB.distance(neighbor->rot_mB);
        double dist2src = dist2pre + current->distDij;

        if (neighbor->distDij == -1) {
          neighbor->prev = current;
          neighbor->distDij = dist2src;
          distHeap<VorCmp>::insert(bv, neighbor);
        } else {
          if (neighbor->distDij > dist2src) {
            neighbor->prev = current;
            distHeap<VorCmp>::decreaseKey(bv, neighbor, dist2src);
          }
        }
      }
    }
  }

  vector<ConfBox3d*> path;
  path.push_back(b);
  while (path.back()->prev) {
    path.push_back(path.back()->prev);
  }
  return path;
}

vector<ConfBox3d*> Path::dijkstraShortestPath(ConfBox3d* a, ConfBox3d* b) {
  a->distDij = 0;
  vector<ConfBox3d*> bv;
  distHeap<PathCmp>::insert(bv, a);
  while (bv.size()) {
    ConfBox3d* current = distHeap<PathCmp>::extractMin(bv);
    if(current->visited) continue;
    current->visited = true;

    for (unsigned i = 0; i < current->neighbors.size(); ++i) {
      ConfBox3d* neighbor = current->neighbors[i];
      if (!neighbor->visited && neighbor->status == FREE) {
        double dist2pre = current->mB.distance(neighbor->mB);
        if(current->rot_width != -2 && neighbor->rot_width != -2)
          dist2pre += current->rot_mB.distance(neighbor->rot_mB);
        double dist2src = dist2pre + current->distDij;

        if (neighbor->distDij == -1) {
          neighbor->prev = current;
          neighbor->distDij = dist2src;
          distHeap<PathCmp>::insert(bv, neighbor);
        } else {
          if (neighbor->distDij > dist2src) {
            neighbor->prev = current;
            distHeap<PathCmp>::decreaseKey(bv, neighbor, dist2src);
          }
        }
      }
    }
  }

  vector<ConfBox3d*> path;
  path.push_back(b);
  while (path.back()->prev) {
    path.push_back(path.back()->prev);
  }
  return path;
}

void Path::printPath(vector<ConfBox3d*> path) {
  std::cout << "Path:" << std::endl;
  std::cout << "Length: " << path.size() << std::endl;
  for (int i = 0; i < path.size(); i++) {
    std::cout << path[i]->mB.X() << "\t" << path[i]->mB.Y() << "\t" << path[i]->mB.Z() << "\t" << path[i]->rot_mB.X() << "\t" << path[i]->rot_mB.Y() << "\t" << path[i]->rot_mB.Z() << std::endl;
  }
}
