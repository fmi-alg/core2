#include "sss/SoftSubdivisionSearch.h"

extern FILE* g_fptr;
extern MainWindow *window;
extern bool verbose;

void printConfBoxes() {
  string enumNames[4] = { "FREE", "STUCK", "MIXED", "UNKNOWN" };
  for (int i = 0; i < ConfBox3d::boxes.size(); i++) {
    ConfBox3d* b = ConfBox3d::boxes[i];
    cout << "===================" << endl;
    cout << "Box " << b->boxId << ":" << endl;
    cout << b->mB.X() << "\t" << b->mB.Y() << "\t" << b->mB.Z() << "\t" << b->width << "\t" << enumNames[b->status] << endl;
    for (int i = 0; i < b->neighbors.size(); i++) {
      cout << b->neighbors[i]->boxId << "\t";
    }
    cout << endl;
    cout << "===================" << endl;
  }
}

// TODO: Let the constructor accept start and goal as configurations of the robot
SoftSubdivisionSearch::SoftSubdivisionSearch(ConfBox3d* root, double e, int searchType, Point3d start, Point3d goal, Point3d startRot, Point3d goalRot):
  epsilon(e), searchType(searchType), ct(0), pRoot(root),
  freeCount(0), stuckCount(0), mixCount(0), mixSmallCount(0) {

  this->start = start;
  this->goal = goal;
  this->startRot = startRot;
  this->goalRot = goalRot;

  switch (searchType) {
    case RANDOM:
      PQ = new RandQueue();
      break;
    case BFS:
      PQ = new SeqQueue();
      break;
    case GREEDY:
      PQ = new DijkstraQueue<DistCmp>();
      break;
    case BIGREEDY:
      PQ = new DijkstraQueue<BiDistCmp>();
      break;
    case GREEDY_SIZE:
      PQ = new DijkstraQueue<DistSizeCmp>();
      break;
    case BIGREEDY_SIZE:
      PQ = new DijkstraQueue<BiDistSizeCmp>();
      break;
    case VORONOI:
      PQ = new DijkstraQueue<VorCmp>();
      break;
    case BIVORONOI:
      PQ = new DijkstraQueue<BiVorCmp>();
      break;
    default:
      std::cerr << "Wrong searchType" << std::endl;
      exit(1);
  }

  predicate = new ConfBox3dPredicate();
  if(searchType == VORONOI || searchType == BIVORONOI){
    predicate->checkVoronoiFeatureSet(pRoot);
    predicate->findCleanrance2(pRoot, pRoot->ring);
  }

  predicate->checkCollisionDetectionFeatureSet(pRoot);
  insertNode(pRoot);
}

void SoftSubdivisionSearch::insertNode(ConfBox3d* b) {

  PQ->process(b);

  switch (b->getStatus()) {
  case FREE:
    unionAdjacent(b);
    ++freeCount;
    break;
  case STUCK:
    ++stuckCount;
    break;
  case MIXED:
    ++mixCount;
    if (b->width < epsilon) {
      ++mixSmallCount;
    }
    break;
  case UNKNOWN:
    std::cout << "UNKNOWN not handled" << std::endl;
    break;
  default:
    std::cerr << "Wrong Status" << std::endl;
    exit(1);
  }
}

bool SoftSubdivisionSearch::expand(ConfBox3d* b) {

  if (b->split(epsilon)) {
    for (int i = 0; i < b->children.size(); ++i) {
      if(searchType == VORONOI || searchType == BIVORONOI){
        predicate->checkVoronoiFeatureSet(b->children[i]);
        predicate->findCleanrance2(b->children[i], b->children[i]->ring);
      }

      predicate->checkCollisionDetectionFeatureSet(b->children[i]);
      insertNode(b->children[i]);
    }
    return true;
  }
  return false;
}

void SoftSubdivisionSearch::unionAdjacent(ConfBox3d* b) {
  if (b->getStatus() != FREE) {
    cerr << "Cannot union boxes that are mixed or stuck" << endl;
    exit(1);
  }
  if (b->pSet == 0) {
    new Set(b);
  }
  for (unsigned i = 0; i < b->neighbors.size(); i++) {
    ConfBox3d* n = b->neighbors[i];
    if (n->status == FREE) {
      if (n->pSet == 0) {
        new Set(n);
      }
      pSets->Union(b, n);
    }
  }
}

ConfBox3d* SoftSubdivisionSearch::findEnclosingFreeBox(Point3d p, Point3d pRot) {
  ConfBox3d* box = pRoot->getBox(p, pRot);
  while (box && !(box)->isFree()) {
    if (!expand(box)) {
      return 0; // Does not have a free box for the given resolution
    }
    ++ct;
    box = box->getBox(p, pRot);
  }
  return box;
}

template<typename Cmp>
bool SoftSubdivisionSearch::findPath(ConfBox3d* boxA, ConfBox3d* boxB, int& ct) {

    boxA->dist2Source = 0;
    DijkstraQueue<Cmp> dijQ;
    dijQ.push(boxA);

    DijkstraQueue<Cmp> dijQ2;
    if(searchType == BIGREEDY || searchType == BIGREEDY_SIZE || searchType == BIVORONOI){
      boxB->dist2Source = 0;
      dijQ2.push(boxB);
    }

    bool alter = true;

    while (!pSets->isConnect(boxA, boxB) && !dijQ.empty()) {

        ConfBox3d* current;

        if(searchType == BIGREEDY || searchType == BIGREEDY_SIZE || searchType == BIVORONOI){
          if(dijQ.empty() || dijQ2.empty()){
            break;
          }
          if(alter) current = dijQ.extract();
          else      current = dijQ2.extract();
        }
        else{
          current = dijQ.extract();
        }

        // if current is MIXED, try expand it and push the children that is
        // ACTUALLY neighbors of the source set (set containing start) into the dijQ again
        if (current->status == MIXED) {
            if (expand(current)) {
                ++ct;
                for (unsigned i = 0; i < current->children.size(); ++i) {
                    if(current->children[i]->inQueue) continue;

                    // go through neighbors of each child to see if it's in source set
                    // if yes, this child go into the dijQ
                    bool isNeighborOfSourceSet = false;
                    for (unsigned j = 0; j < current->children[i]->neighbors.size() && !isNeighborOfSourceSet; ++j) {
                        ConfBox3d* n = current->children[i]->neighbors[j];
                        if (n->dist2Source == 0) {
                            isNeighborOfSourceSet = true;
                            break;
                        }
                    }

                    if (isNeighborOfSourceSet) {
                        switch (current->children[i]->getStatus()) {
                        //if it's FREE, also insert to source set
                        case FREE:
                            //current->children[i]->dist2Source = 0;
                        case MIXED:
                            current->children[i]->inQueue = true;
                            current->children[i]->bidirection = current->bidirection;

                            if(alter) dijQ.push(current->children[i]);
                            else      dijQ2.push(current->children[i]);
                            break;
                        case STUCK:
                            //cerr << "inside FindPath: STUCK case not treated" << endl;
                            break;
                        case UNKNOWN:
                            //cerr << "inside FindPath: UNKNOWN case not treated" << endl;
                            break;
                        }
                    }
                }
            }
        } //end if (current->status == MIXED)

        //found path!
        if(current == boxB &&
           !(searchType == BIGREEDY || searchType == BIGREEDY_SIZE || searchType == BIVORONOI)){
          return true;
        }

        if (current->status == FREE) {
            current->dist2Source = 0;
            // if current is not MIXED, then must be FREE
            // go through it's neighbors and add FREE and MIXED ones to dijQ
            // also add FREE ones to source set
            for (unsigned i = 0; i < current->neighbors.size(); ++i) {
                ConfBox3d* neighbor = current->neighbors[i];
                if(!neighbor->inQueue && neighbor->dist2Source == -1 &&
                   (neighbor->status == FREE || neighbor->status == MIXED)) {
                  //if(neighbor->status == FREE) neighbor->dist2Source = 0;

                  neighbor->inQueue = true;
                  neighbor->bidirection = current->bidirection;
                  if(alter) dijQ.push(neighbor);
                  else      dijQ2.push(neighbor);
                }
            }
        } //end if (current->status == FREE)

        if(searchType == BIGREEDY || searchType == BIGREEDY_SIZE || searchType == BIVORONOI) alter = !alter;
    }


    if(!(searchType == BIGREEDY || searchType == BIGREEDY_SIZE || searchType == BIVORONOI))
      return !dijQ.empty();
    else
      return !dijQ.empty() && !dijQ2.empty();
}

template<typename Cmp>
ConfBox3d* SoftSubdivisionSearch::findVorBox(ConfBox3d* freeBox, int& ct) {

    freeBox->dist2Source = 0;
    DijkstraQueue<Cmp> dijQ;
    dijQ.push(freeBox);

    while (!dijQ.empty()) {

        ConfBox3d* current = dijQ.extract();

        // if current is MIXED, try expand it and push the children that is
        // ACTUALLY neighbors of the source set (set containing start) into the dijQ again
        if (current->status == MIXED) {
            if (expand(current)) {
                ++ct;
                for (unsigned i = 0; i < current->children.size(); ++i) {
                    // go through neighbors of each child to see if it's in source set
                    // if yes, this child go into the dijQ
                    bool isNeighborOfSourceSet = false;
                    for (unsigned j = 0; j < current->children[i]->neighbors.size() && !isNeighborOfSourceSet; ++j) {
                        ConfBox3d* n = current->children[i]->neighbors[j];
                        if (n->dist2Source == 0) {
                            isNeighborOfSourceSet = true;
                            break;
                        }
                    }

                    if (isNeighborOfSourceSet) {
                        switch (current->children[i]->getStatus()) {
                        //if it's FREE, also insert to source set
                        case FREE:
                        case MIXED:
                            current->children[i]->inVorQueue = true;
                            current->children[i]->bidirection = current->bidirection;
                            dijQ.push(current->children[i]);
                            break;
                        case STUCK:
                            //cerr << "inside FindPath: STUCK case not treated" << endl;
                            break;
                        case UNKNOWN:
                            //cerr << "inside FindPath: UNKNOWN case not treated" << endl;
                            break;
                        }
                    }
                }
            }
        } //end if (current->status == MIXED)

        //found path!
        if(current->status == FREE &&
           current->vorCorners.size()+current->vorEdges.size()+current->vorWalls.size() > 2){
          return current;
        }

        if (current->status == FREE) {
            current->dist2Source = 0;
            // if current is not MIXED, then must be FREE
            // go through it's neighbors and add FREE and MIXED ones to dijQ
            // also add FREE ones to source set
            for (unsigned i = 0; i < current->neighbors.size(); ++i) {
                ConfBox3d* neighbor = current->neighbors[i];
                if(!neighbor->inVorQueue && neighbor->dist2Source == -1 &&
                   (neighbor->status == FREE || neighbor->status == MIXED)) {
                  neighbor->inVorQueue = true;
                  neighbor->bidirection = current->bidirection;
                  dijQ.push(neighbor);
                }
            }
        } //end if (current->status == FREE)
    }
    return freeBox;
}

// The main SSS Framework as described in the RSS 2013 RCV Paper, section 7
vector<ConfBox3d*> SoftSubdivisionSearch::softSubdivisionSearch() {
  ConfBox3d::boxes.clear();
  vector<ConfBox3d*> path;
  path.clear();

  if(verbose) fprintf(g_fptr, "program start %d\n", searchType);

  // 1. Initialization
  moveAway = true; // flag for finding voronoi box
  ConfBox3d* boxA = findEnclosingFreeBox(start, startRot);
  if (boxA == NULL) {
    (*window) << "STUCK: Initial configuration is not free\n";
    return path;
  }
  fprintf(g_fptr, "box A finish %d Bt (%f, %f, %f)", boxA->status, boxA->mB.X(), boxA->mB.Y(), boxA->mB.Z());
  if(boxA->rot_width != -2) fprintf(g_fptr, " Br (%f, %f, %f)", boxA->rot_mB.X(), boxA->rot_mB.Y(), boxA->rot_mB.Z());
  fprintf(g_fptr, "\n");
  boxA->bidirection = 1;
  if(searchType == VORONOI || searchType == BIVORONOI){
    boxA = findVorBox<DistCmpSpecial>(boxA, ct);
    if(verbose) fprintf(g_fptr, "vor box A finish %d\n", boxA->status);
    if (boxA == NULL) {
      (*window) << "STUCK: Cannot find Vor box A\n";
      return path;
    }
  }
  boxA->bidirection = 1;

  ConfBox3d* boxB = findEnclosingFreeBox(goal, goalRot);
  // printConfBoxes();
  if (boxB == NULL) {
    (*window) << "STUCK: Final configuration is not free\n";
    return path;
  }
  fprintf(g_fptr, "box B finish %d Bt (%f, %f, %f)", boxB->status, boxB->mB.X(), boxB->mB.Y(), boxB->mB.Z());
  if(boxA->rot_width != -2) fprintf(g_fptr, " Br (%f, %f, %f)", boxB->rot_mB.X(), boxB->rot_mB.Y(), boxB->rot_mB.Z());
  fprintf(g_fptr, "\n");
  boxB->bidirection = -1;
  if(searchType == VORONOI || searchType == BIVORONOI){
    boxB = findVorBox<DistCmpSpecial>(boxB, ct);
    if(verbose) fprintf(g_fptr, "vor box B finish %d\n", boxB->status);
    if (boxB == NULL) {
      (*window) << "STUCK: Cannot find Vor box B\n";
      return path;
    }
  }
  boxB->bidirection = -1;


  moveAway = false;
  // 2. Main
  if(searchType == RANDOM || searchType == BFS){
    while (!pSets->isConnect(boxA, boxB)) {
      if (PQ->empty()) {
        path.clear();
        return path;
      }
      ConfBox3d* b = PQ->extract();
      // b might not be a leaf since it could already be split in
      // expand(ConfBox3d* b), and PQ is not updated there
      if (b->isLeaf() && b->getStatus() == MIXED) {
        expand(b);
      }
      ++ct;
    }
  }
  else if(searchType == GREEDY){
    bool noPath = !findPath<DistCmp>(boxA, boxB, ct);
    if(noPath) return path;
  }
  else if(searchType == BIGREEDY){
    bool noPath = !findPath<BiDistCmp>(boxA, boxB, ct);
    if(noPath) return path;
  }
  else if(searchType == GREEDY_SIZE){
    bool noPath = !findPath<DistSizeCmp>(boxA, boxB, ct);
    if(noPath) return path;
  }
  else if(searchType == BIGREEDY_SIZE){
    bool noPath = !findPath<BiDistSizeCmp>(boxA, boxB, ct);
    if(noPath) return path;
  }
  else if(searchType == VORONOI){
    bool noPath = !findPath<VorCmp>(boxA, boxB, ct);
    if(noPath) return path;
  }
  else if(searchType == BIVORONOI){
    bool noPath = !findPath<BiVorCmp>(boxA, boxB, ct);
    if(noPath) return path;
  }

  // 3. Compute Free Channel
  //path = Path::bfsShortestPath(boxA, boxB);
  //path = Path::dijkstraSafePath(boxA, boxB);
  path = Path::dijkstraShortestPath(boxA, boxB);
  path.insert(path.begin(), ConfBox3d::makePointBox(goal, goalRot));
  path.push_back(ConfBox3d::makePointBox(start, startRot));

  return path;
}
