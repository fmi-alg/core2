#include "./Path.h"

bool Path::isNeighbor(ConfBox3d* a, ConfBox3d* b) {
  double dx = abs(a->x - b->x);
  double dy = abs(a->y - b->y);
  double dz = abs(a->z - b->z);
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

vector<ConfBox3d*> Path::dijkstraShortestPath(ConfBox3d* a, ConfBox3d* b) {
  a->dist2Source = 0;
  vector<ConfBox3d*> bv;
  distHeap<distCmp>::insert(bv, a);
  while (bv.size()) {
    ConfBox3d* current = distHeap<distCmp>::extractMin(bv);
    current->visited = true;
    if (current == b) {
      break;
    }

    for (int i = 0; i < current->neighbors.size(); i++) {
      ConfBox3d* neighbor = current->neighbors[i];
      if (!neighbor->visited && neighbor->status == FREE) {
        double dist2pre =
          sqrt((current->x - neighbor->x) * (current->x - neighbor->x) +
               (current->y - neighbor->y) * (current->y - neighbor->y) +
               (current->z - neighbor->z) * (current->z - neighbor->z) );
        double dist2src = dist2pre + current->dist2Source;

        if (neighbor->dist2Source == -1) {
          neighbor->prev = current;
          neighbor->dist2Source = dist2src;
          distHeap<distCmp>::insert(bv, neighbor);
        } else if (neighbor->dist2Source > dist2src) {
          neighbor->prev = current;
          distHeap<distCmp>::decreaseKey(bv, neighbor, dist2src);
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
    std::cout << path[i]->x << "\t" << path[i]->y << "\t" << path[i]->z << "\t" << path[i]->rot_x << "\t" << path[i]->rot_y << "\t" << path[i]->rot_z << std::endl;
  }
}
