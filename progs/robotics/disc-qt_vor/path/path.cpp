#include "path.h"

vector<Box*> Path::bfsShortestPath(Box* a, Box* b) {
  unordered_set<Box*> visited;
  vector<Box*> fringe;
  fringe.push_back(a);
  visited.insert(a);
  int begin = 0;
  while (begin < fringe.size()) {
    Box* current = fringe[begin];
    begin++;

    for(int i=0; i<4; ++i){
      BoxIter* iter = new BoxIter(current, i);
      Box* neighbor = iter->First();
      while(neighbor && neighbor != iter->End()){
        if(neighbor->getBoxStatus() == FREE && visited.find(neighbor) == visited.end()){
          neighbor->prev = current;
          fringe.push_back(neighbor);
          visited.insert(neighbor);
        }
        neighbor = iter->Next();
      }
    }
  }

  vector<Box*> path;
  path.push_back(b);
  while (path.back()->prev) {
    path.push_back(path.back()->prev);
  }
  return path;
}

vector<Box*> Path::safeShortestPath(Box* a, Box* b){

  a->distDij = 0;
  vector<Box*> bv;
  distHeap<VorCmp>::insert(bv, a);

  while(bv.size()){
    Box* current = distHeap<VorCmp>::extractMin(bv);
    if(current->visited) continue;
    current->visited = true;

    for(int i=0; i<4; ++i){
      BoxIter* iter = new BoxIter(current, i);
      Box* neighbor = iter->First();
      while(neighbor && neighbor != iter->End()){
        if(!neighbor->visited && neighbor->status == FREE){
          double dist2pre = current->mB.distance(neighbor->mB);
          double dist2src = dist2pre + current->distDij;
          if(neighbor->distDij == -1){
            neighbor->prev = current;
            neighbor->distDij = dist2src;
            distHeap<VorCmp>::insert(bv, neighbor);
          }
          else{
            if(neighbor->distDij > dist2src){
              neighbor->prev = current;
              distHeap<VorCmp>::decreaseKey(bv, neighbor, dist2src);
            }
          }
        }
        neighbor = iter->Next();
      }
    }
  }

  vector<Box*> path;
  path.push_back(b);
  while(path.back()->prev){
    path.push_back(path.back()->prev);
  }

  return path;
}

vector<Box*> Path::dijkstraShortestPath(Box* a, Box* b) {

  a->distDij = 0;
  vector<Box*> bv;
  distHeap<distCmp>::insert(bv, a);

  while(bv.size()){
    Box* current = distHeap<distCmp>::extractMin(bv);
    if(current->visited) continue;
    current->visited = true;

    for(int i=0; i<4; ++i){
      BoxIter* iter = new BoxIter(current, i);
      Box* neighbor = iter->First();
      while(neighbor && neighbor != iter->End()){
        if(!neighbor->visited && neighbor->status == FREE){
          double dist2pre = current->mB.distance(neighbor->mB);
          double dist2src = dist2pre + current->distDij;
          if(neighbor->distDij == -1){
            neighbor->prev = current;
            neighbor->distDij = dist2src;
            distHeap<distCmp>::insert(bv, neighbor);
          }
          else{
            if(neighbor->distDij > dist2src){
              neighbor->prev = current;
              distHeap<distCmp>::decreaseKey(bv, neighbor, dist2src);
            }
          }
        }
        neighbor = iter->Next();
      }
    }
  }

  vector<Box*> path;
  path.push_back(b);
  while(path.back()->prev){
    path.push_back(path.back()->prev);
  }

  return path;
}

void Path::printPath(vector<Box*> path) {
  std::cout << "Path:" << std::endl;
  std::cout << "Length: " << path.size() << std::endl;
  for (int i = 0; i < path.size(); i++) {
    std::cout << path[i]->mB.X() << "\t" << path[i]->mB.Y() << "\t" << std::endl;
  }
}
