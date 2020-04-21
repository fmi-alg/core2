#include "./SoftSubdivisionSearch.h"

void printConfBoxes() {
  string enumNames[4] = { "FREE", "STUCK", "MIXED", "UNKNOWN" };
  for (int i = 0; i < ConfBox3d::boxes.size(); i++) {
    ConfBox3d* b = ConfBox3d::boxes[i];
    cout << "===================" << endl;
    cout << "Box " << b->boxId << ":" << endl;
    cout << b->x << "\t" << b->y << "\t" << b->z << "\t" << b->width << "\t" << enumNames[b->status] << endl;
    for (int i = 0; i < b->neighbors.size(); i++) {
      cout << b->neighbors[i]->boxId << "\t";
    }
    cout << endl;
    cout << "===================" << endl;
  }
}

// TODO: Let the constructor accept alpha and beta as configurations of the robot
SoftSubdivisionSearch::SoftSubdivisionSearch(ConfBox3d* root, double e, int qType, unsigned int s, double alpha[3], double beta[3]):
  epsilon(e), QType(qType), ct(0), pRoot(root),
  freeCount(0), stuckCount(0), mixCount(0), mixSmallCount(0) {
  this->alpha[0] = alpha[0];
  this->alpha[1] = alpha[1];
  this->alpha[2] = alpha[2];

  this->beta[0] = beta[0];
  this->beta[1] = beta[1];
  this->beta[2] = beta[2];
  switch (QType) {
  case 1:
    PQ = new SeqQueue();
    break;
  case 0:
    PQ = new RandQueue(s);
    break;
  case 2:
    PQ = new DijkstraQueue(alpha, beta);
    break;
  default:
    std::cerr << "Wrong QType" << std::endl;
    exit(1);
  }
  predicate = new ConfBox3dPredicate();

  predicate->classify(pRoot);
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
      predicate->classify(b->children[i]);
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
  for (int i = 0; i < b->neighbors.size(); i++) {
    ConfBox3d* n = b->neighbors[i];
    if (n->status == FREE) {
      if (n->pSet == 0) {
        new Set(n);
      }
      pSets->Union(b, n);
    }
  }
}

ConfBox3d* SoftSubdivisionSearch::findEnclosingFreeBox(double coordinate[3]) {
  ConfBox3d* box = pRoot->getBox(coordinate[0], coordinate[1], coordinate[2]);
  while (box && !(box)->isFree()) {
    if (!expand(box)) {
      return NULL; // Does not have a free box for the given resolution
    }
    ++ct;
    box = box->getBox(coordinate[0], coordinate[1], coordinate[2]);
  }
  return box;
}

// The SSS Framework as described in the RSS 2013 RCV Paper, section 7
vector<ConfBox3d*> SoftSubdivisionSearch::softSubdivisionSearch() {
  ConfBox3d::boxes.clear();
  vector<ConfBox3d*> path;
  path.clear();

  // 1. Initialization
  ConfBox3d* boxA = findEnclosingFreeBox(alpha);
  ConfBox3d* boxB = findEnclosingFreeBox(beta);
  // printConfBoxes();
  if (boxA == NULL) {
    cout << "STUCK: Initial configuration is not free" << endl;
    return path;
  } else if (boxB == NULL) {
    cout << "STUCK: Final configuration is not free" << endl;
    return path;
  }
  // printConfBoxes();
  // cout << boxA->boxId << "\t" << boxB->boxId << endl;

  // 2. Main
  while (!pSets->isConnect(boxA, boxB)) {
    if (PQ->empty()) {
      // printConfBoxes();
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

  // 3. Compute Free Channel
  // path = getCanonicalPath(boxA, boxB);
  path = Path::bfsShortestPath(boxA, boxB);
  path.insert(path.begin(), ConfBox3d::makePointBox(beta[0], beta[1], beta[2]));
  path.push_back(ConfBox3d::makePointBox(alpha[0], alpha[1], alpha[2]));
  //Path::printPath(path);
  // printConfBoxes();
  return path;
}
